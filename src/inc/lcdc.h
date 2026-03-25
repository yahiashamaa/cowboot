#ifndef LCDC_H_
#define LCDC_H_

#include "types.h"

struct framedesc {
    u32 next;
    u32 databuf;
    u32 id;
    u32 cmd;
    u32 offsize;
    u32 page_width;
    u32 cpos;
    u32 desc_size;
};

void early_lcdc_init(void);

#endif // LCDC_H_