#include "io.h"
#include "mem.h"
#include "common/types.h"

static struct memregion heap  __attribute__ ((section(".data")));

void boot_alloc_init(u32 start, u32 end)
{
    heap.start = start;
    heap.end = end;

    // TODO: we should probably check whether the heap region overlaps with any reserved regions here
    heap.offset = 0;
}

static void *alloc_region(u32 bytes, int align)
{
	u32 addr, new_ptr;
	void *ptr;

	addr = ALIGN(heap.start + heap.offset, align);
	new_ptr = addr + bytes;
	
    
	if (new_ptr > heap.end)
		return NULL;

    heap.offset = ALIGN(new_ptr - heap.start, 8);

	ptr = (void *)KSEG0ADDR(addr);
	return ptr;
}

void *malloc(u32 bytes)
{
	void *ptr = alloc_region(bytes, 1);
	if (!ptr)
		return ptr;

	return ptr;
}