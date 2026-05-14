#include "common/types.h"
#include "io.h"
#include "common/fb.h"

// TODO: add support for format specifiers :<

int video_printf(const char *fmt, ...)
{
    draw_string(cur_x, cur_y, fmt, 0xFFFFFFFF, 0x00000000);
    cur_x = 50;
    cur_y += 16;
    return 0;
}
