#ifndef BOOTMENU_H_
#define BOOTMENU_H_

#include "common/types.h"

typedef struct bootmenu_entry {
	char *title;			/* title of entry */
	void (*command)(void);
} bootmenu_entry;

extern const bootmenu_entry board_boot_options[];
extern const int board_boot_options_count;

void draw_menu(int selection);
void draw_logo(int y, u32 color);
void cycle_menu(int selection, int key_gpio);

#endif // BOOTMENU_H_