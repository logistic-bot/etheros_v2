#include "BasicRenderer.h"
#include "colors.h"
#include "cstr.h"
#include "io.h"

BasicRenderer* renderer;

BasicRenderer::BasicRenderer(Framebuffer* targetFramebuffer_, PSF1_FONT* font_) {
    targetFramebuffer = targetFramebuffer_;
    font = font_;

    color = COLOR_FOREGROUND_NORMAL;
    background_color = COLOR_BACKGROUND_NORMAL;
    cursorPosition = { 0, 0 };
}

void BasicRenderer::putPix(uint32_t x, uint32_t y, unsigned int color) {
    *(uint32_t*)((uint64_t)targetFramebuffer->BaseAddress + x * 4 + (y * targetFramebuffer->PixelsPerScanline * 4)) = color;
}

void BasicRenderer::rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, unsigned int color) {
    for (uint32_t x_pos = x; x_pos < x + width; x_pos++) {
        for (uint32_t y_pos = y; y_pos < y + height; y_pos++) {
            *(uint32_t*)((uint64_t)targetFramebuffer->BaseAddress + x_pos * 4 + (y_pos * targetFramebuffer->PixelsPerScanline * 4)) = color;
        }
    }
}

void BasicRenderer::rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    this->rect(x, y, width, height, color);
}

uint32_t BasicRenderer::getPix(uint32_t x, uint32_t y) {
    return *(uint32_t*)((uint64_t)targetFramebuffer->BaseAddress + x * 4 + (y * targetFramebuffer->PixelsPerScanline * 4));
}

void BasicRenderer::clear_mouse_cursor(Point position, uint8_t* mouse_cursor) {
    if (!mouse_drawn)
        return;
    int xMax = 16;
    int yMax = 16;
    int differenceX = targetFramebuffer->Width - position.x;
    int differenceY = targetFramebuffer->Height - position.y;
    if (differenceX < 16)
        xMax = differenceX;
    if (differenceY < 16)
        yMax = differenceY;

    for (int y = 0; y < yMax; y++) {
        for (int x = 0; x < xMax; x++) {
            int bit = y * 16 + x;
            int byte = bit / 8;
            if ((mouse_cursor[byte] & (0b10000000 >> (x % 8)))) {
                if (getPix(position.x + x, position.y + y) == mouse_cursor_buffer_after[x + y * 16]) {
                    putPix(position.x + x, position.y + y, mouse_cursor_buffer[x + y * 16]);
                }
            }
        }
    }
}

void BasicRenderer::draw_overlay_mouse_cursor(Point position, uint8_t* mouse_cursor) {
    int xMax = 16;
    int yMax = 16;
    int differenceX = targetFramebuffer->Width - position.x;
    int differenceY = targetFramebuffer->Height - position.y;
    if (differenceX < 16)
        xMax = differenceX;
    if (differenceY < 16)
        yMax = differenceY;

    for (int y = 0; y < yMax; y++) {
        for (int x = 0; x < xMax; x++) {
            int bit = y * 16 + x;
            int byte = bit / 8;
            if ((mouse_cursor[byte] & (0b10000000 >> (x % 8)))) {
                mouse_cursor_buffer[x + y * 16] = getPix(position.x + x, position.y + y);
                putPix(position.x + x, position.y + y, color);
                mouse_cursor_buffer_after[x + y * 16] = getPix(position.x + x, position.y + y);
            }
        }
    }

    mouse_drawn = true;
}

void BasicRenderer::clear() {
    uint64_t fbBase = (uint64_t)targetFramebuffer->BaseAddress;
    uint64_t bytesPerScanline = targetFramebuffer->PixelsPerScanline * 4;
    uint64_t fbHeight = targetFramebuffer->Height;

    for (uint64_t verticalScanline = 0; verticalScanline < fbHeight; verticalScanline++) {
        uint64_t pixPtrBase = fbBase + (bytesPerScanline * verticalScanline);
        for (uint32_t* pixPtr = (uint32_t*)pixPtrBase; pixPtr < (uint32_t*)(pixPtrBase + bytesPerScanline); pixPtr++) {
            *pixPtr = background_color;
        }
    }
}

