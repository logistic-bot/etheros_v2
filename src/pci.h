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

struct PCIHeader0 {
    PCIDeviceHeader header;
    uint32_t BAR0;
    uint32_t BAR1;
    uint32_t BAR2;
    uint32_t BAR3;
    uint32_t BAR4;
    uint32_t BAR5;
    uint32_t card_bus_cis_ptr;
    uint16_t subsystem_vendor_id;
    uint16_t subsystem_id;
    uint32_t expansion_rom_base_addr;
    uint8_t capabilities_ptr;
    uint8_t rsv0;
    uint16_t rsv1;
    uint32_t rsv2;
    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint8_t min_grant;
    uint8_t max_latency;
};

void enumerate_PCI(ACPI::MCFGHeader* mcfg);

extern const char* device_classes[];

const char* get_vendor_name(uint16_t vendor_id);
const char* get_device_name(uint16_t vendor_id, uint16_t device_id);
const char* get_subclass_name(uint8_t class_id, uint8_t subclass_id);
const char* get_program_interface_name(uint8_t class_id, uint8_t subclass_id, uint8_t program_interface);
}
