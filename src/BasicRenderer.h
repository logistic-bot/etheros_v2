#pragma once
#include "Framebuffer.h"
#include "math.h"
#include "simpleFonts.h"
#include <stdint.h>

enum TextCursorShape {
    IBeam,
    Block
};

class BasicRenderer {
public:
    BasicRenderer(Framebuffer* targetFramebuffer_, PSF1_FONT* font_);
    Point cursorPosition;
    Point text_cursor_position;
    TextCursorShape text_cursor_shape;
    Framebuffer* targetFramebuffer;
    PSF1_FONT* font;
    uint32_t mouse_cursor_buffer[16 * 16];
    uint32_t mouse_cursor_buffer_after[16 * 16];
    unsigned int color;
    unsigned int background_color;

    void print(const char* str, unsigned int color);
    void println(const char* str, unsigned int color);
    void print(const char* str);
    void println(const char* str);

    void putChar(char chr, unsigned int xOff, unsigned int yOff);
    void putChar(char chr, unsigned int xOff, unsigned int yOff, unsigned int color);
    void putChar(char chr);
    void putPix(uint32_t x, uint32_t y, unsigned int color);
    uint32_t getPix(uint32_t x, uint32_t y);
    void clear_char();
    void clear_char(unsigned int color);
    void clear();
    void next();
    void center();
    void draw_overlay_mouse_cursor(Point position, uint8_t* mouse_cursor);
    void clear_mouse_cursor(Point position, uint8_t* mouse_cursor);
    bool mouse_drawn;

    void rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    void rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, unsigned int color);

    void show_text_cursor();
    void show_text_cursor(uint32_t x, uint32_t y);

private:
    void show_i_beam_text_cursor(uint32_t x, uint32_t y);
    void show_block_text_cursor(uint32_t x, uint32_t y);
};

extern BasicRenderer* renderer;
