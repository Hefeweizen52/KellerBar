#include "bi_sine.h"
#include "color_utils.hpp" // Falls hier dein hslToRgb liegt
#include <cmath>

namespace bi_sine
{
    void bi_sine_t::reset()
    {
        this->counter = 0;
        if (this->strip)
        {
            this->strip->clear(this->conf.idx_begin, this->conf.count);
        }
    }

    void bi_sine_t::tick(uint32_t wavelength, uint32_t speed, HSL color1, HSL color2)
    {
        if (!this->strip) return;

        // Eingänge absichern (0 - 1023)
        uint32_t wl_raw = constrain(wavelength, 0UL, 1023UL);
        uint32_t sp_raw = constrain(speed, 0UL, 1023UL);

        float wl_pixels = (float)wl_raw;
        if (wl_raw == 0) 
        {
            wl_pixels = (float)this->conf.count;
        }

        // Zeitkomponente (Phase) berechnen
        float phase = (float)this->counter * (float)sp_raw * 0.0002f;

        // Die beiden Grundfarben in RGB umrechnen für sauberes physikalisches Mischen
        RGB rgb1 = color1.toRGB(); // Oder hslToRgb(color1), je nach deiner Library
        RGB rgb2 = color2.toRGB();

        for (size_t i = 0; i < this->conf.count; i++)
        {
            if (wl_pixels <= 0.0f) break;

            // Welle 1: Normaler Sinus-Winkel
            float angle1 = ((2.0f * PI * (float)i) / wl_pixels) + phase;
            float sine_val1 = (sinf(angle1) + 1.0f) / 2.0f; // 0.0 bis 1.0

            // Welle 2: Um exakt eine halbe Wellenlänge (+ PI) verschoben
            float angle2 = angle1 + PI;
            float sine_val2 = (sinf(angle2) + 1.0f) / 2.0f; // 0.0 bis 1.0

            // Lineares Ineinanderfließen der RGB-Kanäle
            // Da sine_val1 + sine_val2 immer exakt 1.0 ergibt, bleibt die Gesamthelligkeit stabil
            uint8_t r = (uint8_t)((rgb1.r * sine_val1) + (rgb2.r * sine_val2));
            uint8_t g = (uint8_t)((rgb1.g * sine_val1) + (rgb2.g * sine_val2));
            uint8_t b = (uint8_t)((rgb1.b * sine_val1) + (rgb2.b * sine_val2));

            // Pixel schreiben (Nutzt die RGB-Überladung deiner Library)
            this->strip->set_pixel(this->conf.idx_begin + i, r, g, b);
        }

        this->counter++;
    }
} // namespace bi_sine