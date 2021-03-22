#pragma once
#include <stdint.h>

namespace AZERTYKeyboard {
#define LeftShift 0x2a
#define RightShift 0x36
#define Enter 0x1c
#define Backspace 0x0e
#define Spacebar 0x39

#define Extended 0xe0
#define KeypadEnter 0x1c
#define Super 0x5b
#define AnyAlt 0x38     // When preceded with Extended, is altGr
#define AnyControl 0x1d // When preceded withextended, is right

extern const char ASCIITable[];
char translate(uint8_t scancode, bool uppercase);
}
