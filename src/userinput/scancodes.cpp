#include "scancodes.h"
#include "../BasicRenderer.h"

namespace AZERTYKeyboard {
// Some of this is probably incorecct
const char ASCIITable[] = {
    0, 0, '&', '~',
    '"', '\'', '(', '-',
    '`', '_', '^', '@',
    ')', '=', 0, 0,
    'a', 'z', 'e', 'r',
    't', 'y', 'u', 'i',
    'o', 'p', '^', '$',
    0, 0, 'q', 's',
    'd', 'f', 'g', 'h',
    'j', 'k', 'l', 'm',
    '\'', '`', 0, '*',
    'w', 'x', 'c', 'v',
    'b', 'n', ',', ';',
    ':', '!', 0, '*',
    0, ' '
};

char translate(uint8_t scancode, bool uppercase) {

    if (scancode > 58) {
        if (scancode == 0x56) { // <
            if (uppercase)
                return '>';
            else
                return '<';
        }
        return 0;
    }

    if (uppercase) {
        // numbers
        if (scancode == 0x02) // shift + & (1)
            return '1';
        else if (scancode == 0x03) // shift + ~ (1)
            return '2';
        else if (scancode == 0x04) // shift + " (1)
            return '3';
        else if (scancode == 0x05) // shift + ' (1)
            return '4';
        else if (scancode == 0x06) // shift + ( (1)
            return '5';
        else if (scancode == 0x07) // shift + - (1)
            return '6';
        else if (scancode == 0x08) // shift + ` (1)
            return '7';
        else if (scancode == 0x09) // shift + _ (1)
            return '8';
        else if (scancode == 0x0a) // shift + ^ (1)
            return '9';
        else if (scancode == 0x0b) // shift + @ (1)
            return '0';

        // others
        else if (scancode == 0x33) // shift + ; (.)
            return '.';
        else if (scancode == 0x1a) // shift + ^ (¨) = [
            return '[';
        else if (scancode == 0x1b) // shift + $ (£) = ]
            return ']';
        else if (scancode == 0x28) // shift + ù (%)
            return '%';
        else if (scancode == 0x28) // shift + * (µ) = |
            return '|';
        else if (scancode == 0x32) // shift + , (?)
            return '?';
        else if (scancode == 0x34) // shift + : (/)
            return '/';
        else if (scancode == 0x35) // shift + ! (§)
            return '\\';
        else if (scancode == 0x0d) // shift + = (+)
            return '+';
        return ASCIITable[scancode] - 32;
    }

    if (scancode == Spacebar)
        return ' ';
    else if (scancode == 0x28) // ù
        return '-';

    return ASCIITable[scancode];
}
}
