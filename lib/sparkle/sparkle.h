#ifndef SPARKLE_H
#define SPARKLE_H

#define PIXELS_PER_STAR 10U
#define MIN_SPARKLE_DELAY 5
#define MAX_SPARKLE_DELAY 255

#include <Arduino.h>
#include <comfort_functions.hpp>

namespace sparkle
{
    struct sparkle_config_t
    {
        size_t idx_begin, count;

        sparkle_config_t() = default;
        sparkle_config_t(size_t idx_begin, size_t count) : idx_begin(idx_begin), count(count) {};
    };

    struct star_t
    {
        size_t pos;
        uint32_t counter;
        uint8_t delay;
        float l;
    };

    struct sparkle_t
    {
        sparkle_config_t conf;
        uint32_t counter = 0;
        uint32_t prev_counter = 0;
        comfort_functions *strip = nullptr;
        std::vector<star_t> stars;

        sparkle_t() = default;
        sparkle_t(sparkle_config_t sparkle_config, comfort_functions *strip) : conf(sparkle_config), strip(strip) {};

        void reset();

        void tick(uint32_t n_ticks, HSL base_color);
    };
}

#endif // SPARKLE_H