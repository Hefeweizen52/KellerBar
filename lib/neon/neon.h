#ifndef NEON_H
#define NEON_H

#define PROBABILITY_BROKEN 0.5F
#define PROBABILITY_FLICKER 0.5F
#define MIN_TICKS_INITIAL_DELAY 0
#define MAX_TICKS_INITIAL_DELAY 50
#define MIN_IGNITIONS 3
#define MAX_IGNITIONS 10
#define MIN_TICKS_IGNITION_DELAY 3
#define MAX_TICKS_IGNITION_DELAY 8
#define MAX_HUE_DELTA_DEGREE 8
#define EDGE_WIDTH 5

#include <Arduino.h>
#include <comfort_functions.hpp>
#include <vector>

namespace neon
{
    enum TUBE_STATE
    {
        HEATING,
        STARTING,
        ON
    };

    struct tube_config_t
    {
        size_t idx_begin, count;
        comfort_functions *strip;

        tube_config_t(size_t idx_begin, size_t count) : idx_begin(idx_begin), count(count) {};

        tube_config_t() = default;
    };

    struct tube_t
    {
        tube_config_t conf;
        TUBE_STATE state, prev_state;
        bool is_broken = false;
        bool is_flickering = false;

        uint32_t counter = 0;

        uint8_t ticks_initial_delay;

        uint8_t num_ignitions;
        uint8_t ticks_ignition_delay;

        HSL color;
        HSL color_edge;

        comfort_functions *strip;

        tube_t() = default;

        tube_t(tube_config_t tube_config, comfort_functions *strip) : conf(tube_config), strip(strip) {};

        /**
         * Reset tube parameters anc clear
         */
        void reset();

        /**
         * Do an animation cycle
         */
        void tick();

        /**
         * Draw the edges with a gradient
         */
        void draw_edges();
    };

    inline HSL BASE_COLOR = HSL(36.0f, 0.60f, 0.5f);
    inline HSL EDGE_COLOR = HSL(25.0f, 0.65f, 0.15f);

};

#endif // NEON_H