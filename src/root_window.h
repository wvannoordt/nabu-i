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
#include "gate_placer.h"

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
        
        gate_placer_t gate_placer;
        
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
            float r = 0.5;
            float x = 0;
            float y = 0;
            auto newShape = shape_layer.add<sf::CircleShape>(r, 3);
            newShape->rotate(90.0);
            newShape->setOutlineThickness(0.05*r);
            newShape->setPosition(x - r, y - r);
            newShape->setFillColor(sf::Color::Blue);
            newShape->setOutlineColor(sf::Color::Black);
        }
        
        void reset_camera()
        {
            current_camera = camera_t(window->getSize().x, window->getSize().y);
            set_last_mouse_position();
        }
        
        void handle_event(const sf::Event& event)
        {
            gate_placer.handle_event(event, shape_layer, last_mouse_coords);
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
            
            shape_layer.draw(*window, world_to_screen());
            gate_placer.draw(*window, world_to_screen(), last_mouse_coords);
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