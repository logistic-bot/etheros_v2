#include "interupts.h"
#include "../panic.h"

__attribute__((interrupt)) void PageFault_handler(struct interrupt_frame* frame) {
    panic("Page fault detected");
    while (true);
}
