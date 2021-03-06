#pragma once

#include "paging.h"

class PageTableManager {
public:
    PageTableManager(PageTable* PML4Address);
    PageTable* PML4;

    void map_memory(void* virtualMemory, void* physicalMemory);
};

extern PageTableManager g_page_table_manager;
