#include "keyboard.h"
#include "../cstr.h"
#include "../io.h"

namespace Keyboard {
bool isLeftShiftPressed;
bool isRightShiftPressed;
char key;
uint8_t scancode;
bool updated = false;
void HandleKeyboard(uint8_t code) {
    updated = true;
    scancode = code;

    switch (scancode) {
    case LeftShift:
        isLeftShiftPressed = true;
        key = 0;
        return;
    case LeftShift + 0x80:
        isLeftShiftPressed = false;
        key = 0;
        return;
    case RightShift:
        isRightShiftPressed = true;
        key = 0;
        return;
    case RightShift + 0x80:
        isRightShiftPressed = false;
        key = 0;
        return;
        //    case Enter:
        //        renderer->next();
        //        break;
        //    case Spacebar:
        //        renderer->putChar(' ');
        //        return;
        //    case Backspace:
        //        renderer->clear_char();
        //        break;
    }

    char ascii = AZERTYKeyboard::translate(scancode, isLeftShiftPressed | isRightShiftPressed);
    key = ascii;
}

void wait_for_key() {
    while (!updated) {
        asm("hlt");
    }
}
}
