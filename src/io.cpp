#include "io.h"
#include "panic.h"

void outb(uint16_t port, uint8_t value) {
    asm volatile("outb %0, %1"
                 :
                 : "a"(value), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0"
                 : "=a"(ret)
                 : "Nd"(port));
    return ret;
}

void io_wait() {
    asm volatile("outb %%al, $0x80"
                 :
                 : "a"(0));
}

void initialize_serial() {
    outb(SERIAL_PORT + 1, 0x00); // Disable all interrupts
    outb(SERIAL_PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
    outb(SERIAL_PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    outb(SERIAL_PORT + 1, 0x00); //                  (hi byte)
    outb(SERIAL_PORT + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(SERIAL_PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    outb(SERIAL_PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
    outb(SERIAL_PORT + 4, 0x1E); // Set in loopback mode, test the serial chip
    outb(SERIAL_PORT + 0, 0xAE); // Test serial chip (send byte 0xAE and check if serial returns same byte)
    // Check if serial is faulty (i.e: not same byte as sent)
    if (inb(SERIAL_PORT + 0) != 0xAE) {
        panic("Serial port is faulty");
    }

    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb(SERIAL_PORT + 4, 0x0F);
}

int serial_recived() {
    return inb(SERIAL_PORT + 5) & 1;
}

char read_serial() {
    while (serial_recived() == 0)
        ;

    return inb(SERIAL_PORT);
}

int is_transmit_empty() {
    return inb(SERIAL_PORT + 5) & 0x20;
}
void write_serial(char c) {
    while (is_transmit_empty() == 0)
        ;
    outb(SERIAL_PORT, c);
}
