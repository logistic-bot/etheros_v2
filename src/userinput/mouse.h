#pragma once
#include "../BasicRenderer.h"
#include "../io.h"
#include "../math.h"
#include <stdint.h>

#define PS2LEFTBUTTON 0b00000001
#define PS2RIGHTBUTTON 0b00000010
#define PS2MIDDLEBUTTON 0b00000100

#define PS2XSIGN 0b00010000
#define PS2YSIGN 0b00100000
#define PS2XOVERFLOW 0b01000000
#define PS2YOVERFLOW 0b10000000

extern uint8_t mouse_pointer[];

void init_ps2mouse();
void handle_ps2_mouse(uint8_t data);
void process_mouse_packets();

extern Point mouse_position;
