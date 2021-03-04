#include "kernelUtil.h"

extern "C" void _start(BootInfo *bootInfo)
{
    // initialize kernel
    KernelInfo kernel_info = initialize_kernel(bootInfo);
    // get page table manager
    PageTableManager *pageTableManager = kernel_info.page_table_manager;

    BasicRenderer renderer = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_font);

    pageTableManager->map_memory((void*)0x600000000, (void*)0x80000);
    uint64_t* test = (uint64_t*)0x600000000;
    *test = 26;
    renderer.print(to_string(*test));

    return;
}

// KNOWN BUG (MAYBE RESOLVED): (with 256M of memory) when allocating 56944 pages, GetFreeRAM() returns stranges, waytoobig values (> 10000000000). OOM happens at 56952 pages.
// It seems like the kernel was causing this 4kb dicreapency.
