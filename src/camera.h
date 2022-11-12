#pragma once

#include <SFML/Graphics.hpp>

namespace nbi
{
    struct camera_t
    {
        sf::Vector2f center;
        float px_per_unit;
        
        camera_t()
        {
            center = sf::Vector2f(0,0);
            px_per_unit = 1.0;
        }
        void increment_px(const sf::Vector2i& dx_px)
        {
            center.x += dx_px.x/px_per_unit;
            center.y += dx_px.y/px_per_unit;
        }
    };
}