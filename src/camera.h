#pragma once

#include <SFML/Graphics.hpp>

namespace nbi
{
    struct camera_t
    {
        sf::Vector2f center;
        float px_per_unit;
        sf::Transform cam_trans;
        sf::Transform cam_rot;
        sf::Transform cam_scale;
        
        camera_t()
        {
            center = sf::Vector2f(0,0);
            px_per_unit = 1.0;
            cam_trans = sf::Transform::Identity;
        }
        
        void increment_px(const sf::Vector2i& dx_px)
        {
            center.x += dx_px.x/px_per_unit;
            center.y += dx_px.y/px_per_unit;
            cam_trans = sf::Transform::Identity;
            cam_trans.translate(center);
        }
        
        const sf::Transform& get_transform() const {return cam_trans;}
    };
}