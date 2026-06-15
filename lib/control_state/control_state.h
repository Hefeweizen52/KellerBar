#ifndef CONTROL_STATE_H
#define CONTROL_STATE_H

#include <Arduino.h>
#include <vector>
#include "comfort_functions.hpp" // Wichtig: Basisklasse inkludieren

namespace pwm_handler
{
    void set_duty(uint8_t channel, uint16_t duty);
};

namespace pixel_handler 
{
    extern std::vector<comfort_functions*> strips;
    extern std::vector<size_t> pixel_counts;

    void register_strip(comfort_functions* p_strip, size_t p_pixel_count);
};

#endif //CONTROL_STATE_H