#pragma once

#include <SFML/Graphics.hpp>

#include "nabu.h"
#include "assets.h"

namespace nbi
{
    struct edge_shapes_t
    {
        sf::Vector2f control;
        std::vector<sf::Vector2f> ins;
        std::vector<sf::RectangleShape> shapes;
        std::vector<float> angles;
        std::vector<sf::Vector2f> origins;
        
        edge_shapes_t(){}
        edge_shapes_t(assets_t* assets_in, const sf::Vector2f& control_in, const std::vector<sf::Vector2f>& ins_in)
        {
            control = control_in;
            ins = ins_in;
            shapes.resize(ins_in.size());
            angles.resize(shapes.size());
            origins.resize(shapes.size());
            for (int i = 0; i < shapes.size(); ++i)
            {
                connect(control, ins[i], shapes[i], 0.15, angles[i], origins[i]);
                shapes[i].setFillColor(assets_in->colors.off_state_color);
                shapes[i].setOutlineColor(assets_in->colors.border_color);
                shapes[i].setOutlineThickness(0.09);
            }
        }
        
        void connect(const sf::Vector2f& ini, const sf::Vector2f& trm, sf::RectangleShape& shape, float thick, float& angle, sf::Vector2f& orig)
        {
            const double pi = 3.1415926535;
            sf::Vector2f dx = trm - ini;
            float len = std::sqrt(dx.x*dx.x + dx.y*dx.y);
            shape.setSize(sf::Vector2f(len, thick));
            sf::Vector2f pt = ini + sf::Vector2f(0.0, 0.5*thick);
            float theta = std::atan2(dx.y, dx.x);
            shape.setPosition(pt);
            orig = pt;
            angle = theta*180.0/pi;
        }
        
        void draw(sf::RenderWindow& window, const sf::Transform& trans)
        {
            for (int i = 0; i < shapes.size(); ++i)
            {
                sf::Transform trans_loc = trans;
                trans_loc.rotate(angles[i], origins[i]);
                window.draw(shapes[i], trans_loc);
            }
        }
    };
}