#include "kernelUtil.h"

extern "C" void _start(BootInfo *bootInfo)
{
    BasicRenderer renderer = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_font);
    allocator = PageFrameAllocator();
    allocator.ReadEFIMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);
    uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescSize;

    uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;
    allocator.LockPages(&_KernelStart, kernelPages);

    PageTable* PML4 = (PageTable*)allocator.request_page();
    memset(PML4, 0, 0x1000);

    PageTableManager pageTableManager = PageTableManager(PML4);

    for (uint64_t i = 0; i < get_memory_size(bootInfo->mMap, mMapEntries, bootInfo->mMapDescSize); i+=0x1000) {
        pageTableManager.map_memory((void*)i, (void*)i);
    }

    uint64_t fbBase = (uint64_t)bootInfo->framebuffer->BaseAddress;
    uint64_t fbSize = (uint64_t)bootInfo->framebuffer->BufferSize + 0x1000;
    allocator.LockPages((void*)fbBase, fbSize / 0x1000 + 1);

    for (uint64_t i = fbBase; i < fbBase + fbSize; i+=4096) {
        pageTableManager.map_memory((void*)i, (void*)i);
    }

    asm ("mov %0, %%cr3" : : "r" (PML4));

    // Cleak screen
    memset(bootInfo->framebuffer->BaseAddress, 0, bootInfo->framebuffer->BufferSize);

    pageTableManager.map_memory((void*)0x600000000, (void*)0x80000);
    uint64_t* test = (uint64_t*)0x600000000;
    *test = 26;
    renderer.print(to_string(*test));

    return;
}

// KNOWN BUG (MAYBE RESOLVED): (with 256M of memory) when allocating 56944 pages, GetFreeRAM() returns stranges, waytoobig values (> 10000000000). OOM happens at 56952 pages.
// It seems like the kernel was causing this 4kb dicreapency.
