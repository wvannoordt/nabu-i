#pragma once

#include <SFML/Graphics.hpp>

#include "nabu.h"
#include "shape_buffer.h"

namespace nbi
{
    struct gate_shapes_t
    {
        sf::CircleShape in0, in1, out, aux;
        sf::ConvexShape body;
        sf::ConvexShape r0, r1, ro;
        
        sf::Vector2f get_center(const sf::CircleShape& sirc)
        {
            return sirc.getPosition() + sirc.getRadius()*sf::Vector2f(1,1);
        }
        
        gate_shapes_t(){}
        gate_shapes_t(const nabu::operation& op, const sf::Vector2f& pos, const bool& is_transparent = false)
        {
            float r      = 0.45;
            float r_aux  = 0.25;
            float r_rect = 0.05;
            aux = sf::CircleShape(r_aux);
            in0 = sf::CircleShape(r);
            in1 = sf::CircleShape(r);
            out = sf::CircleShape(r);
            sf::Uint8 alpha = 255;
            if (is_transparent) alpha = 155;
            sf::Color body_color(255,0,  0, alpha);
            sf::Color outl_color(0  ,0  ,0, alpha);
            auto set_style = [&outl_color, &body_color, &r](sf::Shape& s) -> void
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
            
            auto set_rstyle = [&outl_color](sf::Shape& s) -> void
            {
                s.setFillColor(outl_color);
            };
            set_rstyle(r0);
            set_rstyle(r1);
            set_rstyle(ro);
            
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
    };
    
    struct gate_placer_t
    {
        bool is_in_place_mode;
        int index;
        std::array<nabu::operation, 4> ops;
        bool has_last_placed_position;
        sf::Vector2f last_placed;
        gate_shapes_t placing_shapes;
        gate_placer_t()
        {
            is_in_place_mode = false;
            index = 0;
            ops[0] = nabu::op_id;
            ops[1] = nabu::op_inv;
            ops[2] = nabu::op_or;
            ops[3] = nabu::op_and;
            has_last_placed_position = false;
            last_placed = sf::Vector2f(0,0);
        }
        
        void handle_event(const sf::Event& event, shape_buffer_t& buffer, const sf::Vector2f& mouse_pos)
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::A)
                {
                    is_in_place_mode = !is_in_place_mode;
                }
                if (event.key.code == sf::Keyboard::Tab)
                {
                    if (is_in_place_mode)
                    {
                        ++index;
                        index = index % ops.size();
                    }
                }
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left && is_in_place_mode)
                {
                    nabu::operation op = ops[index];
                    placing_shapes = gate_shapes_t(op, mouse_pos);
                    buffer.copy_to_buffer(placing_shapes.r0);
                    buffer.copy_to_buffer(placing_shapes.ro);
                    if ((op == nabu::op_or) || (op == nabu::op_and)) buffer.copy_to_buffer(placing_shapes.r1);
                    buffer.copy_to_buffer(placing_shapes.in0);
                    if ((op == nabu::op_or) || (op == nabu::op_and)) buffer.copy_to_buffer(placing_shapes.in1);
                    buffer.copy_to_buffer(placing_shapes.out);
                    buffer.copy_to_buffer(placing_shapes.body);
                    if (op==nabu::op_inv) buffer.copy_to_buffer(placing_shapes.aux);

                    has_last_placed_position = true;
                    last_placed = mouse_pos;
                }
            }
        }
        
        void draw(sf::RenderWindow& win, const sf::Transform& trans, const sf::Vector2f& mouse_pos)
        {
            if (is_in_place_mode)
            {
                nabu::operation op = ops[index];
                placing_shapes = gate_shapes_t(op, mouse_pos);
                win.draw(placing_shapes.r0, trans);
                win.draw(placing_shapes.ro, trans);
                if ((op == nabu::op_or) || (op == nabu::op_and)) win.draw(placing_shapes.r1, trans);
                win.draw(placing_shapes.in0, trans);
                if ((op == nabu::op_or) || (op == nabu::op_and)) win.draw(placing_shapes.in1, trans);
                win.draw(placing_shapes.body, trans);
                if (op==nabu::op_inv) win.draw(placing_shapes.aux, trans);
                win.draw(placing_shapes.out, trans);
            }
        }
    };
}