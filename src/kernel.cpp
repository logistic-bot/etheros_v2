#include "kernelUtil.h"
#include "memory/heap.h"
#include "scheduling/pit/pit.h"

extern "C" void _start(BootInfo* bootInfo) {
    // initialize kernel
    KernelInfo kernel_info = initialize_kernel(bootInfo);

    renderer->print("Kernel Initialized Sucessfully");
    serial_println("");
    renderer->next();

    PIT::set_divisor(200);

    serial_print(to_string(PIT::time_since_boot));
    serial_print(" ");
    renderer->rect(100, 100, 100, 100);
    serial_println(to_string(PIT::time_since_boot));

    uint32_t x = 0;
    uint32_t y = 0;
    while (true) {
        renderer->rect(x, y, 10, 10);
        //        PIT::sleep_ms(100);
        renderer->rect(x, y, 10, 10, renderer->background_color);
        x++;
    }

    //    uint64_t pages = 0;
    //    while (true) {
    //        void* result = allocator.request_page();
    //        if (result == NULL) {
    //            renderer->print("OOM");
    //            renderer->cursorPosition = {0,renderer->cursorPosition.y + 16};
    //            renderer->print(to_string(pages));
    //            break;
    //        }
    //        pages += 1;
    //        if (pages % 5000 == 0) {
    //            renderer->print(to_string(pages));
    //            renderer->cursorPosition = {0,renderer->cursorPosition.y + 16};
    //        }
    //    }

    // prevent crash on real hardware
    while (true)
        ;
}

// KNOWN BUG (MAYBE RESOLVED): (with 256M of memory) when allocating 56944 pages, GetFreeRAM() returns stranges, waytoobig values (> 10000000000). OOM happens at 56952 pages.
// It seems like the kernel was causing this 4kb dicreapency.
