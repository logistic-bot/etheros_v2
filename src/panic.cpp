#include "panic.h"
#include "BasicRenderer.h"

void panic(const char* panic_message) {
    renderer->background_color = 0x00ff0000;
    renderer->clear();
    renderer->cursorPosition = {0,0};
    renderer->color = 0;
    renderer->print("Kernel Panic");
    renderer->next();
    renderer->print(panic_message);
}
