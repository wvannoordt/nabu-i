#pragma once

#include <memory>
#include <thread>
#include <map>

#include <SFML/Graphics.hpp>

#include "nabu.h"

#include "gate_handle.h"
#include "style.h"
#include "camera.h"
#include "gate_placer.h"
#include "interactive_instance.h"

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
        
        gate_placer_t gate_placer;
        
        //This holds all of the logical state of the instance
        //dealing with the shape buffer and the logical
        //topology of the nabu machine
        interactive_instance_t instance;
        
        root_window_t()
        {
            width  = sf::VideoMode::getDesktopMode().width*0.8;
            height = sf::VideoMode::getDesktopMode().height*0.8;
            window = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(width, height), "NABU Interactive"));
            reset_camera();
            // later: call this in a thread loop
            this->poll_loop();
        }
        
        void debug_func()
        {
            
        }
        
        void reset_camera()
        {
            current_camera = camera_t(window->getSize().x, window->getSize().y);
            set_last_mouse_position();
        }
        
        void handle_event(const sf::Event& event)
        {
            gate_placer.handle_event(event, instance, last_mouse_coords);
            if (event.type == sf::Event::Closed) on_close();
            if (event.type == sf::Event::KeyPressed)
            {
                //todo: handle this elegantly
                key_pressed[event.key.code] = true;
                if (event.key.code == sf::Keyboard::W && event.key.control) on_close();
                if (event.key.code == sf::Keyboard::D) debug_func();
                if (event.key.code == sf::Keyboard::R && event.key.control) reset_camera();
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
                set_last_mouse_position();
            }
        }
        
        sf::Transform world_to_screen() const
        {
            return current_camera.get_transform();
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
            
            instance.get_edge_layer().draw(*window, world_to_screen());
            instance.get_gate_layer().draw(*window, world_to_screen());
            gate_placer.draw(*window, world_to_screen(), last_mouse_coords);
            
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