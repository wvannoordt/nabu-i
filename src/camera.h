#pragma once

#include <SFML/Graphics.hpp>

#include <cmath>

namespace nbi
{
    struct camera_t
    {
        sf::Vector2f center;
        sf::Vector2f zoom_center;
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
            zoom_center = sf::Vector2f(0,0);
            center = sf::Vector2f(0,0);
            cam_trans = sf::Transform::Identity;
            cam_scale = sf::Transform::Identity;
            cam_scale.scale(px_per_unit(), px_per_unit());
        }
        
        void zoom_in()
        {
            float zprev = px_per_unit();
            zoom_factor += dz;
            cam_scale = sf::Transform::Identity;
            cam_scale.scale(px_per_unit(), px_per_unit());
            float znew = px_per_unit();
            sf::Vector2f dx = (center-zoom_center)/(znew-zprev);
            // print(dx.x, dx.y);
            // cam_trans = sf::Transform::Identity;
            // cam_trans.translate(center);
            // center += dx;
        }
        void zoom_out()
        {
            float zprev = px_per_unit();
            zoom_factor -= dz;
            cam_scale = sf::Transform::Identity;
            cam_scale.scale(px_per_unit(), px_per_unit());
            float znew = px_per_unit();
            sf::Vector2f dx = (center-zoom_center)/(znew-zprev);
            // print(dx.x, dx.y);
            // cam_trans = sf::Transform::Identity;
            // cam_trans.translate(center);
            // center += dx;
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
        
        void set_zoom_center(const sf::Vector2f& cen)
        {
            zoom_center = cen;
        }
        
        sf::Transform get_transform() const
        {
            return cam_scale * cam_trans;
        }
    };
}