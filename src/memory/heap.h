#pragma once
#include <stddef.h>
#include <stdint.h>

struct HeapSegmentHeader {
    size_t length;
    HeapSegmentHeader* next;
    HeapSegmentHeader* last;
    bool free;
    void combine_forward();
    void combine_backward();
    HeapSegmentHeader* split(size_t split_length);
};

void initialize_heap(void* heap_address, size_t page_count);

void* malloc(size_t size);
void free(void* address);

void expand_heap(size_t length);

inline void* operator new(size_t size) { return malloc(size); }
inline void* operator new[](size_t size) { return malloc(size); }

inline void operator delete(void* p) { free(p); };
