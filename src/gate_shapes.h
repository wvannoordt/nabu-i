#pragma once

#include <SFML/Graphics.hpp>
#include "nabu.h"

namespace nbi
{
    struct gate_shapes_t
    {
        sf::CircleShape in0, in1, out, aux;
        sf::ConvexShape body;
        sf::ConvexShape r0, r1, ro;
        nabu::operation op;
        assets_t* assets;
        float r      = 0.45;
        float r_aux  = 0.25;
        float r_rect = 0.05;
        
        sf::Vector2f get_center(const sf::CircleShape& sirc)
        {
            return sirc.getPosition() + sirc.getRadius()*sf::Vector2f(1,1);
        }
        
        gate_shapes_t(){}
        gate_shapes_t(const nabu::operation& op_in, const sf::Vector2f& pos, assets_t* assets_in)
        {
            assets = assets_in;
            op = op_in;
            aux = sf::CircleShape(r_aux);
            in0 = sf::CircleShape(r);
            in1 = sf::CircleShape(r);
            out = sf::CircleShape(r);
            sf::Uint8 alpha = 255;
            sf::Color body_color = assets->colors.gate_color;
            sf::Color node_color = assets->colors.bad_state_color;
            sf::Color outl_color = assets->colors.border_color;
            auto set_style = [&](sf::Shape& s) -> void
            {
                s.setOutlineThickness(0.1*r);
                s.setFillColor(body_color);
                s.setOutlineColor(outl_color);
            };
            set_style(in0);
            set_style(in1);
            set_style(out);
            set_style(aux);
            set_style(body);
            in0.setFillColor(node_color);
            in1.setFillColor(node_color);
            out.setFillColor(node_color);
            
            auto set_rstyle = [&outl_color](sf::Shape& s) -> void
            {
                s.setFillColor(outl_color);
            };
            set_rstyle(r0);
            set_rstyle(r1);
            set_rstyle(ro);
            set_position(pos);
        }
        
