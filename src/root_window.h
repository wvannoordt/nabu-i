#pragma once

#include <memory>
#include <thread>
#include <map>

#include "style.h"
#include "nabu_instance.h"

namespace nbi
{
    struct root_window_t
    {
        using key_type          = decltype(sf::Keyboard::Escape);
        using mouse_button_type = decltype(sf::Mouse::Left);
        
        std::size_t frame = 0;
        style_t root_style;
        std::unique_ptr<sf::RenderWindow> window;
        std::map<key_type, bool>          key_pressed;
        std::map<mouse_button_type, bool> mouse_pressed;
        
        int width, height;
        sf::Vector2f focus_position;
        sf::Vector2i last_mouse_position;
        float px_per_unit = 20.0;
        
        root_window_t()
        {
            width  = sf::VideoMode::getDesktopMode().width*0.8;
            height = sf::VideoMode::getDesktopMode().height*0.8;
            window = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(width, height), "SFML works!"));
            
            focus_position = sf::Vector2f(0.5*width,0.5*height);
            // later: call this in a thread loop
            this->poll_loop();
        }
        
        void handle_event(const sf::Event& event)
        {
            if (event.type == sf::Event::Closed) on_close();
            if (event.type == sf::Event::KeyPressed)
            {
                //todo: handle this elegantly
                key_pressed[event.key.code] = true;
                if (event.key.code == sf::Keyboard::W && event.key.control) on_close();
            }
            if (event.type == sf::Event::KeyReleased) key_pressed[event.key.code] = false;
            if (event.type == sf::Event::MouseButtonPressed)
            {
                mouse_pressed[event.mouseButton.button] = true;
                set_last_mouse_position();
            }
            if (event.type == sf::Event::MouseButtonReleased)
            {
                mouse_pressed[event.mouseButton.button] = false;
                set_last_mouse_position();
            }
            if(event.type == sf::Event::MouseMoved)
            {
                if (mouse_pressed[sf::Mouse::Left])
                {
                    //todo: introduce world coordinates
                    sf::Vector2i dx = get_mouse_position() - last_mouse_position;
                    focus_position.x += dx.x;
                    focus_position.y += dx.y;
                    set_last_mouse_position();
                }
            }
        }
        
        sf::Vector2i get_mouse_position()
        {
            return sf::Mouse::getPosition(*window);
        }
        
        void set_last_mouse_position()
        {
            last_mouse_position = get_mouse_position();
        }
        
        void on_close()
        {
            window->close();
        }
        
        void tick()
        {
            frame++;
        }
        
        void render()
        {
            float r = 100.0;
            sf::CircleShape shape(r);
            shape.setPosition(focus_position.x - r, focus_position.y - r);
            shape.setFillColor(sf::Color::Green);
            window->clear(root_style.back_color);
            window->draw(shape);
            window->display();
        }
        
        void poll_loop()
        {
            
            //we don't exit this loop until this window is closed, so need a thread.
            while (window->isOpen())
            {
                sf::Event event;
                this->tick();
                while (window->pollEvent(event))
                {
                    this->handle_event(event);
                }
                this->render();
            }
        }
    };
}