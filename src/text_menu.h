#pragma once

#include <SFML/Graphics.hpp>

#include "PTL.h"

#include "control_modes.h"
#include "assets.h"
#include "view.h"

namespace nbi
{
    struct text_menu_t
    {
        bool enabled = false;
        assets_t* assets;
        
        static constexpr control_mode mode_type() {return control_menu;}
        
        text_menu_t(){}
        text_menu_t(assets_t* assets_in)
        {
            assets = assets_in;
        }
        
        void enable()
        {            
            enabled = true;
        }
        
        void disable()
        {
            enabled = false;
        }
        
        void on_tick(const std::size_t& frame)
        {
            
        }
        
        void draw(sf::RenderWindow& window, const sf::Transform& trans)
        {
            if (enabled)
            {
                sf::Vector2f win_size(window.getSize().x, window.getSize().y);
                sf::RectangleShape background(win_size);
                sf::Color menu_back_color = assets->colors.menu_back_color;
                menu_back_color.a = 150;
                background.setFillColor(menu_back_color);
                window.draw(background);
            }
        }
    };
}