#pragma once

#include "../BasicRenderer.h"
#include "scancodes.h"
#include <stdint.h>

namespace Keyboard {
extern bool isLeftShiftPressed;
extern bool isRightShiftPressed;
extern bool isExtended;
extern bool isLeftAltPressed;
extern bool isRightAltGrPressed;
extern bool isLeftControlPressed;
extern bool isRightControlPressed;
extern char key;
extern uint8_t scancode;
extern bool updated;

void HandleKeyboard(uint8_t scancode);
void wait_for_key();
}
