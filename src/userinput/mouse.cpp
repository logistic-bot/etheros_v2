#include "mouse.h"

uint8_t mouse_pointer[] = {
    0b10000000,
    0b00000000,
    0b11000000,
    0b00000000,
    0b11100000,
    0b00000000,
    0b11110000,
    0b00000000,
    0b11111000,
    0b00000000,
    0b11111100,
    0b00000000,
    0b11110000,
    0b00000000,
    0b11000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
};

void mouse_wait() {
    uint64_t timeout = 100000;
    while (timeout--) {
        if ((inb(0x64) & 0b10) == 0) {
            return;
        }
    }
}

void mouse_wait_input() {
    uint64_t timeout = 100000;
    while (timeout--) {
        if (inb(0x64) & 0b1) {
            return;
        }
    }
}

void mouse_write(uint8_t value) {
    mouse_wait();
    outb(0x64, 0xd4);
    mouse_wait();
    outb(0x60, value);
}

uint8_t mouse_read() {
    mouse_wait_input();
    return inb(0x60);
}

uint8_t mouse_cycle = 0;
uint8_t mouse_packets[4];
bool mouse_packet_ready = false;
Point mouse_position;
Point old_mouse_position;
void handle_ps2_mouse(uint8_t data) {
    process_mouse_packets();
    static bool skip = true;
    if (skip) {
        skip = false;
        return;
    }

    switch (mouse_cycle) {
    case 0:
        if ((data & 0b00001000) == 0)
            break;
        mouse_packets[0] = data;
        mouse_cycle++;
        break;
    case 1:
        mouse_packets[1] = data;
        mouse_cycle++;
        break;
    case 2:
        mouse_packets[2] = data;
        mouse_packet_ready = true;
        mouse_cycle = 0;
        break;
    }
}

void init_ps2mouse() {
    outb(0x64, 0xa8); // Enable auxilary device: mouse
    mouse_wait();
    outb(0x64, 0x20); // tell the keyboard controler that we want to send a command to the mouse
    mouse_wait_input();

    // set the correct bit - compaq status byte
    uint8_t status = inb(0x60);
    status |= 0b10;
    mouse_wait();
    outb(0x64, 0x60);
    mouse_wait();
    outb(0x60, status);

    mouse_write(0xf6); // default settings
    mouse_read();

    mouse_write(0xf4); // enable
    mouse_read();
}

void process_mouse_packets() {
    if (!mouse_packet_ready)
        return;
    bool x_negative, y_negative, x_overflow, y_overflow;

    if (mouse_packets[0] & PS2XSIGN) {
        x_negative = true;
    } else {
        x_negative = false;
    }

    if (mouse_packets[0] & PS2YSIGN) {
        y_negative = true;
    } else {
        y_negative = false;
    }

    if (mouse_packets[0] & PS2XOVERFLOW) {
        x_overflow = true;
    } else {
        x_overflow = false;
    }

    if (mouse_packets[0] & PS2YOVERFLOW) {
        y_overflow = true;
    } else {
        y_overflow = false;
    }

    if (!x_negative) {
        mouse_position.x += mouse_packets[1];
        if (x_overflow) {
            mouse_position.x += 255;
        }
    } else {
        mouse_packets[1] = 256 - mouse_packets[1];
        mouse_position.x -= mouse_packets[1];
        if (x_overflow) {
            mouse_position.x -= 255;
        }
    }

    if (!y_negative) {
        mouse_position.y -= mouse_packets[2];
        if (y_overflow) {
            mouse_position.y -= 255;
        }
    } else {
        mouse_packets[2] = 256 - mouse_packets[2];
        mouse_position.y += mouse_packets[2];
        if (y_overflow) {
            mouse_position.y += 255;
        }
    }

    if (mouse_position.x < 0)
        mouse_position.x = 0;
    if (mouse_position.x > renderer->targetFramebuffer->Width - 1)
        mouse_position.x = renderer->targetFramebuffer->Width - 1;
    if (mouse_position.y < 0)
        mouse_position.y = 0;
    if (mouse_position.y > renderer->targetFramebuffer->Height - 1)
        mouse_position.y = renderer->targetFramebuffer->Height - 1;

    renderer->clear_mouse_cursor(old_mouse_position, mouse_pointer);

    unsigned int old_color = renderer->color;
    renderer->color = 0xffffffff;
    if (mouse_packets[0] & PS2LEFTBUTTON) {
        renderer->color = 0x00ffff00;
    }
    if (mouse_packets[0] & PS2RIGHTBUTTON) {
        renderer->color = 0x0000ffff;
    }
    if (mouse_packets[0] & PS2MIDDLEBUTTON) {
        renderer->color = 0x00ff00ff;
    }

    renderer->draw_overlay_mouse_cursor(mouse_position, mouse_pointer);
    renderer->color = old_color;

    old_mouse_position = mouse_position;
    mouse_packet_ready = false;
}
