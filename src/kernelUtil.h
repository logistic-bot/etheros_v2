#pragma once

#include "BasicRenderer.h"
#include "Bitmap.h"
#include "cstr.h"
#include "efiMemory.h"
#include "gdt/gdt.h"
#include "interupts/IDT.h"
#include "interupts/interupts.h"
#include "io.h"
#include "memory.h"
#include "paging/PageFrameAllocator.h"
#include "paging/PageMapIndexer.h"
#include "paging/PageTableManager.h"
#include "paging/paging.h"
#include "userinput/mouse.h"
#include <stddef.h>

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

struct KernelInfo {
    PageTableManager* page_table_manager;
};

KernelInfo initialize_kernel(BootInfo* boot_info);
