#include "panic.h"
#include "BasicRenderer.h"

void panic(char* panic_message) {
    renderer->clear(0x00ff0000);
    renderer->cursorPosition = {0,0};
    renderer->color = 0;
    renderer->print("Kernel Panic");
    renderer->next();
    renderer->print(panic_message);
}
