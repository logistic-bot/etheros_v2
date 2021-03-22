#include "colors.h"
#include "kernelUtil.h"
#include "memory/heap.h"
#include "scheduling/pit/pit.h"
#include "userinput/keyboard.h"
#include "userinput/scancodes.h"

extern "C" void _start(BootInfo* bootInfo) {
    KernelInfo kernel_info = initialize_kernel(bootInfo);

    // prevent crash on real hardware
    while (true)
        asm("hlt");
}

// KNOWN BUG (MAYBE RESOLVED): (with 256M of memory) when allocating 56944 pages, GetFreeRAM() returns stranges, waytoobig values (> 10000000000). OOM happens at 56952 pages.
// It seems like the kernel was causing this 4kb dicreapency.
