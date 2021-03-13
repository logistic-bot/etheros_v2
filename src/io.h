#pragma once
#include <stdint.h>

#define SERIAL_PORT 0x3f8

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
void io_wait();

void initialize_serial();
int serial_received();
char read_serial();

int is_transmit_empty();
void write_serial(char c);
void serial_print(const char* str);
void serial_println(const char* str);

void serial_print(const char chr);
void serial_println(const char chr);
