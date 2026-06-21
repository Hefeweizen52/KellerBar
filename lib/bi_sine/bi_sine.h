#ifndef BI_SINE_H
#define BI_SINE_H

#include <Arduino.h>
#include <comfort_functions.hpp>

namespace bi_sine
{
    struct bi_sine_config_t
    {
        size_t idx_begin, count;

        bi_sine_config_t(size_t idx_begin, size_t count) : idx_begin(idx_begin), count(count) {};
        bi_sine_config_t() = default;
    };

    struct bi_sine_t
    {
        bi_sine_config_t conf;
        uint32_t counter = 0;
        comfort_functions *strip = nullptr;

        bi_sine_t() = default;
        bi_sine_t(bi_sine_config_t bi_sine_config, comfort_functions *strip) : conf(bi_sine_config), strip(strip) {};

        void reset();
        void tick(uint32_t wavelength, uint32_t speed, HSL color1, HSL color2);
    };
}

#endif // BI_SINE_H