void BasicRenderer::clear_char(unsigned int color) {
    if (cursorPosition.x == 0) {
        cursorPosition.x = targetFramebuffer->Width;
        cursorPosition.y -= 16;
        if (cursorPosition.y < 0) {
            cursorPosition.y = 0;
        }
    }

    unsigned int xOff = cursorPosition.x;
    unsigned int yOff = cursorPosition.y;

    unsigned int* pixelPointer = (unsigned int*)targetFramebuffer->BaseAddress;
    for (unsigned long y = yOff; y < yOff + 16; y++) {
        for (unsigned long x = xOff - 8; x < xOff; x++) {
            *(unsigned int*)(pixelPointer + x + (y * targetFramebuffer->PixelsPerScanline)) = color;
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

void BasicRenderer::clear_char() {
    this->clear_char(this->background_color);
}

void BasicRenderer::next() {
    cursorPosition.x = 0;
    cursorPosition.y += 16;
}

void BasicRenderer::center() {
    cursorPosition.x = targetFramebuffer->PixelsPerScanline / 2;
    cursorPosition.y = targetFramebuffer->Height / 2;
}

void BasicRenderer::print(const char* str) {
    this->print(str, color);
}

void BasicRenderer::println(const char* str) {
    this->print(str);
    this->next();
}

void BasicRenderer::print(const char* str, unsigned int color) {
    char* chr = (char*)str;
    while (*chr != 0) {
        putChar(*chr, cursorPosition.x, cursorPosition.y, color);
        cursorPosition.x += 8;
        if (cursorPosition.x + 8 > targetFramebuffer->Width) {
            cursorPosition.x = 0;
            cursorPosition.y += 16;
        }
        chr++;
    }
    serial_print(str);
}
void BasicRenderer::println(const char* str, unsigned int color) {
    this->print(str, color);
    this->next();
}

void BasicRenderer::putChar(char chr, unsigned int xOff, unsigned int yOff) {
    this->putChar(chr, xOff, yOff, color);
}

void BasicRenderer::putChar(char chr, unsigned int xOff, unsigned int yOff, unsigned int color) {
    unsigned int* pixelPointer = (unsigned int*)targetFramebuffer->BaseAddress;
    char* fontPtr = (char*)font->glyphBuffer + (chr * font->psf_header->charsize);
    for (unsigned long y = yOff; y < yOff + font->psf_header->charsize; y++) {
        for (unsigned long x = xOff; x < xOff + 8; x++) {
            if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0) {
                *(unsigned int*)(pixelPointer + x + (y * targetFramebuffer->PixelsPerScanline)) = color;
            }
        }
        fontPtr++;
    }
}

void BasicRenderer::putChar(char chr) {
    putChar(chr, cursorPosition.x, cursorPosition.y);
    cursorPosition.x += 8;
    if (cursorPosition.x + 8 > targetFramebuffer->Width) {
        next();
    }
}

void BasicRenderer::show_text_cursor() {
    this->show_text_cursor(this->text_cursor_position.x, this->text_cursor_position.y);
}

void BasicRenderer::show_text_cursor(uint32_t x, uint32_t y) {
    switch (this->text_cursor_shape) {
    case TextCursorShape::IBeam:
        this->show_i_beam_text_cursor(x, y);
        break;
    case TextCursorShape::Block:
        this->show_block_text_cursor(x, y);
        break;
    default:
        show_i_beam_text_cursor(x, y);
    }
}
void BasicRenderer::show_i_beam_text_cursor(uint32_t x, uint32_t y) {
    this->rect(x + 1, y + 1, 1, 14, COLOR_ACCENT);
}
void BasicRenderer::show_block_text_cursor(uint32_t x, uint32_t y) {
    unsigned int background = background_color;
    unsigned int foreground = color;
    unsigned int c;
    bool got_background = false;
    bool done = false;
    for (uint32_t x_pos = x; x_pos < x + 8; x_pos++) {
        for (uint32_t y_pos = y; y_pos < y + 16; y_pos++) {
            c = getPix(x_pos, y_pos);
            if (got_background) {
                if (c != foreground) {
                    foreground = c;
                    continue;
                }
            } else {
                background = c;
                got_background = true;
                continue;
            }
            done = true;
            break;
        }
        if (done) {
            break;
        }
    }

    // This is a dirty fix. In the future, each character should have a foreground and background color, and this will make this much easier.
    if (background == foreground) {
        foreground = COLOR_FOREGROUND_HIGHLIGHT;
    }
    serial_println("");
    serial_println(to_hstring((uint64_t)background));
    serial_println(to_hstring((uint64_t)foreground));

    for (uint32_t x_pos = x; x_pos < x + 8; x_pos++) {
        for (uint32_t y_pos = y; y_pos < y + 16; y_pos++) {
            if (getPix(x_pos, y_pos) == background) {
                putPix(x_pos, y_pos, foreground);
                serial_print("2");
            } else {
                putPix(x_pos, y_pos, background);
                serial_print("1");
            }
        }
        serial_println("");
    }
}
