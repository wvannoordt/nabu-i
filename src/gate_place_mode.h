#pragma once

#include <array>

#include "canvas.h"
#include "nabu.h"
#include "gate_shapes.h"

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
        assets_t* assets;
        gate_shapes_t preview;
        sf::Vector2f last_pos;
        
        gate_place_mode_t(){}
        gate_place_mode_t(assets_t* assets_in)
        {
            last_pos = sf::Vector2f(0,0);
            assets = assets_in;
        }
        
        void next_op()
        {
            if (enabled)
            {
                index++;
                index = index % ops.size();
                preview = gate_shapes_t(ops[index], last_pos, assets);
            }
        }
        
        void prev_op()
        {
            if (enabled)
            {
                index--;
                index += ops.size();
                index = index % ops.size();
                preview = gate_shapes_t(ops[index], last_pos, assets);
            }
        }
        
        void on_lclick(const sf::Vector2f& pos, canvas_t& data)
        {
            if (enabled)
            {
                data.add_gate(ops[index], last_pos);
            }
        }
        
        void on_mouse_move(const sf::Vector2f& pos, canvas_t& data)
        {
            last_pos = pos;
            if (enabled)
            {
                preview.set_position(pos);
            }
        }
        
        void draw(sf::RenderWindow& window, const sf::Transform& trans)
        {
            if (enabled)
            {
                preview.draw(window, trans);
            }
        }
        
        void toggle()
        {
            preview = gate_shapes_t(ops[index], last_pos, assets);
            enabled = !enabled;
        }
        
        void disable()
        {
            enabled = false;
        }
    };
}