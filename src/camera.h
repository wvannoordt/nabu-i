#pragma once

#include <SFML/Graphics.hpp>

#include <cmath>

namespace nbi
{
    struct camera_t
    {
        sf::Vector2f center;
        float zoom_factor;
        sf::Transform cam_trans;
        sf::Transform cam_rot;
        sf::Transform cam_scale;
        float dz;
        float zoom_sensitivity;
        
        camera_t()
        {
            zoom_sensitivity = 2.0;
            zoom_factor = 0.0;
            dz = 0.5;
            center = sf::Vector2f(0,0);
            cam_trans = sf::Transform::Identity;
            cam_scale = sf::Transform::Identity;
            cam_scale.scale(px_per_unit(), px_per_unit());
        }
        
        void zoom_in()
        {
            zoom_factor += dz;
            cam_scale = sf::Transform::Identity;
            cam_scale.scale(px_per_unit(), px_per_unit());
        }
        void zoom_out()
        {
            zoom_factor -= dz;
            cam_scale = sf::Transform::Identity;
            cam_scale.scale(px_per_unit(), px_per_unit());
        }
        
        float px_per_unit() const
        {
            const float k = 0.05;
            return 100.0*zoom_sensitivity*exp(k*zoom_factor);
        }
        void increment_px(const sf::Vector2i& dx_px)
        {
            center.x += ((float)dx_px.x)/px_per_unit();
            center.y += ((float)dx_px.y)/px_per_unit();
            cam_trans = sf::Transform::Identity;
            cam_trans.translate(center);
        }
        
        const sf::Transform& get_transform() const {return cam_trans;}
    };
}