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
        sf::Transform win_trans;
        sf::Transform cam_rot;
        sf::Transform cam_scale;
        float zoom_sensitivity;
        int win_width, win_height;
        
        camera_t(){}
        camera_t(const int& wid, const int& hei)
        {
            zoom_sensitivity = 1.5;
            zoom_factor = -20.0;
            zoom_center = sf::Vector2f(0,0);
            center = sf::Vector2f(0,0);
            cam_trans = sf::Transform::Identity;
            cam_scale = sf::Transform::Identity;
            cam_scale.scale(px_per_unit(), px_per_unit());
            win_width = wid;
            win_height = hei;
            win_trans.translate(0.5*win_width, 0.5*win_height);
        }
        
        void zoom_in()
        {
            float zprev = px_per_unit();
            zoom_factor += zoom_sensitivity;
            cam_scale = sf::Transform::Identity;
            cam_scale.scale(px_per_unit(), px_per_unit());
            float znew = px_per_unit();
            float theta = zprev/znew;
            sf::Vector2f new_center;
            //I can't quite account for the neative sign here...
            new_center.x = theta*center.x - (1.0 - theta)*zoom_center.x;
            new_center.y = theta*center.y - (1.0 - theta)*zoom_center.y;
            center = new_center;
            cam_trans = sf::Transform::Identity;
            cam_trans.translate(center);
        }
        void zoom_out()
        {
            float zprev = px_per_unit();
            zoom_factor -= zoom_sensitivity;
            cam_scale = sf::Transform::Identity;
            cam_scale.scale(px_per_unit(), px_per_unit());
            float znew = px_per_unit();
            float theta = zprev/znew;
            sf::Vector2f new_center;
            //I can't quite account for the neative sign here...
            new_center.x = theta*center.x - (1.0 - theta)*zoom_center.x;
            new_center.y = theta*center.y - (1.0 - theta)*zoom_center.y;
            center = new_center;
            cam_trans = sf::Transform::Identity;
            cam_trans.translate(center);
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
            return win_trans * cam_scale * cam_trans;
        }
    };
}