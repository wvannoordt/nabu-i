#pragma once

#include <array>

#include "control_modes.h"
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
        int angle = 0;
        int angle_max = 8;
        
        static constexpr control_mode mode_type() {return control_gate_place;}
        
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
                preview.set_rotation(360.0*angle/(float)angle_max);
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
                preview.set_rotation(360.0*angle/(float)angle_max);
            }
        }
        
        void on_lclick(const sf::Vector2f& pos, canvas_t& data)
        {
            if (enabled)
            {
                data.add_gate(ops[index], last_pos, 360.0*angle/(float)angle_max);
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
        
        void rotate_preview(const int& right_left)
        {
            //right_left:  1 => right
            //right_left: -1 => left
            if (enabled)
            {
                angle += right_left + angle_max;
                angle %= angle_max;
                float theta = 360.0*angle/(float)angle_max;
                preview.set_rotation(theta);
            }
        }
        
        void draw(sf::RenderWindow& window, const sf::Transform& trans)
        {
            if (enabled)
            {
                preview.draw(window, trans);
            }
        }

        void enable()
        {
            enabled = true;
            preview = gate_shapes_t(ops[index], last_pos, assets);
        }
        
        void disable()
        {
            preview = gate_shapes_t(ops[index], last_pos, assets);
            enabled = false;
        }
    };
}