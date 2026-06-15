#ifndef CONFIG_H
#define CONFIG_H

#include <vector>

#define NUM_STRIPS 4
#define PIXELS_PER_STRIP 10

std::vector<uint8_t> STRIP_PIXEL_COUNT = {100, 0, 0, 0};

const uint32_t PWM_FREQ = 20000; // 20 kHz
const uint8_t PWM_RES = 10;
const uint32_t MAX_DUTY = (1 << PWM_RES) - 1;

#endif // CONFIG_H