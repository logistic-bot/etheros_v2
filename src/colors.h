#pragma once
#include <stdint.h>

// Mostly inspired by Dark Synth '84
// Source: https://github.com/brainomite/dark-synthwave-vscode

void show_pallete(uint32_t origin_x, uint32_t origin_y, uint32_t size);

#define COLOR_BACKGROUND_NORMAL 0x0c0c0c
#define COLOR_BACKGROUND_HIGHLIGHT 0x1a1a1a
#define COLOR_BACKGROUND_DIM 0x000000
#define COLOR_BACKGROUND_INACTIVE COLOR_BACKGROUND_DIM

#define COLOR_FOREGROUND_NORMAL 0xccccc5
#define COLOR_FOREGROUND_HIGHLIGHT 0xffffff
#define COLOR_FOREGROUND_DIM 0x3d3d3d
#define COLOR_FOREGROUND_INACTIVE COLOR_FOREGROUND_DIM

#define COLOR_ACCENT 0xff3366
#define COLOR_ACCENT_INACTIVE 0xdb2c58
#define COLOR_ACCENT_DIM COLOR_ACCENT_INACTIVE

#define COLOR_YES 0x8fff45
#define COLOR_YES_INACTIVE 0x64b330
#define COLOR_YES_DIM COLOR_YES_INACTIVE

#define COLOR_NO 0xff2424
#define COLOR_NO_INACTIVE 0xdb1f1f
#define COLOR_NO_DIM COLOR_NO_INACTIVE
