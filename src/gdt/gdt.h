#pragma once

#include <stdint.h>

struct GDTDescriptor
{
    uint16_t size;
    uint64_t offset;
} __attribute__((packed));

struct GDTEntry
{
    uint16_t limit0;
    uint16_t base0;
    uint8_t base1;
    uint8_t access_byte;
    uint8_t limit1_flags;
    uint8_t base2;
} __attribute__((packed));

struct GDT
{
    GDTEntry Null;
    GDTEntry kernel_code;
    GDTEntry kernel_data;
    GDTEntry user_null;
    GDTEntry user_code;
    GDTEntry user_data;
} __attribute__((packed)) __attribute__((aligned(0x1000)));

extern GDT default_gdt;

extern "C" void LoadGDT(GDTDescriptor* gdtDescriptor);
