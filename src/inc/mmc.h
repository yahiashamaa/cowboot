#ifndef MMC_H_
#define MMC_H_

struct mmc_data {
    union {
        char *dest;
        const char *src;
    };
    u32        blocksize;      /* data block size */
    u32        blocks;     /* number of blocks */
    u32        flags;
};

#define MMC0_BASE   0xb3450000

#define MMC_CTRL            0x000
#define MMC_STAT            0x004
#define MMC_CLKRT           0x008
#define MMC_CMDAT           0x00C
#define MMC_RESTO           0x010
#define MMC_RDTO            0x014
#define MMC_BLKLEN          0x018
#define MMC_NOB             0x01C
#define MMC_SNOB            0x020
#define MMC_IMASK           0x024
#define MMC_IFLG            0x028
#define MMC_CMD             0x02C
#define MMC_ARG             0x030
#define MMC_RES             0x034
#define MMC_RXFIFO          0x038

#define MMC_CMD_SEND_STATUS     13
#define MMC_STATUS_MASK     (~0x0206BF7F)
#define MMC_STATUS_RDY_FOR_DATA (1 << 8)
#define MMC_STATUS_CURR_STATE   (0xf << 9)
#define MMC_STATUS_ERROR    (1 << 19)

#define MMC_STATE_PRG       (7 << 9)

#define MMC_CMD_SEND_OP_COND        1
#define JZ_MMC_BUS_WIDTH_MASK   0x3
#define JZ_MMC_BUS_WIDTH_1      0x0
#define JZ_MMC_BUS_WIDTH_4      0x2
#define JZ_MMC_BUS_WIDTH_8      0x3
#define JZMMC_CARD_NEED_INIT    (1 << 2)

#define MMC_RSP_PRESENT (1 << 0)
#define MMC_RSP_136 (1 << 1)        /* 136 bit response */
#define MMC_RSP_CRC (1 << 2)        /* expect valid crc */
#define MMC_RSP_BUSY    (1 << 3)        /* card may send busy */
#define MMC_RSP_OPCODE  (1 << 4)        /* response contains opcode */

#define MMC_RSP_NONE    (0)
#define MMC_RSP_R1  (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R1b (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE| \
            MMC_RSP_BUSY)
#define MMC_RSP_R2  (MMC_RSP_PRESENT|MMC_RSP_136|MMC_RSP_CRC)
#define MMC_RSP_R3  (MMC_RSP_PRESENT)

/* MMC Status Register (MMC_STAT) */

#define STAT_PRG_DONE           (1 << 13)
#define STAT_DATA_TRAN_DONE     (1 << 12)
#define STAT_END_CMD_RES        (1 << 11)
#define STAT_DATA_FIFO_EMPTY        (1 << 6)
#define STAT_CRC_RES_ERR        (1 << 5)
#define STAT_CRC_READ_ERROR     (1 << 4)
#define STAT_CRC_WRITE_ERROR_SHF    2
#define STAT_CRC_WRITE_ERROR_MASK   (0x3 << STAT_CRC_WRITE_ERROR_SHF)
#define STAT_CRC_WRITE_ERROR        (1 << STAT_CRC_WRITE_ERROR_SHF)
#define STAT_CRC_WRITE_ERROR_NOSTS  (2 << STAT_CRC_WRITE_ERROR_SHF)
#define STAT_TIME_OUT_RES       (1 << 1)
#define STAT_TIME_OUT_READ      (1 << 0)
#define MMC_RSP_R6  (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)


/* MMC Interrupts Status Register (MMC_IREG) */

#define IFLG_DATA_TRAN_DONE     (1 << 0)
#define IFLG_PRG_DONE           (1 << 1)
#define IFLG_END_CMD_RES        (1 << 2)


/* MMC Clock and Control Register (MMC_CTRL) */

#define CTRL_START_OP           (1 << 2)

#define CMDAT_BUSY          (1 << 6)

#define CMDAT_RESPONSE_SHF      0
#define CMDAT_RESPONSE_MASK     (0x7 << CMDAT_RESPONSE_SHF)
#define CMDAT_RESPONSE_NONE         (0x0 << CMDAT_RESPONSE_SHF) /* No response */
#define CMDAT_RESPONSE_R1       (0x1 << CMDAT_RESPONSE_SHF) /* Format R1 and R1b */
#define CMDAT_RESPONSE_R2       (0x2 << CMDAT_RESPONSE_SHF) /* Format R2 */
#define CMDAT_RESPONSE_R3       (0x3 << CMDAT_RESPONSE_SHF) /* Format R3 */
#define CMDAT_RESPONSE_R4       (0x4 << CMDAT_RESPONSE_SHF) /* Format R4 */
#define CMDAT_RESPONSE_R5       (0x5 << CMDAT_RESPONSE_SHF) /* Format R5 */
#define CMDAT_RESPONSE_R6       (0x6 << CMDAT_RESPONSE_SHF) /* Format R6 */
#define CMDAT_RESRONSE_R7       (0x7 << CMDAT_RESPONSE_SHF) /* Format R7 */

#define CMDAT_WRITE_READ        (1 << 4)
#define CMDAT_DATA_EN           (1 << 3)
#define CMDAT_BUS_WIDTH_SHF     9
#define CMDAT_BUS_WIDTH_MASK        (0x3 << CMDAT_BUS_WIDTH_SHF)
#define CMDAT_BUS_WIDTH_1BIT        (0x0 << CMDAT_BUS_WIDTH_SHF) /* 1-bit data bus */
#define CMDAT_BUS_WIDTH_4BIT        (0x2 << CMDAT_BUS_WIDTH_SHF) /* 4-bit data bus */
#define CMDAT_BUS_WIDTH_8BIT        (0x3 << CMDAT_BUS_WIDTH_SHF) /* 8-bit data bus */



#define ERROR_STAT      0x3f


#define MMC_DATA_READ       1
#define MMC_DATA_WRITE      2  /* We probably won't ever implement write.*/


#define MMC_CMD_STOP_TRANSMISSION 12
#define MMC_CMD_SET_BLOCKLEN    16
#define MMC_CMD_READ_SINGLE_BLOCK 17
#define MMC_CMD_READ_MULTIPLE_BLOCK 18

int mmc_read_data(u32 lba, u32 block_count, void *dest);
int mmc_send_status(void);

#endif // MMC_H_