#pragma once

#include <SFML/Graphics.hpp>

#include "canvas.h"

namespace nbi
{
    struct view_t
    {
        sf::Vector2f center;
        sf::Vector2f last_px;
        sf::Vector2f zoom_center;
        float zoom_factor;
        sf::Transform cam_trans;
        sf::Transform win_trans;
        sf::Transform cam_rot;
        sf::Transform cam_scale;
        float zoom_sensitivity;
        int win_width, win_height;
        const sf::RenderWindow* window;
        
        view_t(){}
        view_t(const sf::RenderWindow& window_in)
        {
            window = &window_in;
            zoom_sensitivity = 1.5;
            zoom_factor = -20.0;
            zoom_center = sf::Vector2f(0,0);
            center = sf::Vector2f(0,0);
            cam_trans = sf::Transform::Identity;
            cam_scale = sf::Transform::Identity;
            cam_scale.scale(px_per_unit(), px_per_unit());
            win_width = window->getSize().x;
            win_height = window->getSize().y;
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

        void increment_px(const sf::Vector2f& dx_px)
        {
            center.x += (dx_px.x)/px_per_unit();
            center.y += (dx_px.y)/px_per_unit();
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
        
        sf::Vector2f get_mouse_coords(const sf::RenderWindow& window) const
        {
            sf::Vector2i px = sf::Mouse::getPosition(window);
            sf::Vector2f xx((float)px.x, (float)px.y);
            auto tr = this->get_transform().getInverse();
            xx = tr.transformPoint(xx);
            return xx;
        }
        
        void on_uscroll(const sf::Vector2f& pos, canvas_t& data)
        {
            zoom_in();
        }
        
        void on_dscroll(const sf::Vector2f& pos, canvas_t& data)
        {
            zoom_out();
        }
        
        void on_rdrag(const sf::Vector2f& pos, canvas_t& data)
        {
            auto tr  = this->get_transform();
            auto pos_px = tr.transformPoint(pos);
            auto dx_px = pos_px - last_px;
            increment_px(dx_px);
            last_px = pos_px;
        }
        
        void on_mouse_move(const sf::Vector2f& pos, canvas_t& data)
        {
            zoom_center = pos;
            last_px = this->get_transform().transformPoint(pos);
        }
    };
}