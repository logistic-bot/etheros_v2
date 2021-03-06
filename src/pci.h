#pragma once
#include "BasicRenderer.h"
#include "acpi.h"
#include "cstr.h"
#include "paging/PageTableManager.h"
#include <stdint.h>

namespace PCI {
struct PCIDeviceHeader {
    uint16_t vendor_id;
    uint16_t device_id;
    uint16_t command;
    uint16_t status;
    uint8_t revision_id;
    uint8_t program_interface;
    uint8_t subclass_id;
    uint8_t class_id;
    uint8_t cache_line_size;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t BIST;
};

void enumerate_PCI(ACPI::MCFGHeader* mcfg);

extern const char* device_classes[];

const char* get_vendor_name(uint16_t vendor_id);
}
