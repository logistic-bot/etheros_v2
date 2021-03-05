#pragma once
#include "math.h"
#include "Framebuffer.h"
#include "simpleFonts.h"
#include <stdint.h>

class BasicRenderer
{
public:
    BasicRenderer(Framebuffer *targetFramebuffer_, PSF1_FONT *font_);
    Point cursorPosition;
    Framebuffer *targetFramebuffer;
    PSF1_FONT *font;
    uint32_t mouse_cursor_buffer[16 * 16];
    uint32_t mouse_cursor_buffer_after[16 * 16];
    unsigned int color;
    unsigned int background_color;
    void print(const char *str);
    void putChar(char chr, unsigned int xOff, unsigned int yOff);
    void putChar(char chr);
    void putPix(uint32_t x, uint32_t y, unsigned int color);
    uint32_t getPix(uint32_t x, uint32_t y);
    void clear_char();
    void clear();
    void next();
    void center();
    void draw_overlay_mouse_cursor(Point position, uint8_t* mouse_cursor);
    void clear_mouse_cursor(Point position, uint8_t* mouse_cursor);
    bool mouse_drawn;
};

extern BasicRenderer* renderer;
