#pragma once

#include <stdint.h>
#include "efiMemory.h"

uint64_t get_memory_size(EFI_MEMORY_DESCRIPTOR *mMap, uint64_t mMapEntries, uint64_t mMapDescSize);
