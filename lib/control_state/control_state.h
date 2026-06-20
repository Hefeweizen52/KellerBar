#ifndef CONTROL_STATE_H
#define CONTROL_STATE_H

#include <Arduino.h>
#include <vector>
#include "comfort_functions.hpp" // Wichtig: Basisklasse inkludieren
#include "neon.h"

namespace pwm_handler
{
    void set_duty(uint8_t channel, uint16_t duty);
};

namespace pixel_handler 
{
    extern std::vector<comfort_functions*> strips;
    extern std::vector<size_t> pixel_counts;
    extern std::vector<uint8_t> modes;
    extern std::vector<uint8_t> prev_modes;
    extern std::vector<HSL> colors;
    extern std::vector<HSL> bi_colors;
    extern std::vector<std::vector<uint32_t>> params;
    extern std::vector<std::vector<neon::tube_t>> tubes;

    void tick();

    void register_strip(comfort_functions* p_strip, size_t p_pixel_count);

    void set_mode(uint8_t strip, uint8_t mode);

    uint8_t get_mode(uint8_t strip);

    void set_color(uint8_t strip, RGB color);

    void set_color(uint8_t strip, HSL color);

    void set_color_packed(uint8_t strip, uint32_t packed);

    void register_tube(uint8_t strip, neon::tube_t tube);

    void set_param(uint8_t strip, uint8_t idx, uint32_t value);
};

#endif //CONTROL_STATE_H