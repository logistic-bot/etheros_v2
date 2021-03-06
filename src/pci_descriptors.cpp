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

const char* get_device_name(uint16_t vendor_id, uint16_t device_id) {
    switch (vendor_id) {
    case 0x1234: // QEMU
        switch (device_id) {
        case 0x1111:
            return "VGA compatible controller";
        default:
            return to_hstring(device_id);
        }
    case 0x8086: // Intel
        switch (device_id) {
        case 0x29c0:
            return "Express DRAM Controller";
        case 0x2918:
            return "LPC Interface Controller";
        case 0x2922:
            return "6 port SATA Controller [AHCI mode]";
        case 0x2930:
            return "SMBus Controller";
        default:
            return to_hstring(device_id);
        }
    default:
        return to_hstring(device_id);
    }
}
}
