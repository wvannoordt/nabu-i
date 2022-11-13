#pragma once

#include <SFML/Graphics.hpp>

#include "nabu.h"

namespace nbi
{
    struct gate_handle_t
    {
        const nabu::gate_t* gate;
        sf::Vector2f pos;
        sf::CircleShape* in0;
        sf::CircleShape* in1;
        sf::CircleShape* out;
        
        gate_handle_t(){}
        gate_handle_t(const nabu::gate_t& gate_in)
        {
            gate = &gate_in;
            pos.x = 0;
            pos.y = 0;
        }
        
        void set_position(const sf::Vector2f& x)
        {
            
        }
    };
}