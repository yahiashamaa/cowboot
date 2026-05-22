#ifndef MEM_H_
#define MEM_H_

#include "common/types.h"
#include "io.h"
#include "cache.h"

struct memregion {
    u32 start;
    u32 end;
    u32 offset;
};

void boot_alloc_init(u32 start, u32 end);
void *malloc(u32 bytes);

#endif // MEM_H_