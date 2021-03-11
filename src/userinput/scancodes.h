#pragma once
#include <stdint.h>

namespace AZERTYKeyboard {
#define LeftShift 0x2a
#define RightShift 0x36
#define Enter 0x1c
#define Backspace 0x0e
#define Spacebar 0x39
#define Backspace 0x0e

extern const char ASCIITable[];
char translate(uint8_t scancode, bool uppercase);
}
