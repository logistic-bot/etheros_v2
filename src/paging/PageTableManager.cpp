#include "PageTableManager.h"

#include "PageFrameAllocator.h"
#include "PageMapIndexer.h"

#include "../memory.h"

#include <stdint.h>

PageTableManager g_page_table_manager = NULL;

PageTableManager::PageTableManager(PageTable* PML4Address) {
    this->PML4 = PML4Address;
}

void PageTableManager::map_memory(void* virtualMemory, void* physicalMemory) {
    PageMapIndexer indexer = PageMapIndexer((uint64_t)virtualMemory);
    PageDirectoryEntry PDE;

    PDE = PML4->entries[indexer.PDP_index];
    PageTable* PDP;
    if (!PDE.get_flag(PT_Flag::present)) {
        PDP = (PageTable*)allocator.request_page();
        memset(PDP, 0, 0x1000);
        PDE.set_address((uint64_t)PDP >> 12);
        PDE.set_flag(PT_Flag::present, true);
        PDE.set_flag(PT_Flag::read_write, true);
        PML4->entries[indexer.PDP_index] = PDE;
    } else {
        PDP = (PageTable*)((uint64_t)PDE.get_address() << 12);
    }

    PDE = PDP->entries[indexer.PD_index];
    PageTable* PD;
    if (!PDE.get_flag(PT_Flag::present)) {
        PD = (PageTable*)allocator.request_page();
        memset(PD, 0, 0x1000);
        PDE.set_address((uint64_t)PD >> 12);
        PDE.set_flag(PT_Flag::present, true);
        PDE.set_flag(PT_Flag::read_write, true);
        PDP->entries[indexer.PD_index] = PDE;
    } else {
        PD = (PageTable*)((uint64_t)PDE.get_address() << 12);
    }

    PDE = PD->entries[indexer.PT_index];
    PageTable* PT;
    if (!PDE.get_flag(PT_Flag::present)) {
        PT = (PageTable*)allocator.request_page();
        memset(PT, 0, 0x1000);
        PDE.set_address((uint64_t)PT >> 12);
        PDE.set_flag(PT_Flag::present, true);
        PDE.set_flag(PT_Flag::read_write, true);
        PD->entries[indexer.PT_index] = PDE;
    } else {
        PT = (PageTable*)((uint64_t)PDE.get_address() << 12);
    }

    PDE = PT->entries[indexer.P_index];
    PDE.set_address((uint64_t)physicalMemory >> 12);
    PDE.set_flag(PT_Flag::present, true);
    PDE.set_flag(PT_Flag::read_write, true);

    PT->entries[indexer.P_index] = PDE;
}
