#pragma once

#include <memory>
#include <thread>
#include <map>

#include <SFML/Graphics.hpp>

#include "style.h"
#include "camera.h"
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
        
        camera_t current_camera;
        sf::Vector2i last_mouse_position;
        
        root_window_t()
        {
            width  = sf::VideoMode::getDesktopMode().width*0.8;
            height = sf::VideoMode::getDesktopMode().height*0.8;
            window = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(width, height), "SFML works!"));
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
                set_last_mouse_position();
                if (mouse_pressed[sf::Mouse::Right])
                {
                    //todo: introduce world coordinates
                    sf::Vector2i dx = get_mouse_position() - last_mouse_position;
                    current_camera.increment_px(dx);
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
            sf::Transform screen_offset = sf::Transform::Identity;
            auto win_size = window->getSize();
            screen_offset.translate(0.5*(float)win_size.x, 0.5*(float)win_size.y);
            
            float r = 100.0;
            sf::CircleShape shape(r);
            
            sf::CircleShape shape2(r);
            shape.setFillColor(sf::Color::Green);
            
            window->clear(root_style.back_color);
            window->draw(shape, screen_offset);
            
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