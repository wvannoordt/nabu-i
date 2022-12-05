#pragma once

#include <SFML/Graphics.hpp>

#include "control_modes.h"
#include "canvas.h"
#include "assets.h"

namespace nbi
{
    struct select_mode_t
    {
        bool enabled = false;
        sf::Vector2f last_pos;
        
        static constexpr control_mode mode_type() {return control_select;}
        
        select_mode_t(){}
        void enable()  { enabled = true; }
        void disable() { enabled = false; }
        
        void on_lclick(const sf::Vector2f& pos, canvas_t& data)
        {
            last_pos = pos;
            if (enabled)
            {
                
            }
        }
        
        void on_mouse_move(const sf::Vector2f& pos, canvas_t& data)
        {
            last_pos = pos;
            if (enabled)
            {
                
            }
        }
    };
}