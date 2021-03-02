#include "BasicRenderer.h"

BasicRenderer::BasicRenderer(Framebuffer *targetFramebuffer_, PSF1_FONT *font_)
{
    targetFramebuffer = targetFramebuffer_;
    font = font_;

    color = 0xffffffff;
    cursorPosition = {0, 0};
}

void BasicRenderer::print(const char *str)
{
    char *chr = (char *)str;
    while (*chr != 0)
    {
        putChar(*chr, cursorPosition.x, cursorPosition.y);
        cursorPosition.x += 8;
        if (cursorPosition.x + 8 > targetFramebuffer->Width)
        {
            cursorPosition.x = 0;
            cursorPosition.y += 16;
        }
        chr++;
    }
}

void BasicRenderer::putChar(char chr, unsigned int xOff, unsigned int yOff)
{
    unsigned int *pixelPointer = (unsigned int *)targetFramebuffer->BaseAddress;
    char *fontPtr = (char *)font->glyphBuffer + (chr * font->psf_header->charsize);
    for (unsigned long y = yOff; y < yOff + font->psf_header->charsize; y++)
    {
        for (unsigned long x = xOff; x < xOff + 8; x++)
        {
            if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0)
            {
                *(unsigned int *)(pixelPointer + x + (y * targetFramebuffer->PixelsPerScanline)) = color;
            }
        }
        fontPtr++;
    }
}
