#pragma once
#include "../BasicRenderer.h"
#include "../cstr.h"
#include "../io.h"
#include "../panic.h"
#include "../userinput/keyboard.h"
#include "../userinput/mouse.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

struct interrupt_frame;
__attribute__((interrupt)) void PageFault_handler(interrupt_frame* frame);
__attribute__((interrupt)) void DoubleFault_handler(interrupt_frame* frame);
__attribute__((interrupt)) void GPFault_handler(interrupt_frame* frame);
__attribute__((interrupt)) void KeyboardInterupt_handler(interrupt_frame* frame);
__attribute__((interrupt)) void MouseInterupt_handler(interrupt_frame* frame);
__attribute__((interrupt)) void PITInterupt_handler(interrupt_frame* frame);

void remap_pic();
void pic_end_master();
void pic_end_slave();
