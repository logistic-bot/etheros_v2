#include "kernelUtil.h"
#include "memory/heap.h"

KernelInfo kernel_info;
void prepare_memory(BootInfo* boot_info) {
    // calculate number of memory map entries
    uint64_t mMapEntries = boot_info->mMapSize / boot_info->mMapDescSize;

    // Initialize page frame allocaotr
    allocator = PageFrameAllocator();
    allocator.ReadEFIMemoryMap(boot_info->mMap, boot_info->mMapSize, boot_info->mMapDescSize);

    // Lock kernel pages
    uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;
    allocator.LockPages(&_KernelStart, kernelPages);

    // Initialize page table manager
    PageTable* PML4 = (PageTable*)allocator.request_page();
    memset(PML4, 0, 0x1000);

    g_page_table_manager = PageTableManager(PML4);

    // Custom memory map (?)
    for (uint64_t i = 0; i < get_memory_size(boot_info->mMap, mMapEntries, boot_info->mMapDescSize); i += 0x1000) {
        g_page_table_manager.map_memory((void*)i, (void*)i);
    }

    // lock pages for frambuffer
    uint64_t fbBase = (uint64_t)boot_info->framebuffer->BaseAddress;
    uint64_t fbSize = (uint64_t)boot_info->framebuffer->BufferSize + 0x1000;
    allocator.LockPages((void*)fbBase, fbSize / 0x1000 + 1);

    // map pages for framebuffer
    for (uint64_t i = fbBase; i < fbBase + fbSize; i += 4096) {
        g_page_table_manager.map_memory((void*)i, (void*)i);
    }

    // actually apply the memory map
    asm("mov %0, %%cr3"
        :
        : "r"(PML4));

    // prepare kernel info
    kernel_info.page_table_manager = &g_page_table_manager;
}

IDTR idtr;
void SetIDTGate(void* handler, uint8_t entry_offset, uint8_t type_attr, uint8_t selector) {
    IDTDescEntry* interupt = (IDTDescEntry*)(idtr.offset + entry_offset * sizeof(IDTDescEntry));
    interupt->set_offset((uint64_t)handler);
    interupt->type_attr = type_attr;
    interupt->selector = selector;
}

void prepare_interupts() {
    idtr.limit = 0x0fff;
    idtr.offset = (uint64_t)allocator.request_page();

    SetIDTGate((void*)PageFault_handler, 0xe, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)DoubleFault_handler, 0x8, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)GPFault_handler, 0xd, IDT_TA_InterruptGate, 0x8);
    SetIDTGate((void*)KeyboardInterupt_handler, 0x21, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)MouseInterupt_handler, 0x2c, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)PITInterupt_handler, 0x20, IDT_TA_InterruptGate, 0x08);

    asm("lidt %0"
        :
        : "m"(idtr));

    remap_pic();
}

void prepare_acpi(BootInfo* boot_info) {
    ACPI::SDTHeader* xsdt = (ACPI::SDTHeader*)(boot_info->rsdp->xsdt_address);

    ACPI::MCFGHeader* mcfg = (ACPI::MCFGHeader*)ACPI::find_table(xsdt, (char*)"MCFG");

    PCI::enumerate_PCI(mcfg);
}

BasicRenderer r = BasicRenderer(NULL, NULL);
KernelInfo initialize_kernel(BootInfo* boot_info) {
    serial_println("Initializing kernel...");
    // prepare global renderer
    serial_println("Setting up renderer");
    r = BasicRenderer(boot_info->framebuffer, boot_info->psf1_font);
    renderer = &r;

    // Load new GDT
    serial_println("Setting up GDT");
    GDTDescriptor gdtDescriptor;
    gdtDescriptor.size = sizeof(GDT) - 1;
    gdtDescriptor.offset = (uint64_t)&default_gdt;
    serial_println("Loading GDT");
    LoadGDT(&gdtDescriptor);

    serial_println("Preapring memory");
    prepare_memory(boot_info);

    serial_println("Clearing screen");
    renderer->clear();

    serial_println("Initializing heap");
    initialize_heap((void*)0x0000100000000000, 0x10);

    // prepare interupts
    serial_println("Preparing interupts");
    prepare_interupts();

    serial_println("Initializing mouse");
    init_ps2mouse();

    serial_println("Preparing acpi");
    prepare_acpi(boot_info);

    // set interupts bits
    serial_println("Setting interupt bits");
    outb(PIC1_DATA, 0b11111000);
    outb(PIC2_DATA, 0b11101111);

    // enable interupts
    serial_println("Enabeling interupts");
    asm("sti");

    // serial port
    serial_println("Initializing serial communications");
    initialize_serial();

    return kernel_info;
}
