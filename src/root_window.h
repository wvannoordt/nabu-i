#pragma once

#include <functional>
#include <SFML/Graphics.hpp>

#include "control_modes.h"
#include "assets.h"
#include "view.h"
#include "event_dispatch.h"
#include "gate_place_mode.h"
#include "select_mode.h"
#include "canvas.h"
#include "key.h"
#include "mouse.h"

namespace nbi
{    
    struct root_window_t
    {
        assets_t assets;
        std::size_t frame = 0;
        std::unique_ptr<sf::RenderWindow> window;
        event_dispatch_t<key_t,   void()> key_event_dispatch;
        event_dispatch_t<mouse_t, void(const sf::Vector2f&, canvas_t&)> mouse_event_dispatch;
        mouse_state_t mouse_state;
        canvas_t canvas;
        view_t view;
        
        control_mode current_mode = control_select;
        gate_place_mode_t gate_place_mode;
        select_mode_t select_mode; //on by default
        
        // gate_placer_t gate_placer;
        
        //This holds all of the logical state of the instance
        //dealing with the shape buffer and the logical
        //topology of the nabu machine
        // interactive_instance_t instance;
        
        root_window_t(const assets_t& assets_in)
        {
            assets = assets_in;
            int width  = sf::VideoMode::getDesktopMode().width*0.8;
            int height = sf::VideoMode::getDesktopMode().height*0.8;
            window = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(width, height), "NABU Interactive"));
            
            //key events
            key_event_dispatch.add_call(
                key::ctrl + key::w,
                std::bind(&root_window_t::on_close, this));
            key_event_dispatch.add_call(
                key::a,
                std::bind(&root_window_t::toggle_control_mode, this, control_gate_place));
            key_event_dispatch.add_call(
                key::tab,
                std::bind(&gate_place_mode_t::next_op, &gate_place_mode));
            key_event_dispatch.add_call(
                key::shift + key::tab,
                std::bind(&gate_place_mode_t::prev_op, &gate_place_mode));
            key_event_dispatch.add_call(
                key::esc,
                std::bind(&root_window_t::toggle_control_mode, this, control_select));
            key_event_dispatch.add_call(
                key::ctrl + key::shift + key::d,
                std::bind(&root_window_t::debug_func, this));
            key_event_dispatch.add_call(
                key::ctrl + key::r,
                std::bind(&root_window_t::reset_view, this));
            
            //mouse events
            mouse_event_dispatch.add_call(
                mouse_lclick,
                std::bind(&mouse_state_t::on_lclick, &mouse_state, std::placeholders::_1, std::placeholders::_2)
            );
            mouse_event_dispatch.add_call(
                mouse_rclick,
                std::bind(&mouse_state_t::on_rclick, &mouse_state, std::placeholders::_1, std::placeholders::_2)
            );
            mouse_event_dispatch.add_call(
                mouse_lrelease,
                std::bind(&mouse_state_t::on_lrelease, &mouse_state, std::placeholders::_1, std::placeholders::_2)
            );
            mouse_event_dispatch.add_call(
                mouse_rrelease,
                std::bind(&mouse_state_t::on_rrelease, &mouse_state, std::placeholders::_1, std::placeholders::_2)
            );
            mouse_event_dispatch.add_call(
                mouse_uscroll,
                std::bind(&view_t::on_uscroll, &view, std::placeholders::_1, std::placeholders::_2)
            );
            mouse_event_dispatch.add_call(
                mouse_dscroll,
                std::bind(&view_t::on_dscroll, &view, std::placeholders::_1, std::placeholders::_2)
            );
            mouse_event_dispatch.add_call(
                mouse_rdrag,
                std::bind(&view_t::on_rdrag, &view, std::placeholders::_1, std::placeholders::_2)
            );
            mouse_event_dispatch.add_call(
                mouse_move,
                std::bind(&view_t::on_mouse_move, &view, std::placeholders::_1, std::placeholders::_2)
            );
            mouse_event_dispatch.add_call(
                mouse_lclick,
                std::bind(&gate_place_mode_t::on_lclick, &gate_place_mode, std::placeholders::_1, std::placeholders::_2)
            );
            mouse_event_dispatch.add_call(
                mouse_lclick,
                std::bind(&select_mode_t::on_lclick, &select_mode, std::placeholders::_1, std::placeholders::_2)
            );
            mouse_event_dispatch.add_call(
                mouse_move,
                std::bind(&gate_place_mode_t::on_mouse_move, &gate_place_mode, std::placeholders::_1, std::placeholders::_2)
            );
            mouse_event_dispatch.add_call(
                mouse_move,
                std::bind(&select_mode_t::on_mouse_move, &select_mode, std::placeholders::_1, std::placeholders::_2)
            );
            
            canvas = canvas_t(&assets);
            gate_place_mode = gate_place_mode_t(&assets);
            reset_view();
            set_control_mode(control_select);
        }
        
        template <typename func_t> void foreach_mode_obj(const func_t& func)
        {
            func(gate_place_mode);
            func(select_mode);
        }
        
        void set_control_mode(const control_mode& mode)
        {
            foreach_mode_obj([&](auto& mode_obj) -> void
            {
                mode_obj.disable();
                if (mode_obj.mode_type() == mode)
                {
                    mode_obj.enable();
                }
            });
            current_mode = mode;
        }
        
        void toggle_control_mode(const control_mode& mode)
        {
            if (mode == current_mode) set_control_mode(control_select);
            else set_control_mode(mode);
        }
        
        void reset_view()
        {
            view = view_t(*window);
        }
        
        void on_close()
        {
            window->close();
        }
        
        void tick()
        {
            frame++;
        }
        
        void debug_func()
        {
            canvas.debug();
        }
        
        void render()
        {
            window->clear(assets.colors.back_color);
            canvas.draw(*window, view.get_transform());
            gate_place_mode.draw(*window, view.get_transform());
            window->display();
        }
        
        void run()
        {
            
            //we don't exit this loop until this window is closed, so need a thread?
            while (window->isOpen())
            {
                sf::Event event;
                this->tick();
                while (window->pollEvent(event))
                {
                    key_t key_out;
                    if (is_key_event(event, key_out))
                    {
                        key_event_dispatch.dispatch(key_out);
                    }
                    mouse_t mouse_out;
                    if (is_mouse_event(event, mouse_out, mouse_state))
                    {
                        const auto& pos = view.get_mouse_coords(*window);
                        mouse_event_dispatch.dispatch(mouse_out, pos, canvas);
                    }
                    if (event.type == sf::Event::Closed)
                    {
                        key_event_dispatch.dispatch(key::ctrl + key::w);
                    }
                }
                this->render();
            }
        }
    };
}