#include "scancodes.h"

namespace AZERTYKeyboard {
// Some of this is probably incorecct
const char ASCIITable[] = {
    0, 0, '&', '~',
    '"', '\'', '(', '-',
    '`', '_', '^', '@',
    ')', '=', 0, 0,
    'a', 'z', 'e', 'r',
    't', 'y', 'u', 'i',
    'o', 'p', '[', ']',
    0, 0, 'q', 's',
    'd', 'f', 'g', 'h',
    'j', 'k', 'l', 'm',
    '\'', '`', 0, '\\',
    'w', 'x', 'c', 'v',
    'b', 'n', ',', ';',
    '.', '!', 0, '*',
    0, ' '
};

char translate(uint8_t scancode, bool uppercase) {
    if (scancode > 58) {
        return 0;
    }

    if (scancode == Spacebar) {
        return ' ';
    }

    if (uppercase) {
        if (scancode == 0x33) // shift + ; (.)
            return '.';
        return ASCIITable[scancode] - 32;
    }
    return ASCIITable[scancode];
}
}
