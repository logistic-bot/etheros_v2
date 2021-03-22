#include "keyboard.h"
#include "../cstr.h"
#include "../io.h"

namespace Keyboard {
bool isLeftShiftPressed;
bool isRightShiftPressed;
bool isSuperPressed;
bool isRightAltGrPressed;
bool isLeftAltPressed;
bool isLeftControlPressed;
bool isRightControlPressed;
bool isExtended;
char key;
uint8_t scancode;
bool updated = false;
void HandleKeyboard(uint8_t code) {
    if (code == Extended) {
        isExtended = true;
        return;
    }
    updated = true;
    scancode = code;
    key = 0;

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
    case Super:
        isSuperPressed = true;
        return;
    case Super + 0x80:
        isSuperPressed = false;
        return;
    case AnyAlt:
        if (isExtended) {
            isRightAltGrPressed = true;
            return;
        }
        isLeftAltPressed = true;
        return;
    case AnyAlt + 0x80:
        if (isExtended) {
            isRightAltGrPressed = false;
            return;
        }
        isLeftAltPressed = false;
        return;
    case AnyControl:
        if (isExtended) {
            isRightControlPressed = true;
            return;
        }
        isLeftControlPressed = true;
        return;
    case AnyControl + 0x80:
        if (isExtended) {
            isRightControlPressed = false;
            return;
        }
        isLeftControlPressed = false;
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

    if (isExtended)
        serial_print("EXTENDED ");
    serial_print("KBD ");
    serial_print(to_hstring(scancode));
    serial_print(" ");
    serial_print(ascii);
    serial_println("");
    isExtended = false;
}

void wait_for_key() {
    while (!updated) {
        asm("hlt");
    }
}
}
