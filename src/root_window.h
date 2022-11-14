#pragma once

#include <memory>
#include <thread>
#include <map>

#include <SFML/Graphics.hpp>

#include "nabu.h"

#include "gate_handle.h"
#include "style.h"
#include "camera.h"
#include "nabu_instance.h"
#include "shape_buffer.h"

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
        sf::Vector2f last_mouse_coords;
        
        shape_buffer_t edge_layer;
        shape_buffer_t shape_layer;
        
        nabu::machine_t machine;
        
        root_window_t()
        {
            width  = sf::VideoMode::getDesktopMode().width*0.8;
            height = sf::VideoMode::getDesktopMode().height*0.8;
            window = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(width, height), "NABU Interactive"));
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
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    float r = 0.5;
                    auto newShape = shape_layer.add<sf::CircleShape>(r);
                    newShape->setOutlineThickness(0.05*r);
                    newShape->setPosition(last_mouse_coords.x - r, last_mouse_coords.y - r);
                    newShape->setFillColor(sf::Color::Green);
                    newShape->setOutlineColor(sf::Color::Black);
                }
            }
            if (event.type == sf::Event::MouseButtonReleased)
            {
                mouse_pressed[event.mouseButton.button] = false;
                set_last_mouse_position();
            }
            if(event.type == sf::Event::MouseMoved)
            {
                if (mouse_pressed[sf::Mouse::Right])
                {
                    //todo: introduce world coordinates
                    sf::Vector2i dx = get_mouse_position() - last_mouse_position;
                    current_camera.increment_px(dx);
                }
                set_last_mouse_position();
                current_camera.set_zoom_center(last_mouse_coords);
            }
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
                {
                    int delta = event.mouseWheelScroll.delta;
                    if (delta < 0) current_camera.zoom_out();
                    if (delta > 0) current_camera.zoom_in();
                }
            }
        }
        
        sf::Transform get_screen_transform() const
        {
            sf::Transform screen_offset = sf::Transform::Identity;
            auto win_size = window->getSize();
            
            //this corresponds to setting the 0,0 coordinate at the screen center
            screen_offset.translate(0.4*(float)win_size.x, 0.4*(float)win_size.y);
            return screen_offset;
        }
        
        sf::Transform world_to_screen() const
        {
            auto scr = get_screen_transform();
            sf::Transform output = scr*current_camera.get_transform();
            return output;
        }
        
        sf::Transform screen_to_world() const
        {
            auto trs = world_to_screen();
            return trs.getInverse();
        }
        
        sf::Vector2i get_mouse_position()
        {
            return sf::Mouse::getPosition(*window);
        }
        
        void set_last_mouse_position()
        {
            last_mouse_position = get_mouse_position();
            sf::Transform trs = screen_to_world();
            sf::Vector2f mpf((float)last_mouse_position.x, (float)last_mouse_position.y);
            last_mouse_coords = trs.transformPoint(mpf);
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
            window->clear(root_style.back_color);
            
            shape_layer.draw(*window, world_to_screen());
            
            // nabu::gate_t org(nabu::op_or);
            // gate_handle_t pp(org);
            
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