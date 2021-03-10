#pragma once
#include <stdint.h>

namespace PIT {
extern double time_since_boot;
const uint64_t base_frequency = 1193182;

void sleep_s(double seconds);
void sleep_ms(uint64_t miliseconds);

void set_divisor(uint16_t divisior);
uint64_t get_frequency();
void set_frequency(uint64_t frequency);
void tick();
}
