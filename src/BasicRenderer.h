#pragma once
#include "math.h"
#include "Framebuffer.h"
#include "simpleFonts.h"

class BasicRenderer
{
public:
    BasicRenderer(Framebuffer *targetFramebuffer_, PSF1_FONT *font_);
    Point cursorPosition;
    Framebuffer *targetFramebuffer;
    PSF1_FONT *font;
    unsigned int color;
    void print(const char *str);
    void putChar(char chr, unsigned int xOff, unsigned int yOff);
};
