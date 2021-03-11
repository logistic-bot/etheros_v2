#include "colors.h"
#include "BasicRenderer.h"

void show_color(uint32_t x, uint32_t y, uint32_t size, unsigned int color) {
    renderer->rect(x, y, size, size, color);
}

void show_pallete(uint32_t origin_x, uint32_t origin_y, uint32_t size) {
    uint32_t x = origin_x;
    uint32_t y = origin_y;

    show_color(x, y, size, COLOR_BACKGROUND_NORMAL);
    x += size;
    show_color(x, y, size, COLOR_BACKGROUND_HIGHLIGHT);
    x += size;
    show_color(x, y, size, COLOR_BACKGROUND_DIM);
    x = origin_x;
    y += size; // new line
    show_color(x, y, size, COLOR_FOREGROUND_NORMAL);
    x += size;
    show_color(x, y, size, COLOR_FOREGROUND_HIGHLIGHT);
    x += size;
    show_color(x, y, size, COLOR_FOREGROUND_DIM);
    x = origin_x;
    y += size; // new line
    show_color(x, y, size, COLOR_ACCENT);
    x += size;
    show_color(x, y, size, COLOR_ACCENT_INACTIVE);
    x += size;
    show_color(x, y, size, COLOR_YES);
    x += size;
    show_color(x, y, size, COLOR_YES_INACTIVE);
    x += size;
    show_color(x, y, size, COLOR_NO);
    x += size;
    show_color(x, y, size, COLOR_NO_INACTIVE);
    x += size;
}
