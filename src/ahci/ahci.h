#pragma once
#include "../pci.h"
#include <stdint.h>

namespace AHCI {
enum PortType {
    None = 0,
    SATA = 1,
    SEMB = 2,
    PM = 3,
    SATAPI = 4,
};

struct HBAPort {
    uint32_t command_list_base;
    uint32_t command_list_base_upper;
    uint32_t fis_base_address;
    uint32_t fis_base_address_upper;
    uint32_t interrupt_status;
    uint32_t interrupt_enabled;
    uint32_t command_and_status;
    uint32_t rsv0;
    uint32_t task_file_data;
    uint32_t signature;
    uint32_t sata_status;
    uint32_t sata_control;
    uint32_t sata_error;
    uint32_t sata_active;
    uint32_t command_issue;
    uint32_t sata_notification;
    uint32_t fis_switch_control;
    uint32_t rsv1[11];
    uint32_t vedor[4];
};

struct HBAMemory {
    uint32_t host_capability;
    uint32_t global_host_control;
    uint32_t interrupt_status;
    uint32_t ports_implemented;
    uint32_t version;
    uint32_t ccc_control;
    uint32_t ccc_ports;
    uint32_t enclosure_managment_location;
    uint32_t enclosure_managment_control;
    uint32_t host_capabilities_extended;
    uint32_t bios_handoff_control_and_status;
    uint8_t rsv0[0x74];
    uint8_t vendor[0x60];
    HBAPort ports[1];
};

class AHCIDriver {
public:
    AHCIDriver(PCI::PCIDeviceHeader* pci_base_address);
    ~AHCIDriver();

    PCI::PCIDeviceHeader* pci_base_address;
    HBAMemory* ABAR;

    void probe_ports();
};
}
