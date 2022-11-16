#pragma once

#include <SFML/Graphics.hpp>

#include "nabu.h"
#include "shape_buffer.h"

namespace nbi
{
    struct gate_shapes_t
    {
        sf::CircleShape in0, in1, out;
        sf::ConvexShape body;
        
        gate_shapes_t(){}
        gate_shapes_t(const nabu::operation& op, const sf::Vector2f& pos)
        {
            float r = 0.5;
            in0 = sf::CircleShape(r);
            in1 = sf::CircleShape(r);
            out = sf::CircleShape(r);
            auto set_style = [&](sf::CircleShape& s) -> void
            {
                s = sf::CircleShape(r);
                s.setOutlineThickness(0.05*r);
                s.setFillColor(sf::Color::Red);
                s.setOutlineColor(sf::Color::Black);
            };
            set_style(in0);
            set_style(in1);
            set_style(out);
            
            switch(op)
            {
                case nabu::op_id:
                {
                    in0.setPosition(pos.x - r - 1.8, pos.y - r);
                    out.setPosition(pos.x - r + 1.8, pos.y - r);
                    break;
                }
                case nabu::op_inv:
                {
                    in0.setPosition(pos.x - r - 1.8, pos.y - r);
                    out.setPosition(pos.x - r + 1.8, pos.y - r);
                    break;
                }
                case nabu::op_or:
                {
                    in0.setPosition(pos.x - r - 1.8, pos.y - r - 1.0);
                    in1.setPosition(pos.x - r - 1.8, pos.y - r + 1.0);
                    out.setPosition(pos.x - r + 1.8, pos.y - r);
                    break;
                }
                case nabu::op_and:
                {
                    in0.setPosition(pos.x - r - 1.8, pos.y - r - 1.0);
                    in1.setPosition(pos.x - r - 1.8, pos.y - r + 1.0);
                    out.setPosition(pos.x - r + 1.8, pos.y - r);
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
                    last_placed = mouse_pos;
                    has_last_placed_position = true;
                    float r = 0.5;
                    auto p = buffer.add<sf::CircleShape>(r);
                    p->setOutlineThickness(0.05*r);
                    p->setPosition(mouse_pos.x - r, mouse_pos.y - r);
                    if (ops[index] == nabu::op_id)  p->setFillColor(sf::Color::Red);
                    if (ops[index] == nabu::op_inv) p->setFillColor(sf::Color::Green);
                    if (ops[index] == nabu::op_or)  p->setFillColor(sf::Color::Blue);
                    if (ops[index] == nabu::op_and) p->setFillColor(sf::Color::Yellow);
                    p->setOutlineColor(sf::Color::Black);
                }
            }
        }
        
        void draw(sf::RenderWindow& win, const sf::Transform& trans, const sf::Vector2f& mouse_pos)
        {
            if (is_in_place_mode)
            {
                nabu::operation op = ops[index];
                placing_shapes = gate_shapes_t(op, mouse_pos);
                win.draw(placing_shapes.in0, trans);
                if ((op == nabu::op_or) || (op == nabu::op_and)) win.draw(placing_shapes.in1, trans);
                win.draw(placing_shapes.out, trans);
            }
        }
    };
}