#include "interupts.h"

__attribute__((interrupt)) void PageFault_handler(struct interrupt_frame* frame) {
    renderer->print("Page fault detected");
    while (true);
}
