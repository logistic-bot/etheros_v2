#include "PageFrameAllocator.h"

uint64_t freeMemory;
uint64_t reservedMemory;
uint64_t usedMemory;

bool Initialized = false;
PageFrameAllocator allocator;

void PageFrameAllocator::ReadEFIMemoryMap(EFI_MEMORY_DESCRIPTOR* mMap, size_t mMapSize, size_t mMapDescSize) {
    if (Initialized)
        return;
    Initialized = true;

    uint64_t mMapEntries = mMapSize / mMapDescSize;

    void* largesFreeMemSegment = NULL;
    size_t largestFreeMemSegmentSize = 0;

    for (int i = 0; i < mMapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + (i * mMapDescSize));
        if (desc->type == 7) { // Type = EfiConventionalMemory
            if (desc->numPages * 4096 > largestFreeMemSegmentSize) {
                largesFreeMemSegment = desc->physAddr;
                largestFreeMemSegmentSize = desc->numPages * 4096;
            }
        }
    }

    uint64_t memorySize = get_memory_size(mMap, mMapEntries, mMapDescSize);
    freeMemory = memorySize;
    uint64_t bitmapSize = memorySize / 4096 / 8 + 1;

    InitBitmap(bitmapSize, largesFreeMemSegment);

    // lock pages of bitmap
    ReservePages(0, memorySize / 4096 + 1);
    // reserve pages of unusable memory
    for (int i = 0; i < mMapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + (i * mMapDescSize));
        if (desc->type == 7) {
            // EFI conventional memory
            UnreservePages(desc->physAddr, desc->numPages);
        }
    }
    ReservePages(0, 0x100); // reserve between 0 and 0x1000000
    LockPages(page_bitmap.buffer, page_bitmap.size / 4096 + 1);
}

void PageFrameAllocator::InitBitmap(size_t bitmapSize, void* bufferAddress) {
    page_bitmap.size = bitmapSize;
    page_bitmap.buffer = (uint8_t*)bufferAddress;
    for (int i = 0; i < bitmapSize; i++) {
        *(uint8_t*)(page_bitmap.buffer + i) = 0;
    }
}

//void *PageFrameAllocator::request_page() // THIS IS UNOPTIMIZED, I ONLY KEEP IT FOR DEMONSTRATION
//{
//    for (uint64_t index = 0; index < page_bitmap.size * 8; index++)
//    {
//        if (page_bitmap[index] == true)
//            continue;
//        LockPage((void *)(index * 4096));
//        return (void *)(index * 4096);
//    }

//    return NULL; // TODO: page frame swap to file
//}

uint64_t page_bitmap_index = 0;
void* PageFrameAllocator::request_page() {
    for (; page_bitmap_index < page_bitmap.size * 8; page_bitmap_index++) {
        if (page_bitmap[page_bitmap_index] == true)
            continue;
        LockPage((void*)(page_bitmap_index * 4096));
        return (void*)(page_bitmap_index * 4096);
    }

    return NULL; // TODO: page frame swap to file
}

void PageFrameAllocator::FreePage(void* address) {
    uint64_t index = (uint64_t)address / 4096;
    if (page_bitmap[index] == false)
        return;
    if (page_bitmap.set(index, false)) {
        freeMemory += 4096;
        usedMemory -= 4096;
        if (page_bitmap_index > index) {
            page_bitmap_index = index;
        }
    }
}

void PageFrameAllocator::FreePages(void* address, uint64_t page_count) {
    for (int i = 0; i < page_count; i++) {
        FreePage((void*)((uint64_t)address + (i * 4096)));
    }
}

void PageFrameAllocator::LockPage(void* address) {
    uint64_t index = (uint64_t)address / 4096;
    if (page_bitmap[index] == true)
        return;
    if (page_bitmap.set(index, true)) {
        freeMemory -= 4096;
        usedMemory += 4096;
    }
}

void PageFrameAllocator::LockPages(void* address, uint64_t page_count) {
    for (int i = 0; i < page_count; i++) {
        LockPage((void*)((uint64_t)address + (i * 4096)));
    }
}

void PageFrameAllocator::ReservePage(void* address) {
    uint64_t index = (uint64_t)address / 4096;
    if (page_bitmap[index] == true)
        return;
    if (page_bitmap.set(index, true)) {
        freeMemory -= 4096;
        reservedMemory += 4096;
    }
}

void PageFrameAllocator::ReservePages(void* address, uint64_t page_count) {
    for (int i = 0; i < page_count; i++) {
        ReservePage((void*)((uint64_t)address + (i * 4096)));
    }
}

void PageFrameAllocator::UnreservePage(void* address) {
    uint64_t index = (uint64_t)address / 4096;
    if (page_bitmap[index] == false)
        return;
    if (page_bitmap.set(index, false)) {
        freeMemory += 4096;
        reservedMemory -= 4096;
        if (page_bitmap_index > index) {
            page_bitmap_index = index;
        }
    }
}

void PageFrameAllocator::UnreservePages(void* address, uint64_t page_count) {
    for (int i = 0; i < page_count; i++) {
        UnreservePage((void*)((uint64_t)address + (i * 4096)));
    }
}

uint64_t PageFrameAllocator::GetFreeRAM() {
    return freeMemory;
}
uint64_t PageFrameAllocator::GetUsedRAM() {
    return usedMemory;
}
uint64_t PageFrameAllocator::GetReservedRAM() {
    return reservedMemory;
}
