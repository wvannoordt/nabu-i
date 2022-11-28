#pragma once

#include <map>
#include <SFML/Graphics.hpp>

namespace nbi
{
    struct control_state_t
    {
        using key_type          = decltype(sf::Keyboard::Escape);
        using mouse_button_type = decltype(sf::Mouse::Left);
        
        std::map<key_type, bool>          key_pressed;
        std::map<mouse_button_type, bool> mouse_pressed;
        
        sf::Vector2i last_mouse_position;
        sf::Vector2f last_mouse_coords;
    };
}