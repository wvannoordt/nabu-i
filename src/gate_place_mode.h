#pragma once

#include <array>

#include "canvas.h"
#include "nabu.h"

namespace nbi
{
    struct gate_place_mode_t
    {
        bool enabled = false;
        std::array<nabu::operation, 4> ops
        {
            nabu::op_id,
            nabu::op_inv,
            nabu::op_or,
            nabu::op_and
        };
        int index = 0;
        
        void next_op()
        {
            if (enabled)
            {
                index++;
                index = index % ops.size();
            }
        }
        
        void on_lclick(const sf::Vector2f& pos, canvas_t& data)
        {
            
        }
        
        void toggle()
        {
            enabled = !enabled;
        }
    };
}