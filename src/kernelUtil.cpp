#include "kernelUtil.h"

KernelInfo kernel_info;
PageTableManager pageTableManager = NULL;
void prepare_memory(BootInfo* boot_info) {
    // calculate number of memory map entries
    uint64_t mMapEntries = boot_info->mMapSize / boot_info->mMapDescSize;

    // Initialize page frame allocaotr
    allocator = PageFrameAllocator();
    allocator.ReadEFIMemoryMap(boot_info->mMap, boot_info->mMapSize, boot_info->mMapDescSize);

    // Lock kernel pages
    uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;
    allocator.LockPages(&_KernelStart, kernelPages);

    // Initialize page table manager
    PageTable* PML4 = (PageTable*)allocator.request_page();
    memset(PML4, 0, 0x1000);

    pageTableManager = PageTableManager(PML4);

    // Custom memory map (?)
    for (uint64_t i = 0; i < get_memory_size(boot_info->mMap, mMapEntries, boot_info->mMapDescSize); i+=0x1000) {
        pageTableManager.map_memory((void*)i, (void*)i);
    }

    // lock pages for frambuffer
    uint64_t fbBase = (uint64_t)boot_info->framebuffer->BaseAddress;
    uint64_t fbSize = (uint64_t)boot_info->framebuffer->BufferSize + 0x1000;
    allocator.LockPages((void*)fbBase, fbSize / 0x1000 + 1);

    // map pages for framebuffer
    for (uint64_t i = fbBase; i < fbBase + fbSize; i+=4096) {
        pageTableManager.map_memory((void*)i, (void*)i);
    }

    // actually apply the memory map
    asm ("mov %0, %%cr3" : : "r" (PML4));

    // prepare kernel info
    kernel_info.page_table_manager = &pageTableManager;
}

KernelInfo initialize_kernel(BootInfo* boot_info) {
    prepare_memory(boot_info);

    // Cleak screen
    memset(boot_info->framebuffer->BaseAddress, 0, boot_info->framebuffer->BufferSize);

    return kernel_info;
}
