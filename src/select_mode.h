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
        sf::Vector2f last_pos, last_click;
        bool require_update = false;
        bool box_selecting = false;
        assets_t* assets;
        std::set<gate_shapes_t*> selected_shapes;
        std::set<std::pair<gate_shapes_t*, sf::Shape*>> selected_nodes;
        bool multi_select = false;
        selection_type current_selection_type = any_selection;
        
        canvas_t clipboard;
        sf::Vector2f current_offset, offset_increment;
        
        bool motion_moved = false;
        bool motion_mode  = false;
        
        static constexpr control_mode mode_type() {return control_select;}
        
        select_mode_t(){}
        select_mode_t(assets_t* assets_in)
        {
            assets = assets_in;
            offset_increment = sf::Vector2f(15.0, 15.0);
            current_offset = offset_increment;
            clipboard = canvas_t(assets_in);
        }
        
        void set_multi_select(bool val) {multi_select = val;}
        
        void enable()  { enabled = true;  }
        void disable() { enabled = false; }
        
        void on_lclick(const sf::Vector2f& pos, canvas_t& data)
        {
            last_pos = pos;
            last_click = pos;
            if (enabled)
            {
                motion_mode = ((hover_gate_shapes != nullptr) && (selected_shapes.size() > 0));
            }
        }
        
        void cut_selected(canvas_t* source)
        {
            if (selected_shapes.size() == 0) return;
            copy_selected(source);
            source->delete_items(&selected_shapes, &selected_nodes);
            current_offset = sf::Vector2f(0,0);
        }
        
        void copy_selected(canvas_t* source)
        {
            if (selected_shapes.size() == 0) return;
            clipboard.clear();
            current_offset = offset_increment;
            auto condition = [&](gate_shapes_t* shapes) -> bool
            {
                return selected_shapes.find(shapes) != selected_shapes.end();
            };
            copy_canvas(*source, clipboard, condition);
        }
        
        void paste_clipboard(canvas_t* destination)
        {
            clear_selections();
            auto condition = [&](gate_shapes_t* shapes) -> bool
            {
                return true;
            };
            selected_shapes = copy_canvas(clipboard, *destination, condition, current_offset);
            require_update = true;
            current_offset += offset_increment;
        }
        
        void on_ldrag(const sf::Vector2f& pos, canvas_t& data)
        {
            if (enabled)
            {
                if (motion_mode) motion_moved = true;
                box_selecting = (((selected_shapes.size() == 0) || multi_select) && !motion_mode);
                if (motion_moved)
                {
                    sf::Vector2f dx = last_pos - pos;
                    for (auto p: selected_shapes)
                    {
                        auto xx = p->get_position();
                        xx -= dx;
                        p->set_position(xx);
                        nabu::gate_t* gate = data.shape_to_gate.at(p);
                        nabu::edge_t* e_out = gate->out().edge;
                        nabu::edge_t* e_in0 = gate->in(0).edge;
                        nabu::edge_t* e_in1 = gate->in(1).edge;
                        if (e_out != nullptr) data.recompute_edge(e_out);
                        if (e_in0 != nullptr) data.recompute_edge(e_in0);
                        if (e_in1 != nullptr) data.recompute_edge(e_in1);
                    }
                }
            }
            last_pos = pos;
        }
        
        void on_lrelease(const sf::Vector2f& pos, canvas_t& data)
        {
            last_pos = pos;
            
            if (enabled)
            {
                //damn this is super ugly
                require_update = true;
                if ((hover_shape == nullptr || hover_gate_shapes == nullptr) && !multi_select && !motion_moved)
                {
                    clear_selections();
                }
                if (hover_shape != nullptr && hover_gate_shapes != nullptr)
                {
                    //if only one selection, we can select the other type without multi-select!
                    if (!multi_select && (selected_shapes.size() + selected_nodes.size())==1) current_selection_type = any_selection;
                    
                    // if we click on a type of thing with a group of the other
                    // thing selected and are not attempting multi-select, clear and select that thing
                    if (!motion_moved && !multi_select && (hover_shape == &hover_gate_shapes->body) && current_selection_type==node_selection) clear_selections();
                    if (!motion_moved && !multi_select && (hover_shape != &hover_gate_shapes->body) && current_selection_type==body_selection) clear_selections();
                    if ((hover_shape == &hover_gate_shapes->body) && ((current_selection_type == body_selection) || (current_selection_type == any_selection)))
                    {
                        auto it = selected_shapes.find(hover_gate_shapes);
                        if (!motion_moved)
                        {
                            if (it == selected_shapes.end())
                            {
                                if (!multi_select && !motion_moved) clear_selections();
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
                    }
                    else if ((hover_shape != &hover_gate_shapes->body) && (current_selection_type == node_selection) || (current_selection_type == any_selection))
                    {
                        std::pair<gate_shapes_t*, sf::Shape*> elem = {hover_gate_shapes, hover_shape};
                        auto it = selected_nodes.find(elem);
                        if (it == selected_nodes.end())
                        {
                            if (!multi_select && !motion_moved) clear_selections();
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
                    motion_moved = false;
                    motion_mode = false;
                }
                if (box_selecting)
                {
                    float x0 = utils::min(last_click.x, last_pos.x);
                    float x1 = utils::max(last_click.x, last_pos.x);
                    float y0 = utils::min(last_click.y, last_pos.y);
                    float y1 = utils::max(last_click.y, last_pos.y);
                    std::vector<gate_shapes_t*> selected = data.get_shapes_in_bounding_box(x0, x1, y0, y1);
                    if ((!multi_select || selected_nodes.size() > 0) && !motion_moved) clear_selections();
                    for (auto p: selected) selected_shapes.insert(p);
                    box_selecting = false;
                }
            }
        }
        
        void on_tick(const std::size_t& frame)
        {
            
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
                for (auto p: selected_nodes)
                {
                    gate_shapes_t* shapes = p.first;
                    if (p.second == &shapes->in0)
                    {
                        data.shape_to_gate.at(shapes)->toggle_input(0);
                    }
                    if (p.second == &shapes->in1)
                    {
                        data.shape_to_gate.at(shapes)->toggle_input(1);
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
            if(box_selecting)
            {
                float x0 = last_click.x;
                float y0 = last_click.y;
                float x1 = last_pos.x;
                float y1 = last_pos.y;
                sf::Color select_box_color = assets->colors.select_box_color;
                sf::Vertex line1[] = {sf::Vertex(sf::Vector2f(x0,y0), select_box_color), sf::Vertex(sf::Vector2f(x0,y1), select_box_color)};
                sf::Vertex line2[] = {sf::Vertex(sf::Vector2f(x0,y1), select_box_color), sf::Vertex(sf::Vector2f(x1,y1), select_box_color)};
                sf::Vertex line3[] = {sf::Vertex(sf::Vector2f(x1,y1), select_box_color), sf::Vertex(sf::Vector2f(x1,y0), select_box_color)};
                sf::Vertex line4[] = {sf::Vertex(sf::Vector2f(x1,y0), select_box_color), sf::Vertex(sf::Vector2f(x0,y0), select_box_color)};
                window.draw(line1, 2, sf::Lines, trans);
                window.draw(line2, 2, sf::Lines, trans);
                window.draw(line3, 2, sf::Lines, trans);
                window.draw(line4, 2, sf::Lines, trans);
            }
        }
    };
}