#include "interupts.h"
#include "../panic.h"
#include "../io.h"
#include "../cstr.h"
#include "../keyboard/keyboard.h"

__attribute__((interrupt)) void PageFault_handler(struct interrupt_frame* frame) {
    panic("Page fault detected");
    while (true);
}

__attribute__((interrupt)) void GPFault_handler(struct interrupt_frame* frame) {
    panic("General Protection fault detected");
    while (true);
}

__attribute__((interrupt)) void DoubleFault_handler(struct interrupt_frame* frame) {
    panic("Double fault detected");
    while (true);
}

__attribute__((interrupt)) void KeyboardInterupt_handler(struct interrupt_frame* frame) {
    uint8_t scancode = inb(0x60);
    HandleKeyboard(scancode);
//    renderer->print(to_hstring(scancode));
//    renderer->next();
    pic_end_master();
}

void pic_end_master() {
    outb(PIC1_COMMAND, PIC_EOI);
}

void pic_end_slave() {
    outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}

void remap_pic() {
    uint8_t a1, a2;

    a1 = inb(PIC1_DATA);
    io_wait();
    a2 = inb(PIC2_DATA);
    io_wait();

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();

    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, a1);
    io_wait();
    outb(PIC2_DATA, a2);
    io_wait();
}
