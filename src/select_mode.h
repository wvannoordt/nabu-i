#pragma once

#include <set>

#include <SFML/Graphics.hpp>

#include "control_modes.h"
#include "canvas.h"
#include "assets.h"

namespace nbi
{
    enum selection_type
    {
        any_selection,
        node_selection,
        body_selection
    };
    
    struct select_mode_t
    {
        bool enabled = false;
        gate_shapes_t* hover_gate_shapes = nullptr;
        sf::Shape* hover_shape;
        sf::Vector2f last_pos;
        bool require_update = false;
        assets_t* assets;
        std::set<gate_shapes_t*> selected_shapes;
        std::set<std::pair<gate_shapes_t*, sf::Shape*>> selected_nodes;
        bool multi_select = false;
        selection_type current_selection_type = any_selection;
        
        static constexpr control_mode mode_type() {return control_select;}
        
        select_mode_t(){}
        select_mode_t(assets_t* assets_in)
        {
            assets = assets_in;
        }
        
        void set_multi_select(bool val) {multi_select = val;}
        
        void enable()  { enabled = true;  }
        void disable() { enabled = false; }
        
        void on_lclick(const sf::Vector2f& pos, canvas_t& data)
        {
            last_pos = pos;
            if (enabled)
            {
                //damn this is super ugly
                require_update = true;
                if (hover_shape != nullptr && hover_gate_shapes != nullptr)
                {
                    //if only one selection, we can select the other type without multi-select!
                    if (!multi_select && (selected_shapes.size() + selected_nodes.size())==1) current_selection_type = any_selection;
                    
                    // if we click on a type of thing with a group of the other
                    // thing selected and are not attempting multi-select, clear and select that thing
                    if (!multi_select && (hover_shape == &hover_gate_shapes->body) && current_selection_type==node_selection) clear_selections();
                    if (!multi_select && (hover_shape != &hover_gate_shapes->body) && current_selection_type==body_selection) clear_selections();
                    if ((hover_shape == &hover_gate_shapes->body) && ((current_selection_type == body_selection) || (current_selection_type == any_selection)))
                    {
                        auto it = selected_shapes.find(hover_gate_shapes);
                        if (it == selected_shapes.end())
                        {
                            if (!multi_select) clear_selections();
                            current_selection_type = body_selection;
                            selected_shapes.insert(hover_gate_shapes);
                        }
                        else
                        {
                            selected_shapes.erase(it);
                            current_selection_type = body_selection;
                            if (selected_shapes.size()==0) current_selection_type = any_selection;
                        }
                    }
                    else if ((hover_shape != &hover_gate_shapes->body) && (current_selection_type == node_selection) || (current_selection_type == any_selection))
                    {
                        std::pair<gate_shapes_t*, sf::Shape*> elem = {hover_gate_shapes, hover_shape};
                        auto it = selected_nodes.find(elem);
                        if (it == selected_nodes.end())
                        {
                            if (!multi_select) clear_selections();
                            current_selection_type = node_selection;
                            selected_nodes.insert(elem);
                        }
                        else
                        {
                            selected_nodes.erase(elem);
                            current_selection_type = node_selection;
                            if (selected_nodes.size()==0) current_selection_type = any_selection;
                        }
                    }
                }
            }
        }
        
        void on_lrelease(const sf::Vector2f& pos, canvas_t& data)
        {
            last_pos = pos;
            if (enabled)
            {
                require_update = true;
                if (hover_shape == nullptr || hover_gate_shapes == nullptr)
                {
                    clear_selections();
                }
            }
        }
        
        void on_rclick(const sf::Vector2f& pos, canvas_t& data)
        {
            last_pos = pos;
            if (enabled)
            {
                if (hover_shape != nullptr && hover_gate_shapes != nullptr)
                {
                    if  (hover_shape == &hover_gate_shapes->in0)
                    {
                        data.shape_to_gate.at(hover_gate_shapes)->toggle_input(0);
                    }
                    if  (hover_shape == &hover_gate_shapes->in1)
                    {
                        data.shape_to_gate.at(hover_gate_shapes)->toggle_input(1);
                    }
                }
            }
        }
        
        void on_mouse_move(const sf::Vector2f& pos, canvas_t& data)
        {
            last_pos = pos;
            if (enabled)
            {
                gate_shapes_t* new_hover;
                sf::Shape* selected_shape;
                require_update = true;
                
                //slow for now
                if (data.get_shape_at_cursor(pos, new_hover, selected_shape))
                {
                    if (hover_gate_shapes!= nullptr && hover_shape != nullptr) hover_gate_shapes->deselect(assets);
                    hover_gate_shapes = new_hover;
                    hover_shape = selected_shape;
                }
                else
                {
                    if (hover_gate_shapes!= nullptr && hover_shape != nullptr) hover_gate_shapes->deselect(assets);
                    hover_gate_shapes = nullptr;
                    hover_shape = nullptr;
                }
            }
        }
        
        void update_colors()
        {
            for (auto p: selected_shapes) p->select(assets, &p->body);
            for (auto p: selected_nodes)
            {
                p.first->select(assets, p.second);
            }
            if (hover_gate_shapes != nullptr && hover_shape != nullptr)
            {
                hover_gate_shapes->preselect(assets, hover_shape);
            }
        }
        
        void clear_selections()
        {
            for (auto p:selected_shapes) p->deselect(assets);
            for (auto p:selected_nodes) p.first->deselect(assets);
            selected_shapes.clear();
            selected_nodes.clear();
            update_colors();
            current_selection_type = any_selection;
        }
        
        void draw(sf::RenderWindow& window, const sf::Transform& trans)
        {
            if (require_update)
            {
                require_update = false;
                update_colors();
            }
        }
    };
}