#include "colors.h"
#include "kernelUtil.h"
#include "memory/heap.h"
#include "scheduling/pit/pit.h"
#include "userinput/keyboard.h"
#include "userinput/scancodes.h"

char* action = 0;
uint8_t action_pos = 0;

void clear_input(unsigned int color) {
    uint32_t s_height = renderer->targetFramebuffer->Height - 16;
    Point pos = renderer->cursorPosition;

    renderer->cursorPosition = { 0, s_height };

    renderer->rect(0, s_height, renderer->targetFramebuffer->PixelsPerScanline, 16, color);
    renderer->print(">", COLOR_YES);

    renderer->cursorPosition = pos;
}

void clear_input() {
    clear_input(COLOR_BACKGROUND_HIGHLIGHT);
}

void draw_text_cursor() {
    renderer->text_cursor_position = { 8 + 8 * action_pos, renderer->targetFramebuffer->Height - 16 };
    renderer->show_text_cursor();
}

void clear_text_cursor() {
    renderer->rect(2 + 8 * action_pos - 1, renderer->targetFramebuffer->Height - 16, 1, 16, COLOR_BACKGROUND_HIGHLIGHT);
}

void cmd_add_char(char chr) {
    action[action_pos] = chr;
    action_pos++;
    action[action_pos] = 0;
}

void draw_text() {
    Point pos = renderer->cursorPosition;
    renderer->cursorPosition = { 8, renderer->targetFramebuffer->Height - 16 };
    renderer->print(action);
    renderer->cursorPosition = pos;
}

void draw_input() {
    clear_input();
    draw_text_cursor();
    draw_text();
}

void flash_action_line() {
    clear_input(COLOR_NO);
    PIT::sleep_ms(100);
    draw_text_cursor();
    draw_text();
    draw_input();
}

void redraw_input() {
    clear_text_cursor();
    renderer->putChar(action[action_pos - 1], 8 * action_pos, renderer->targetFramebuffer->Height - 16);
    draw_text_cursor();
}

void commit() {
    renderer->print(">", COLOR_YES);
    renderer->println(action, COLOR_FOREGROUND_HIGHLIGHT);
    action_pos = 0;
    action[0] = 0;
    draw_input();
}

void backspace() {
    if (action_pos > 0) {
        action_pos++;
        clear_text_cursor();
        Point pos = renderer->cursorPosition;
        renderer->cursorPosition = { 8 * action_pos, renderer->targetFramebuffer->Height - 16 };
        renderer->clear_char(COLOR_BACKGROUND_HIGHLIGHT);
        renderer->cursorPosition = pos;

        action_pos--;
        action_pos--;
        action[action_pos] = 0;
        draw_text_cursor();
    } else {
        flash_action_line();
    }
}

extern "C" void _start(BootInfo* bootInfo) {
    // initialize kernel
    KernelInfo kernel_info = initialize_kernel(bootInfo);

    renderer->println("Kernel Initialized Sucessfully");
    renderer->next();
    PIT::set_divisor(100);
    PIT::sleep_ms(1);

    renderer->text_cursor_position = { 8, renderer->targetFramebuffer->Height - 16 };

    char key;
    action[0] = 0;
    draw_input();
    while (true) {
        Keyboard::wait_for_key();
        Keyboard::updated = false;
        key = Keyboard::key;

        switch (key) {
        case 0: // special
            if (Keyboard::scancode == Enter) {
                commit();
            } else if (Keyboard::scancode == Backspace) {
                backspace();
            }
            continue;
        default: // normal
            cmd_add_char(key);
            redraw_input();
        }
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
