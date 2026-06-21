#ifndef SINE_H
#define SINE_H

#include <Arduino.h>
#include <comfort_functions.hpp>

namespace sine
{
    struct sine_config_t
    {
        size_t idx_begin, count;

        // Konstruktor analog zu neon
        sine_config_t(size_t idx_begin, size_t count) : idx_begin(idx_begin), count(count) {};
        sine_config_t() = default;
    };

    struct sine_t
    {
        sine_config_t conf;
        uint32_t counter = 0;
        comfort_functions *strip = nullptr;

        sine_t() = default;
        sine_t(sine_config_t sine_config, comfort_functions *strip) : conf(sine_config), strip(strip) {};

        /**
         * Setzt den Frame-Counter zurück und leert die LEDs der Animation
         */
        void reset();

        /**
         * Berechnet den Sinus-Frame basierend auf den dynamischen Parametern
         */
        void tick(uint32_t wavelength, uint32_t speed, HSL base_color);
    };
}

#endif // SINE_H