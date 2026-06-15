#include "control_state.h"

namespace pwm_handler
{
    void set_duty(uint8_t channel, uint16_t duty)
    {
        ledcWrite(channel, duty);
    }
};

namespace pixel_handler 
{
    std::vector<comfort_functions*> strips;
    std::vector<size_t> pixel_counts;

    void register_strip(comfort_functions *p_strip, size_t p_pixel_count)
    {
        strips.push_back(p_strip);
        pixel_counts.push_back(p_pixel_count);
    }
};