        void set_position(const sf::Vector2f& pos)
        {
            auto create_connection = [&](sf::ConvexShape& rect, const sf::Vector2f& p1, const sf::Vector2f& p2) -> void
            {
                sf::Vector2f dx(p2.y-p1.y, p1.x-p2.x);
                float norm = -std::sqrt(dx.x*dx.x + dx.y*dx.y);
                dx /= norm;
                rect.setPointCount(4);
                rect.setPoint(0, p1 + r_rect*dx);
                rect.setPoint(1, p2 + r_rect*dx);
                rect.setPoint(2, p2 - r_rect*dx);
                rect.setPoint(3, p1 - r_rect*dx);
            };
            //TODO: make these not look like complete garbage
            switch(op)
            {
                case nabu::op_id:
                {
                    in0.setPosition(pos.x - r - 2.8, pos.y - r);
                    out.setPosition(pos.x - r + 2.8, pos.y - r);
                    body.setPointCount(3);
                    body.setPoint(0, pos + sf::Vector2f(-1.0,  1.0));
                    body.setPoint(1, pos + sf::Vector2f( 1.0,  0.0));
                    body.setPoint(2, pos + sf::Vector2f(-1.0, -1.0));
                    create_connection(r0, get_center(in0), get_center(in0)+sf::Vector2f(2.1, 0.0));
                    create_connection(ro, get_center(out) - sf::Vector2f(2.1, 0.0), get_center(out));
                    break;
                }
                case nabu::op_inv:
                {
                    in0.setPosition(pos.x - r - 2.8, pos.y - r);
                    out.setPosition(pos.x - r + 2.8, pos.y - r);
                    aux.setPosition(pos + sf::Vector2f( 1.0,  0.0) - sf::Vector2f(r_aux, r_aux));
                    body.setPointCount(3);
                    body.setPoint(0, pos + sf::Vector2f(-1.0,  1.0));
                    body.setPoint(1, pos + sf::Vector2f( 1.0,  0.0));
                    body.setPoint(2, pos + sf::Vector2f(-1.0, -1.0));
                    create_connection(r0, get_center(in0), get_center(in0)+sf::Vector2f(2.1, 0.0));
                    create_connection(ro, get_center(out) - sf::Vector2f(2.1, 0.0), get_center(out));
                    break;
                }
                case nabu::op_or:
                {
                    in0.setPosition(pos.x - r - 2.8, pos.y - r - 1.0);
                    in1.setPosition(pos.x - r - 2.8, pos.y - r + 1.0);
                    out.setPosition(pos.x - r + 2.8, pos.y - r);
                    body.setPointCount(13);
                    
                    body.setPoint(1,  pos + sf::Vector2f(-0.7,  0.9));
                    body.setPoint(0,  pos + sf::Vector2f(-0.4,  0.4));
                    
                    body.setPoint(2,  pos + sf::Vector2f(-1.0,  1.3));
                    body.setPoint(3,  pos + sf::Vector2f(-0.5,  1.2));
                    body.setPoint(4,  pos + sf::Vector2f( 0.3,  0.9));
                    body.setPoint(5,  pos + sf::Vector2f( 0.8,  0.5));
                    body.setPoint(6,  pos + sf::Vector2f( 1.0,  0.0));
                    body.setPoint(7,  pos + sf::Vector2f( 0.8, -0.5));
                    body.setPoint(8,  pos + sf::Vector2f( 0.3, -0.9));
                    body.setPoint(9,  pos + sf::Vector2f(-0.5, -1.2));
                    body.setPoint(10, pos + sf::Vector2f(-1.0, -1.3));
                    
                    body.setPoint(12, pos + sf::Vector2f(-0.4, -0.4));
                    body.setPoint(11, pos + sf::Vector2f(-0.7, -0.9));
                    
                    create_connection(r0, get_center(in0), get_center(in0)+sf::Vector2f(2.1, 0.0));
                    create_connection(r1, get_center(in1), get_center(in1)+sf::Vector2f(2.1, 0.0));
                    create_connection(ro, get_center(out) - sf::Vector2f(2.1, 0.0), get_center(out));
                    break;
                }
                case nabu::op_and:
                {
                    in0.setPosition(pos.x - r - 2.8, pos.y - r - 1.0);
                    in1.setPosition(pos.x - r - 2.8, pos.y - r + 1.0);
                    out.setPosition(pos.x - r + 2.8, pos.y - r);
                    body.setPointCount(9);
                    body.setPoint(0, pos + sf::Vector2f(-1.0,  1.3));
                    body.setPoint(1, pos + sf::Vector2f(-0.5,  1.2));
                    body.setPoint(2, pos + sf::Vector2f( 0.3,  0.9));
                    body.setPoint(3, pos + sf::Vector2f( 0.8,  0.5));
                    body.setPoint(4, pos + sf::Vector2f( 1.0,  0.0));
                    body.setPoint(5, pos + sf::Vector2f( 0.8, -0.5));
                    body.setPoint(6, pos + sf::Vector2f( 0.3, -0.9));
                    body.setPoint(7, pos + sf::Vector2f(-0.5, -1.2));
                    body.setPoint(8, pos + sf::Vector2f(-1.0, -1.3));
                    
                    create_connection(r0, get_center(in0), get_center(in0)+sf::Vector2f(2.1, 0.0));
                    create_connection(r1, get_center(in1), get_center(in1)+sf::Vector2f(2.1, 0.0));
                    create_connection(ro, get_center(out) - sf::Vector2f(2.1, 0.0), get_center(out));
                    break;
                }
            }
        }
        
        void draw(sf::RenderWindow& window, const sf::Transform& trans)
        {
            window.draw(r0, trans);
            window.draw(ro, trans);
            if ((op == nabu::op_or) || (op == nabu::op_and)) window.draw(r1, trans);
            window.draw(in0, trans);
            if ((op == nabu::op_or) || (op == nabu::op_and)) window.draw(in1, trans);
            window.draw(body, trans);
            if (op==nabu::op_inv) window.draw(aux, trans);
            window.draw(out, trans);
        }
        
        void add_to_buffer(shape_buffer_t& buffer)
        {
            buffer.copy_to_buffer(r0);
            buffer.copy_to_buffer(ro);
            if ((op == nabu::op_or) || (op == nabu::op_and)) buffer.copy_to_buffer(r1);
            buffer.copy_to_buffer(in0);
            if ((op == nabu::op_or) || (op == nabu::op_and)) buffer.copy_to_buffer(in1);
            buffer.copy_to_buffer(body);
            if (op==nabu::op_inv) buffer.copy_to_buffer(aux);
            buffer.copy_to_buffer(out);
        }
    };
}