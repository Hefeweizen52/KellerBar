#include "neon.h"
#include "comfort_functions.hpp"

namespace neon
{
    void tube_t::reset()
    {
        this->strip->clear(this->conf.idx_begin, this->conf.count);

        this->state = TUBE_STATE::HEATING;
        this->prev_state = TUBE_STATE::HEATING;

        this->counter = 0;

        this->is_broken = random(0, uint8_t(1.0F / PROBABILITY_BROKEN)) == 0;
        this->is_flickering = random(0, uint8_t(1.0F / PROBABILITY_FLICKER)) == 0;
        this->num_ignitions = random(MIN_IGNITIONS, MAX_IGNITIONS + 1);
        this->ticks_initial_delay = random(MIN_TICKS_INITIAL_DELAY, MAX_TICKS_INITIAL_DELAY + 1);
        this->ticks_ignition_delay = random(MIN_TICKS_IGNITION_DELAY, MAX_TICKS_IGNITION_DELAY + 1);

        this->color = BASE_COLOR;
        this->color.h += random(-MAX_HUE_DELTA_DEGREE, MAX_HUE_DELTA_DEGREE + 1);

        this->color_edge = EDGE_COLOR;
        this->color_edge.l = 0.0f;
    };

    void tube_t::draw_edges()
    {
        for (size_t i = 0; i < EDGE_WIDTH; i++)
        {
            float fade_factor = 1.0f - (0.75f * (float)i / (EDGE_WIDTH - 1));

            HSL graded_color = color_edge;
            graded_color.l *= fade_factor;

            this->strip->set_pixel(this->conf.idx_begin + i, graded_color);
            this->strip->set_pixel(this->conf.idx_begin + this->conf.count - 1 - i, graded_color);
        }
    };

    void tube_t::tick()
    {
        switch (this->state)
        {
        case TUBE_STATE::HEATING:
        {
            // initial wait before igniting
            if (this->counter < this->ticks_initial_delay)
                break;

            if (this->color_edge.l < EDGE_COLOR.l)
            {
                this->color_edge.l += 0.05f;
                // ränder setzen...
            }
            else if (this->color_edge.l >= EDGE_COLOR.l)
            {
                this->state = STARTING;
            }

            this->draw_edges();

            break;
        };

        case TUBE_STATE::STARTING:
        {
            this->draw_edges();

            if (this->prev_state != TUBE_STATE::STARTING)
                this->color.l = 0.0f;

            // ignite the tube with a ramp
            if (this->counter >= this->ticks_ignition_delay)
            {
                if (this->color.l < BASE_COLOR.l)
                {
                    this->color.l += 0.25;
                    this->strip->fill(this->conf.idx_begin + EDGE_WIDTH, this->conf.count - 2 * EDGE_WIDTH, this->color);
                }
                else
                {
                    this->counter = 0;
                    this->num_ignitions--;
                    this->color.l = 0.1;
                }
            }

            if (num_ignitions == 0)
            {
                this->state = TUBE_STATE::ON;
                break;
            }
            else
            {
                // wait between each blink
                if (this->counter < this->ticks_ignition_delay)
                {
                    this->strip->clear(this->conf.idx_begin + EDGE_WIDTH, this->conf.count - 2 * EDGE_WIDTH);
                }
            }

            break;
        };

        case TUBE_STATE::ON:
        {
            this->color.l = 0.5f;

            this->draw_edges();
            this->strip->fill(this->conf.idx_begin + EDGE_WIDTH, this->conf.count - 2 * EDGE_WIDTH, this->color);
            break;
        };
        };

        if (this->prev_state != this->state)
            this->counter = 0;
        else
            this->counter++;

        this->prev_state = this->state;

        this->strip->show();
    };
};