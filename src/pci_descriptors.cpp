#include "cstr.h"
#include <stdint.h>

namespace PCI {
const char* device_classes[] {
    "Unclassified",
    "Mass Storage Controller",
    "Network Controller",
    "Display Controller",
    "Multimedia Controller",
    "Memory Controller",
    "Bridge Device",
    "Simple Communication Controller",
    "Base System Peripheral",
    "Input Device Controller",
    "Docking Station",
    "Processor",
    "Serial Bus Controller",
    "Wireless Controller",
    "Intelligent Controller",
    "Satellite Communication Controller",
    "Encryption Controller",
    "Signal Processing Controller",
    "Processing Accelerator",
    "Non Essential Instrumentation"
};

const char* get_vendor_name(uint16_t vendor_id) {
    switch (vendor_id) {
    case 0x1234:
        return "QEMU";
    case 0x8086:
        return "Intel Corporation";
    case 0x1022:
        return "AMD";
    case 0x1dde:
        return "NVIDIA Corporation";
    default:
        return to_hstring(vendor_id);
    }
}
}
