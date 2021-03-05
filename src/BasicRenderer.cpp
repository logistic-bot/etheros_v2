#include "BasicRenderer.h"

BasicRenderer* renderer;

BasicRenderer::BasicRenderer(Framebuffer *targetFramebuffer_, PSF1_FONT *font_)
{
    targetFramebuffer = targetFramebuffer_;
    font = font_;

    color = 0xffffffff;
    background_color = 0;
    cursorPosition = {0, 0};
}

void BasicRenderer::clear() {
    uint64_t fbBase = (uint64_t)targetFramebuffer->BaseAddress;
    uint64_t bytesPerScanline = targetFramebuffer->PixelsPerScanline * 4;
    uint64_t fbHeight = targetFramebuffer->Height;
    uint64_t fbSize = targetFramebuffer->BufferSize;

    for (int verticalScanline = 0; verticalScanline < fbHeight; verticalScanline++) {
        uint64_t pixPtrBase = fbBase + (bytesPerScanline * verticalScanline);
        for (uint32_t* pixPtr = (uint32_t*)pixPtrBase; pixPtr < (uint32_t*)(pixPtrBase + bytesPerScanline); pixPtr++) {
            *pixPtr = background_color;
        }
    }
}

void BasicRenderer::clear_char() {
    if (cursorPosition.x == 0) {
        cursorPosition.x = targetFramebuffer->Width;
        cursorPosition.y -= 16;
        if (cursorPosition.y < 0) {
            cursorPosition.y = 0;
        }
    }

    unsigned int xOff = cursorPosition.x;
    unsigned int yOff = cursorPosition.y;

    unsigned int *pixelPointer = (unsigned int *)targetFramebuffer->BaseAddress;
    for (unsigned long y = yOff; y < yOff + 16; y++)
    {
        for (unsigned long x = xOff - 8; x < xOff; x++)
        {
            *(unsigned int *)(pixelPointer + x + (y * targetFramebuffer->PixelsPerScanline)) = background_color;
        }
    }

    cursorPosition.x -= 8;
    if (cursorPosition.x < 0) {
        cursorPosition.x = targetFramebuffer->Width;
        cursorPosition.y -= 16;
        if (cursorPosition.y < 0) {
            cursorPosition.y = 0;
        }
    }
}

void BasicRenderer::next() {
    cursorPosition.x = 0;
    cursorPosition.y += 16;
}

void BasicRenderer::center() {
    cursorPosition.x = targetFramebuffer->PixelsPerScanline / 2;
    cursorPosition.y = targetFramebuffer->Height / 2;
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

void BasicRenderer::putChar(char chr)
{
    putChar(chr, cursorPosition.x, cursorPosition.y);
    cursorPosition.x += 8;
    if (cursorPosition.x + 8 > targetFramebuffer->Width) {
        next();
    }
}
