#pragma once

int board_early_init(void);
int board_late_init(void);

#ifdef DOWNSTREAM_BOOT
void embed_boot(void);
#endif

void continue_boot(void);


#if defined(BOARD_qogir)
#include "qogir.h"
#endif