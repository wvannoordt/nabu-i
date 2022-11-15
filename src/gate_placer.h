#pragma once

#include <SFML/Graphics.hpp>

#include "nabu.h"
#include "shape_buffer.h"

namespace nbi
{
    struct gate_placer_t
    {
        bool is_in_place_mode;
        int index;
        std::array<nabu::operation, 4> ops;
        bool has_last_placed_position;
        sf::Vector2f last_placed;
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
                float r = 0.5;
                sf::CircleShape shp(r);
                shp.setOutlineThickness(0.05*r);
                shp.setPosition(mouse_pos.x - r, mouse_pos.y - r);
                if (ops[index] == nabu::op_id)  shp.setFillColor(sf::Color::Red);
                if (ops[index] == nabu::op_inv) shp.setFillColor(sf::Color::Green);
                if (ops[index] == nabu::op_or)  shp.setFillColor(sf::Color::Blue);
                if (ops[index] == nabu::op_and) shp.setFillColor(sf::Color::Yellow);
                shp.setOutlineColor(sf::Color::Black);
                win.draw(shp, trans);
            }
        }
    };
}