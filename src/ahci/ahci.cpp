#include "ahci.h"
#include "../io.h"
#include "../paging/PageTableManager.h"

namespace AHCI {

#define HBA_PORT_DEVICE_PRESENT 0x3
#define HBA_PORT_IPM_ACTIVE 0x1
#define SATA_SIGNATURE_ATAPI 0xeb140101
#define SATA_SIGNATURE_ATA 0x00000101
#define SATA_SIGNATURE_SEMB 0xc33c0101
#define SATA_SIGNATURE_PM 0x96690101

PortType check_port_type(HBAPort* port) {
    uint32_t sata_status = port->sata_status;
    uint8_t interface_power_management = (sata_status >> 8) & 0b111;
    uint8_t device_detection = sata_status & 0b111;

    if (device_detection != HBA_PORT_DEVICE_PRESENT) {
        return PortType::None;
    }

    if (interface_power_management != HBA_PORT_IPM_ACTIVE) {
        return PortType::None;
    }

    switch (port->signature) {
    case SATA_SIGNATURE_ATAPI:
        return PortType::SATAPI;
    case SATA_SIGNATURE_ATA:
        return PortType::SATA;
    case SATA_SIGNATURE_PM:
        return PortType::PM;
    case SATA_SIGNATURE_SEMB:
        return PortType::SEMB;
    default:
        return PortType::None;
    }
}

void AHCIDriver::probe_ports() {
    uint32_t ports_implemented = ABAR->ports_implemented;
    for (int i = 0; i < 32; i++) {
        if (ports_implemented & (1 << i)) {
            PortType port_type = check_port_type(&ABAR->ports[i]);
            if (port_type == PortType::SATA) {
                serial_println("SATA drive");
            } else if (port_type == PortType::SATAPI) {
                serial_println("SATAPI drive");
            } else if (port_type == PortType::PM) {
                serial_println("PM drive");
            } else if (port_type == PortType::SEMB) {
                serial_println("SEMB drive");
            } else {
                serial_println("No drive connected");
            }
        }
    }
}

AHCIDriver::AHCIDriver(PCI::PCIDeviceHeader* pci_base_address) {
    this->pci_base_address = pci_base_address;
    serial_println("AHCI Driver instance intiilized");

    ABAR = (HBAMemory*)((PCI::PCIHeader0*)pci_base_address)->BAR5;

    g_page_table_manager.map_memory(ABAR, ABAR);
    probe_ports();
}

AHCIDriver::~AHCIDriver() {
}
}
