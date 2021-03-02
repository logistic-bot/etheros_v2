#include <stddef.h>
#include "BasicRenderer.h"
#include "cstr.h"
#include "efiMemory.h"
#include "memory.h"
#include "Bitmap.h"
#include "PageFrameAllocator.h"

struct BootInfo
{
    Framebuffer *framebuffer;
    PSF1_FONT *psf1_font;
    EFI_MEMORY_DESCRIPTOR *mMap;
    uint64_t mMapSize;
    uint64_t mMapDescSize;
};

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

extern "C" void _start(BootInfo *bootInfo)
{

    BasicRenderer renderer = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_font);
    PageFrameAllocator allocator;
    allocator.ReadEFIMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);

    uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescSize;

    uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;
    allocator.LockPages(&_KernelStart, kernelPages);

    renderer.cursorPosition = {0, 0};
    renderer.print("kernel start: ");
    renderer.print(to_hstring((uint64_t)&_KernelStart));
    renderer.print(" kernel size: ");
    renderer.print(to_string((uint64_t)kernelPages));

    renderer.cursorPosition = {0, 16};
    renderer.print("free ");
    renderer.print(to_string(allocator.GetFreeRAM() / 1024));
    renderer.print(" KB");
    renderer.cursorPosition = {0, renderer.cursorPosition.y + 16};
    renderer.print("used ");
    renderer.print(to_string(allocator.GetUsedRAM() / 1024));
    renderer.print(" KB");
    renderer.cursorPosition = {0, renderer.cursorPosition.y + 16};
    renderer.print("reserved ");
    renderer.print(to_string(allocator.GetReservedRAM() / 1024));
    renderer.print(" KB");
    renderer.cursorPosition = {0, renderer.cursorPosition.y + 16};

    for (int i = 0; i < 20; i++)
    {
        void *address = allocator.RequestPage();
        renderer.print(to_hstring((uint64_t)address));
        renderer.cursorPosition = {0, renderer.cursorPosition.y + 16};
    }

    renderer.print("free ");
    renderer.print(to_string(allocator.GetFreeRAM() / 1024));
    renderer.print(" KB");
    renderer.cursorPosition = {0, renderer.cursorPosition.y + 16};
    renderer.print("used ");
    renderer.print(to_string(allocator.GetUsedRAM() / 1024));
    renderer.print(" KB");
    renderer.cursorPosition = {0, renderer.cursorPosition.y + 16};
    renderer.print("reserved ");
    renderer.print(to_string(allocator.GetReservedRAM() / 1024));
    renderer.print(" KB");
    renderer.cursorPosition = {0, renderer.cursorPosition.y + 16};

    // renderer.print(to_string(get_memory_size(bootInfo->mMap, mMapEntries, bootInfo->mMapDescSize)));

    // for (int i; i < mMapEntries; i++)
    // {
    //     EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR *)((uint64_t)bootInfo->mMap + (i * bootInfo->mMapDescSize));
    //     renderer.color = 0xffffffff;
    //     renderer.print(EFI_MEMORY_TYPE_STRINGS[desc->type]);
    //     renderer.color = 0xffff00ff;
    //     renderer.print(" ");
    //     renderer.print(to_string(desc->numPages * 4096 / 1024));
    //     renderer.print(" KB");
    //     renderer.cursorPosition = {0, renderer.cursorPosition.y + 16};
    // }

    return;
}

// KNOWN BUG (MAYBE RESOLVED): (with 256M of memory) when allocating 56944 pages, GetFreeRAM() returns stranges, waytoobig values (> 10000000000). OOM happens at 56952 pages.
// It seems like the kernel was causing this 4kb dicreapency.
