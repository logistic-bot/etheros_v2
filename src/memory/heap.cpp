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
