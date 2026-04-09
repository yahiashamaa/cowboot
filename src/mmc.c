#include "inc/types.h"
#include "inc/io.h"
#include "inc/timer.h"
#include "inc/mmc.h"
#include "inc/regs.h"
#include "inc/console.h"

static int mmc_poll_status(u32 status) {
    u32 timeout = 50 * 1000 * 1000; // 50 seconds

    while (timeout--) {
        u32 stat = readl(MMC0_BASE + MMC_STAT);

        if (stat & ERROR_STAT)
        {
            video_printf("stat error status");
            return -1;
        }
        
        
        if (stat & status)
            return 0;

        udelay(1);
    }

    video_printf("mmc poll status timeout");
    return -1;
}

static int mmc_await_response(void) {
    if (mmc_poll_status(STAT_END_CMD_RES) < 0) {
        video_printf("await response timeout");
        return -1;
    }

    writel(IFLG_END_CMD_RES, MMC0_BASE + MMC_IFLG);

    return 0;
}

static inline void clear_mmc_irq(u32 bits) {
    writel(bits, MMC0_BASE + MMC_IFLG);
}


static int _mmc_read_data(struct mmc_data *data) {
    int sz = data->blocks * data->blocksize;
    void *buf = data->dest;
    u32 stat;

    do {
        stat = readl(MMC0_BASE + MMC_STAT);

        if (stat & STAT_TIME_OUT_READ) {
            video_printf("mmc read timeout");
            return -1;
        }
        if (stat & STAT_CRC_READ_ERROR) {
            video_printf("mmc crc error");
            return -1;
        }
        if (stat & STAT_DATA_FIFO_EMPTY) {
            udelay(100);
            continue;
        }
        do {
            u32 val = readl(MMC0_BASE + MMC_RXFIFO);

            if (sz == 1)
                *(u8 *)buf = (u8)val;
            else if (sz == 2)
                *(u16 *)buf = (u16)val;
            else if (sz >= 4)
                *(u32 *)buf = val;

            buf = (u8 *)buf + 4;
            sz -= 4;
            stat = readl(MMC0_BASE + MMC_STAT);

        } while (!(stat & STAT_DATA_FIFO_EMPTY) && sz > 0);

    } while (!(stat & STAT_DATA_TRAN_DONE) && sz > 0);

    while (!(readl(MMC0_BASE + MMC_IFLG) & IFLG_DATA_TRAN_DONE));
    
    clear_mmc_irq(IFLG_DATA_TRAN_DONE);

    return 0;
}

static int mmc_send_cmd(u32 opcode, u32 arg, u32 resp_type, u32 resp[], struct mmc_data *data) {
	u32 cmdat = 0;

    writel(opcode, MMC0_BASE + MMC_CMD);
    writel(arg, MMC0_BASE + MMC_ARG);

    if (data) {
        cmdat |= CMDAT_DATA_EN;
        if (data->flags & MMC_DATA_WRITE)
            cmdat |= CMDAT_WRITE_READ;

        writel(data->blocks, MMC0_BASE + MMC_NOB);
        writel(data->blocksize, MMC0_BASE + MMC_BLKLEN);
    }

    switch (resp_type) {
        case MMC_RSP_R1:
        case MMC_RSP_R1b:
            cmdat |= CMDAT_RESPONSE_R1;
            break;
        case MMC_RSP_R2:
            cmdat |= CMDAT_RESPONSE_R2;
            break;
        case MMC_RSP_R3:
            cmdat |= CMDAT_RESPONSE_R3;
            break;
        default:
            break;
    }

     if (resp_type & MMC_RSP_BUSY)
        cmdat |= CMDAT_BUSY;


    /* This piece of shit, made me sleep at 5 am
    * for a few days trying to debug it. Apperantly 
    * U-boot we've been piggybacking on has left the mmc in
    * 8 bit mode. I was reading garbled data as I was reading 
    * in 1 bit mode. I'm tired, I feel that I've wasted too much
    * time. I tried everything, and this is the thing that comes
    * and bites me in the ass. Why? what have I done wrong?
    */
    cmdat |= CMDAT_BUS_WIDTH_8BIT;   // (0x3 << 9)



    writel(cmdat, MMC0_BASE + MMC_CMDAT);

    // Set timeout and clear interrupts before starting the command
    writel(0xffffffff, MMC0_BASE + MMC_RDTO);
    writel(0xffffffff, MMC0_BASE + MMC_IMASK);
    writel(0xffffffff, MMC0_BASE + MMC_IFLG);
    
    writel(CTRL_START_OP, MMC0_BASE + MMC_CTRL);

    if (mmc_await_response() < 0){
        return -1;
    }


    if (resp && (resp_type & MMC_RSP_PRESENT)) {
        if (resp_type & MMC_RSP_136) {
            u16 a, b, c, i;
            a = readw(MMC0_BASE + MMC_RES);
            for (i = 0; i < 4; i++) {
                b = readw(MMC0_BASE + MMC_RES);
                c = readw(MMC0_BASE + MMC_RES);
                resp[i] = (a << 24) | (b << 8) | (c >> 8);
                a = c;
            }

        } else {
            resp[0] = readw(MMC0_BASE + MMC_RES) << 24;
            resp[0] |= readw(MMC0_BASE + MMC_RES) << 8;
            resp[0] |= readw(MMC0_BASE + MMC_RES) & 0xff;
        }
    }

    if (resp_type == MMC_RSP_R1b) {
        if (mmc_poll_status(STAT_PRG_DONE) < 0) 
            return -1;
        clear_mmc_irq(IFLG_PRG_DONE);
    }

    if (data && (data->flags & MMC_DATA_READ))
        return _mmc_read_data(data);

    return 0;
}


int mmc_send_status(void)
{
    int error = 0;
    int timeout = 10;
    u32 resp;
    
    do {
        /* For the device we are currently using, our mmc's rca is 0, this could 
         * differ if we plan to port other devices which may have more than one mmc
        */
        error = mmc_send_cmd(MMC_CMD_SEND_STATUS, 0x0, MMC_RSP_R1, &resp, 0);

        if (!error) {
            if ((resp & MMC_STATUS_RDY_FOR_DATA) && ((resp & MMC_STATUS_CURR_STATE) != MMC_STATE_PRG))
            {
                video_printf("card is ready");
                return 0;
            }
            if (resp & MMC_STATUS_MASK) 
            {   
                video_printf("card error");
                return -1;
            }
        } 

        udelay(1000);

    } while (timeout--);

    video_printf("timeout waiting for card status");
    return -1;
    
}

int mmc_read_data(u32 lba, u32 block_count, void *dest)
{

    u32 resp[4];
    struct mmc_data data = {
        .dest = dest,
        .blocks = block_count,
        .blocksize = 512,
        .flags = MMC_DATA_READ,
    };

    // Set block length to 512 bytes
    mmc_send_cmd(MMC_CMD_SET_BLOCKLEN, 512, MMC_RSP_R1, resp, 0);
    
    // Read multiple blocks
    mmc_send_cmd(MMC_CMD_READ_MULTIPLE_BLOCK, lba, MMC_RSP_R1, resp, &data);

    // Stop transmission
    mmc_send_cmd(MMC_CMD_STOP_TRANSMISSION, 0, MMC_RSP_R1b, resp, 0);  

    // Pretty simple, no?
    
    return 0;
}