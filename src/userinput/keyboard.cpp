#include "keyboard.h"

bool isLeftShiftPressed;
bool isRightShiftPressed;
void HandleKeyboard(uint8_t scancode) {
    switch (scancode) {
        case LeftShift:
            isLeftShiftPressed = true;
            return;
        case LeftShift + 0x80:
            isLeftShiftPressed = false;
            return;
        case RightShift:
            isRightShiftPressed = true;
            return;
        case RightShift + 0x80:
            isRightShiftPressed = false;
            return;
        case Enter:
            renderer->next();
            break;
        case Spacebar:
            renderer->putChar(' ');
            return;
        case Backspace:
            renderer->clear_char();
            break;
    }

    char ascii = AZERTYKeyboard::translate(scancode, isLeftShiftPressed | isRightShiftPressed);
    if (ascii != 0) {
        renderer->putChar(ascii);
    }
}
