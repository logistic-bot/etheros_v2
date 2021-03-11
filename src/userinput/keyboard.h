#pragma once

#include "../BasicRenderer.h"
#include "scancodes.h"
#include <stdint.h>

namespace Keyboard {
extern bool isLeftShiftPressed;
extern bool isRightShiftPressed;
extern char key;
extern uint8_t scancode;
extern bool updated;

void HandleKeyboard(uint8_t scancode);
void wait_for_key();
}
