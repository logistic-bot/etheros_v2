#include "heap.h"
#include "../paging/PageFrameAllocator.h"
#include "../paging/PageTableManager.h"

void* heap_start;
void* heap_end;
HeapSegmentHeader* last_header;

void initialize_heap(void* heap_address, size_t page_count) {
    void* pos = heap_address;
    for (size_t i = 0; i < page_count; i++) {
        g_page_table_manager.map_memory(pos, allocator.request_page());
        pos = (void*)((size_t)pos + 0x1000);
    }

    size_t heap_length = page_count * 0x1000;

    heap_start = heap_address;
    heap_end = (void*)((size_t)heap_start + heap_length);
    HeapSegmentHeader* start_segment = (HeapSegmentHeader*)heap_address;
    start_segment->length = heap_length - sizeof(HeapSegmentHeader);
    start_segment->last = NULL;
    start_segment->next = NULL;
    start_segment->free = true;
    last_header = start_segment;
}

void* malloc(size_t size) {
    if (size % 0x10 > 0) {
        // not a multiple of 128
        size -= (size % 0x10);
        size += 0x10;
    }

    if (size == 0) {
        return NULL;
    }

    HeapSegmentHeader* current_segment = (HeapSegmentHeader*)heap_start;
    while (true) {
        if (current_segment->free) {
            if (current_segment->length > size) {
                current_segment->split(size);
                current_segment->free = false;
                return (void*)((uint64_t)current_segment + sizeof(HeapSegmentHeader));
            }
            if (current_segment->length == size) {
                current_segment->free = false;
                return (void*)((uint64_t)current_segment + sizeof(HeapSegmentHeader));
            }
        }
        if (current_segment->next == NULL)
            break;
        current_segment = current_segment->next;
    }
    expand_heap(size);
    return malloc(size);
}

HeapSegmentHeader* HeapSegmentHeader::split(size_t split_length) {
    if (split_length < 0x10) {
        return NULL;
    }
    int64_t split_segment_length = length - split_length - (sizeof(HeapSegmentHeader));
    if (split_segment_length < 0x10)
        return NULL;

    HeapSegmentHeader* new_split_header = (HeapSegmentHeader*)((size_t)this + split_length + sizeof(HeapSegmentHeader));
    next->last = new_split_header;
    new_split_header->next = next;
    next = new_split_header;
    new_split_header->last = this;
    new_split_header->length = split_segment_length;
    new_split_header->free = free;
    length = split_length;

    if (last_header == this)
        last_header = new_split_header;
    return new_split_header;
}

void expand_heap(size_t length) {
}
