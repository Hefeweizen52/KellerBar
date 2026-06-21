#include "sine.h"
#include <cmath>

namespace sine
{
    void sine_t::reset()
    {
        this->counter = 0;
        if (this->strip)
        {
            this->strip->clear(this->conf.idx_begin, this->conf.count);
        }
    }

    void sine_t::tick(uint32_t wavelength, uint32_t speed, HSL base_color)
    {
        if (!this->strip) return;

        // 1. Sicherheit geht vor: Werte strikt auf den 10-Bit-Bereich festnageln
        uint32_t wl_raw = constrain(wavelength, 0UL, 1023UL);
        uint32_t sp_raw = constrain(speed, 0UL, 1023UL);

        // 2. Wellenlänge (Wavelength) verarbeiten
        // Wenn der Slider ganz links auf 0 steht, nehmen wir automatisch die 
        // Gesamtlänge des Segments als Standard (eine komplette Welle über den Strip).
        float wl_pixels = (float)wl_raw;
        if (wl_raw == 0) 
        {
            wl_pixels = (float)this->conf.count;
        }

        // 3. Geschwindigkeit (Speed) skalieren
        // Da ein roher Wert von 1023 viel zu schnell für die Phase wäre, 
        // skalieren wir den Wert fließend herunter. 
        // Der Faktor 0.0002f sorgt dafür, dass 1023 eine schöne, schnelle Welle ergibt.
        float phase = (float)this->counter * (float)sp_raw * 0.0002f;

        // 4. Den Sinus über die LEDs jagen
        for (size_t i = 0; i < this->conf.count; i++)
        {
            // Failsafe: Falls wl_pixels durch irgendeinen dummen Zufall doch 0 ist, abbrechen
            if (wl_pixels <= 0.0f) break;

            // Winkelberechnung: (2 * PI * LED_Position / Wellenlänge_in_Pixeln) + Zeit_Verschiebung
            float angle = ((2.0f * PI * (float)i) / wl_pixels) + phase;

            // Sinus von [-1, 1] auf [0, 1] bringen
            float sine_val = (sinf(angle) + 1.0f) / 2.0f;

            // Farbe basierend auf dem Sinus-Ausschlag dimmen
            HSL led_color = base_color;
            led_color.l *= sine_val;

            // Pixel auf die Hardware schreiben
            this->strip->set_pixel(this->conf.idx_begin + i, led_color);
        }

        this->counter++;
    }
} // namespace sine