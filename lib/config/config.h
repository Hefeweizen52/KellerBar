#ifndef CONFIG_H
#define CONFIG_H

#include <vector>

#define NUM_STRIPS 4

std::vector<size_t> STRIP_PIXEL_COUNT = {252, 0, 0, 0};

const uint32_t PWM_FREQ = 20000; // 20 kHz
const uint8_t PWM_RES = 10;
const uint32_t MAX_DUTY = (1 << PWM_RES) - 1;

enum PIXEL_MODES
{   
    PIXEL_MODE_STATIC,
    PIXEL_MODE_SINE,
    PIXEL_MODE_BI_SINE,
    PIXEL_MODE_NEON,
    PIXEL_MODE_SPARKLE
};

#endif // CONFIG_H