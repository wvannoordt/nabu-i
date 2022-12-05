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
        gate_shapes_t* hover_gate_shapes = nullptr;
        sf::Shape* hover_shape;
        sf::Vector2f last_pos;
        assets_t* assets;
        
        static constexpr control_mode mode_type() {return control_select;}
        
        select_mode_t(){}
        select_mode_t(assets_t* assets_in)
        {
            assets = assets_in;
        }
        
        void enable()  { enabled = true;  }
        void disable() { enabled = false; }
        
        void on_lclick(const sf::Vector2f& pos, canvas_t& data)
        {
            last_pos = pos;
            if (enabled)
            {
                
            }
        }
        
        void on_rclick(const sf::Vector2f& pos, canvas_t& data)
        {
            last_pos = pos;
            if (enabled)
            {
                if (hover_shape != nullptr && hover_gate_shapes != nullptr)
                {
                    if  (hover_shape == &hover_gate_shapes->in0)
                    {
                        data.shape_to_gate.at(hover_gate_shapes)->toggle_input(0);
                    }
                    if  (hover_shape == &hover_gate_shapes->in1)
                    {
                        data.shape_to_gate.at(hover_gate_shapes)->toggle_input(1);
                    }
                }
            }
        }
        
        void on_mouse_move(const sf::Vector2f& pos, canvas_t& data)
        {
            last_pos = pos;
            if (enabled)
            {
                gate_shapes_t* new_hover;
                sf::Shape* selected_shape;
                
                //slow for now
                if (data.get_shape_at_cursor(pos, new_hover, selected_shape))
                {
                    if (hover_gate_shapes != nullptr && new_hover != hover_gate_shapes) hover_gate_shapes->deselect(assets);
                    hover_gate_shapes = new_hover;
                    hover_shape = selected_shape;
                    hover_gate_shapes->preselect(assets, selected_shape);
                }
                else
                {
                    if (hover_gate_shapes != nullptr) hover_gate_shapes->deselect(assets);
                    hover_gate_shapes = nullptr;
                    hover_shape = nullptr;
                }
            }
        }
        
        void draw(sf::RenderWindow& window, const sf::Transform& trans)
        {
            
        }
    };
}