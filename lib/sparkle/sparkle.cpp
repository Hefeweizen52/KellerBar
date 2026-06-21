#include "sparkle.h"

namespace sparkle
{
    void sparkle_t::reset()
    {
        this->stars.clear();

        size_t star_count = this->conf.count / PIXELS_PER_STAR;

        for (size_t i = 0; i < star_count; i++)
        {
            star_t star;
            star.counter = 0;
            star.delay = random(MIN_SPARKLE_DELAY, MAX_SPARKLE_DELAY + 1);
            star.pos = this->conf.idx_begin + random(0, this->conf.count);
            
            // Sterne starten im Hintergrund-Modus (Dormant)
            star.l = 0.0f; 

            stars.push_back(star);
        }

        this->counter = 0;
        this->prev_counter = 0;
    }

    void sparkle_t::tick(uint32_t n_ticks, HSL base_color)
    {
        // 1. Hintergrund in JEDEM Frame frisch zeichnen
        this->strip->fill(this->conf.idx_begin, this->conf.count, base_color);

        // 2. Slider-Wert (0 - 1023) in eine feine Fade-Geschwindigkeit umrechnen
        // 0 -> langsames Ausglimmen (0.01) | 1023 -> blitzschnelles Funkeln (0.15)
        uint32_t speed_raw = constrain(n_ticks, 0UL, 1023UL);
        float fade_speed = 0.001f + ((float)speed_raw / 1023.0f) * 0.005f;

        // 3. Alle Sterne berechnen und drüberzeichnen
        for (star_t &star : stars)
        {
            if (star.counter < star.delay)
            {
                // Warteraum: Stern ist noch inaktiv und schläft
                star.counter++;
            }
            else
            {
                // Stern ist aktiv!
                if (star.counter == star.delay)
                {
                    // Erster Frame nach dem Schlafen: BOOM! Volles Aufblitzen auf Weiß
                    star.l = 1.0f;
                }
                else
                {
                    // Folgende Frames: Langsam abdunkeln
                    star.l -= fade_speed;
                }

                // Nur zeichnen, wenn wir heller als das Hintergrundlicht sind
                if (star.l > base_color.l)
                {
                    this->strip->set_pixel(star.pos, HSL(base_color.h, base_color.s, star.l));
                    star.counter++;
                }
                else
                {
                    // Fadeout beendet -> Stern komplett zurücksetzen für das nächste Mal
                    star.counter = 0;
                    star.delay = random(MIN_SPARKLE_DELAY, MAX_SPARKLE_DELAY + 1);
                    star.pos = this->conf.idx_begin + random(0, this->conf.count);
                    star.l = 0.0f;
                }
            }
        }

        this->counter++;
    }
};