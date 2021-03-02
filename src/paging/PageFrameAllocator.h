#pragma once
#include "efiMemory.h"
#include <stdint.h>
#include "Bitmap.h"
#include "memory.h"

class PageFrameAllocator
{
public:
    void ReadEFIMemoryMap(EFI_MEMORY_DESCRIPTOR *mMap, size_t mMapSize, size_t mMapDescSize);
    Bitmap page_bitmap;
    void FreePage(void *address);
    void LockPage(void *address);

    void FreePages(void *address, uint64_t page_count);
    void LockPages(void *address, uint64_t page_count);

    uint64_t GetFreeRAM();
    uint64_t GetUsedRAM();
    uint64_t GetReservedRAM();

    void *RequestPage();

private:
    void InitBitmap(size_t bitmapSize, void *bufferAddress);
    void ReservePage(void *address);
    void UnreservePage(void *address);

    void ReservePages(void *address, uint64_t page_count);
    void UnreservePages(void *address, uint64_t page_count);
};
