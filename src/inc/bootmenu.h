#ifndef BOOTMENU_H_
#define BOOTMENU_H_

#include "types.h"

void draw_menu(int selection);
void draw_logo(int y, u32 color);
void cycle_menu(int selection, int key_gpio);

#endif // BOOTMENU_H_