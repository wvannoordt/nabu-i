#pragma once

#include "shape_buffer.h"
#include "view.h"
#include "nabu.h"

namespace nbi
{
    //holds all data that the user is exposed to
    struct canvas_t
    {
        assets_t* assets;
        shape_buffer_t edge_layer;
        shape_buffer_t gate_layer;
        nabu::machine_t machine;
        canvas_t(){}
        canvas_t(assets_t* assets_in)
        {
            assets = assets_in;
        }
        
        void draw(sf::RenderWindow& window, const sf::Transform& trans)
        {
            edge_layer.draw(window, trans);
            gate_layer.draw(window, trans);
        }
        
        void add_gate(const sf::Vector2f& position, const nabu::operation& oper)
        {
            
        }
    };
}