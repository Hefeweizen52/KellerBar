#include <Arduino.h>
#include "control_state.h"
#include "config.h"
#include "neon.h"
#include "sine.h"

namespace pwm_handler
{
    void set_duty(uint8_t channel, uint16_t duty)
    {
        ledcWrite(channel, duty);
    }
};

namespace pixel_handler
{
    std::vector<comfort_functions *> strips;
    std::vector<size_t> pixel_counts;
    std::vector<uint8_t> modes;
    std::vector<uint8_t> prev_modes;
    std::vector<HSL> colors;
    std::vector<float> bi_hues;
    std::vector<std::vector<uint32_t>> params;
    std::vector<std::vector<neon::tube_t>> tubes;
    std::vector<std::vector<sine::sine_t>> sines;
    std::vector<std::vector<bi_sine::bi_sine_t>> bi_sines;
    std::vector<std::vector<sparkle::sparkle_t>> sparkles;

    void tick()
    {
        for (uint8_t i = 0; i < strips.size(); i++)
        {
            comfort_functions *strip = strips.at(i);
            size_t num_leds = pixel_counts.at(i);
            uint8_t mode = modes.at(i);
            uint8_t prev_mode = prev_modes.at(i);
            HSL color = colors.at(i);
            float bi_hue = bi_hues.at(i);

            switch (mode)
            {
            case PIXEL_MODE_STATIC:
                strip->fill(0, num_leds, color);
                break;

            case PIXEL_MODE_SINE:
                // if (prev_mode != mode)
                // {
                //     strip->clear(num_leds);

                //     for (sine::sine_t &sine : sines.at(i))
                //         sine.reset();
                // }

                for (sine::sine_t &sine : sines.at(i))
                    sine.tick(params.at(i).at(INDEX_PARAM_SINE_WAVELENGTH), params.at(i).at(INDEX_PARAM_SINE_SPEED), color);

                break;

            case PIXEL_MODE_BI_SINE:
                for (bi_sine::bi_sine_t &bi_sine : bi_sines.at(i))
                    bi_sine.tick(params.at(i).at(INDEX_PARAM_SINE_WAVELENGTH), params.at(i).at(INDEX_PARAM_SINE_SPEED), color, HSL(bi_hue, color.s, color.l));

                break;

            case PIXEL_MODE_SPARKLE:
                if (prev_mode != mode)
                {
                    for (sparkle::sparkle_t &sparkle : sparkles.at(i))
                        sparkle.reset();
                }

                for (sparkle::sparkle_t &sparkle : sparkles.at(i))
                    sparkle.tick(params.at(i).at(INDEX_PARAM_SINE_SPEED), color);

                break;

            case PIXEL_MODE_NEON:
                if (prev_mode != mode)
                {
                    strip->clear(num_leds);

                    for (neon::tube_t &tube : tubes.at(i))
                        tube.reset();
                }

                for (neon::tube_t &tube : tubes.at(i))
                    tube.tick();

                break;

            case PIXEL_MODE_FILL_COUNT:
                strip->clear(num_leds);
                strip->fill(0, params.at(i).at(0), color);

            default:
                mode = PIXEL_MODE_STATIC;
                break;
            }

            prev_modes.at(i) = modes.at(i);
            strip->show();
        }
    }

    void register_strip(comfort_functions *p_strip, size_t p_pixel_count)
    {
        strips.push_back(p_strip);
        pixel_counts.push_back(p_pixel_count);
        modes.push_back(0);
        prev_modes.push_back(0);
        colors.push_back(HSL(0, 1.0, 0.5));
        bi_hues.push_back(20.0f);
        params.push_back(std::vector<uint32_t>(10));
        tubes.push_back(std::vector<neon::tube_t>(0));
        sines.push_back(std::vector<sine::sine_t>(0));
        bi_sines.push_back(std::vector<bi_sine::bi_sine_t>(0));
        sparkles.push_back(std::vector<sparkle::sparkle_t>(0));
    }

    void set_mode(uint8_t strip, uint8_t mode)
    {
        modes.at(strip) = mode;
        // Serial.printf("Set mode %u to strip %u\n", mode, strip);
    }

    uint8_t get_mode(uint8_t strip)
    {
        return modes.at(strip);
    }

    void set_color(uint8_t strip, RGB color)
    {
        colors.at(strip) = color.toHSL();
    }

    void set_color(uint8_t strip, HSL color)
    {
        colors.at(strip) = color;
    }

    void set_color_packed(uint8_t strip, uint32_t packed)
    {
        colors.at(strip).h = (((packed >> 20) & 0x3FF) * 360.0f) / 1023.0f;
        colors.at(strip).s = ((packed >> 10) & 0x3FF) / 1023.0f;
        colors.at(strip).l = ((packed & 0x3FF) / 1023.0f);
    }

    void register_tube(uint8_t strip, neon::tube_t tube)
    {
        tubes.at(strip).push_back(tube);
    }

    void register_sine(uint8_t strip, sine::sine_t sine)
    {
        sines.at(strip).push_back(sine);
    }

    void register_bi_sine(uint8_t strip, bi_sine::bi_sine_t bi_sine)
    {
        bi_sines.at(strip).push_back(bi_sine);
    }

    void register_sparkle(uint8_t strip, sparkle::sparkle_t sparkle)
    {
        sparkles.at(strip).push_back(sparkle);
    }

    void set_param(uint8_t strip, uint8_t idx, uint32_t value)
    {
        params.at(strip).at(idx) = value;

        if (idx == INDEX_PARAM_BI_HUE)
        {
            bi_hues.at(strip) = (value * 360.0f) / 1023.0f;
        }
    }

};
