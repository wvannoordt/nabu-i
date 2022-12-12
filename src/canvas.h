#pragma once

#include <set>
#include <algorithm>

#include "shape_buffer.h"
#include "view.h"
#include "nabu.h"
#include "gate_shapes.h"
#include "edge_shapes.h"

namespace nbi
{
    //holds all data that the user is exposed to
    struct canvas_t
    {
        assets_t* assets;
        std::vector<gate_shapes_t*> gate_shapes;
        std::vector<edge_shapes_t*> edge_shapes;
        nabu::machine_t machine;
        gate_shapes_t* last_added_shapes = nullptr;
        nabu::gate_t* last_added_gate = nullptr;
        std::map<gate_shapes_t*, nabu::gate_t*> shape_to_gate;
        std::map<nabu::gate_t*, gate_shapes_t*> gate_to_shape;
        std::map<edge_shapes_t*, nabu::edge_t*> shape_to_edge;
        std::map<nabu::edge_t*, edge_shapes_t*> edge_to_shape;
        canvas_t(){}
        canvas_t(assets_t* assets_in)
        {
            assets = assets_in;
        }
        
        void update_colors()
        {
            auto updcl = [&](const nabu::state& state_in, sf::Shape& shp) -> void
            {
                switch (state_in)
                {
                    case nabu::off_state: {shp.setFillColor(assets->colors.off_state_color); return;}
                    case nabu::bad_state: {shp.setFillColor(assets->colors.bad_state_color); return;}
                    case nabu::on_state:  {shp.setFillColor(assets->colors.on_state_color);  return;}
                }
            };
            for (auto p: machine.get_edges())
            {
                auto e_shapes = edge_to_shape.at(p);
                for (auto& p2: e_shapes->shapes)
                {
                    updcl(p->get_state(), p2);
                }
            }
            for (auto& p: machine.get_gates())
            {
                auto shapes = gate_to_shape.at(p);
                updcl(p->in(0).node_state, shapes->in0);
                updcl(p->in(1).node_state, shapes->in1);
                updcl(p->out().node_state, shapes->out);
            }
        }
        
        void draw(sf::RenderWindow& window, const sf::Transform& trans)
        {
            update_colors();
            for (auto& p: edge_shapes)
            {
                p->draw(window, trans);
            }
            for (auto& p: gate_shapes)
            {
                p->draw(window, trans);
            }
        }
        
        nabu::gate_t* add_gate(const nabu::operation& oper, const sf::Vector2f& position, const float& angle)
        {
            gate_shapes_t* shapes = new gate_shapes_t(oper, position, assets);
            gate_shapes.push_back(shapes);
            auto new_gate = machine.add_gate(oper);
            
            //need to keep track of the correspondence between gates and the shapes that they 
            gate_shapes_t* new_shapes = shapes;
            shape_to_gate.insert({new_shapes, new_gate});
            gate_to_shape.insert({new_gate, new_shapes});
            last_added_gate = new_gate;
            new_shapes->set_rotation(angle);
            return new_gate;
        }
        
        void delete_edge(edge_shapes_t* edge)
        {
            print("AAA");
        }
        
        void delete_gate(gate_shapes_t* handle)
        {
            nabu::gate_t* gate = shape_to_gate.at(handle);
            auto it1 = std::find(gate_shapes.begin(), gate_shapes.end(), handle);
            if (it1 != gate_shapes.end()) gate_shapes.erase(it1);
            auto it2 = shape_to_gate.find(handle);
            if (it2 != shape_to_gate.end()) shape_to_gate.erase(it2);
            auto it3 = gate_to_shape.find(gate);
            if (it3 != gate_to_shape.end()) gate_to_shape.erase(it3);
            std::array<std::pair<nabu::edge_t*, bool>, 3> data = machine.remove(gate);
            for (auto p: data)
            {
                if (p.second)
                {
                    edge_shapes_t* e_handle = edge_to_shape.at(p.first);
                    delete_edge(e_handle);
                }
                //still need to handle the case of updating an edge that isn't removed
            }
        }
        
        void delete_gates(std::set<gate_shapes_t*>* handles)
        {
            for (auto handle: *handles)
            {
                delete_gate(handle);
            }
        }
        
        void create_edge_from_node_selection(std::set<std::pair<gate_shapes_t*, sf::Shape*>>* handles)
        {
            nabu::onode_t* control_node = nullptr;
            std::vector<nabu::inode_t*> inodes;
            sf::Vector2f control_point;
            std::vector<sf::Vector2f> i_pts;
            auto is_inode = [&](const std::pair<gate_shapes_t*, sf::Shape*>& p) -> bool
            {
                return p.second != &(p.first->out);
            };
            for (auto p: *handles)
            {
                nabu::gate_t& gate = *shape_to_gate.at(p.first);
                if (is_inode(p))
                {
                    sf::Vector2f ipt;
                    int i = 0;
                    ipt = p.first->get_center_r(p.first->in0);
                    if (p.second == &(p.first->in1))
                    {
                        i = 1;
                        ipt = p.first->get_center_r(p.first->in1);
                    }
                    inodes.push_back(&gate.in(i));
                    i_pts.push_back(ipt);
                }
                else
                {
                    control_node = &gate.out();
                    control_point = p.first->get_center_r(*((sf::CircleShape*)p.second));
                }
            }
            if (control_node != nullptr && (inodes.size()>0))
            {
                auto new_edge = machine.add_edge();
                new_edge->attach(*control_node);
                for (auto p: inodes) new_edge->attach(*p);
                edge_shapes_t* new_edge_shapes = new edge_shapes_t(assets, control_point, i_pts);
                edge_shapes.push_back(new_edge_shapes);
                shape_to_edge.insert({new_edge_shapes, new_edge});
                edge_to_shape.insert({new_edge, new_edge_shapes});
            }
        }
        
        //todo: optimize this using bounding boxes!
        bool get_shape_at_cursor(const sf::Vector2f& pos, gate_shapes_t*& shapes_out, sf::Shape*& which_shape)
        {
            for (auto p: gate_shapes)
            {
                if (p->check_collide(pos, which_shape))
                {
                    shapes_out = p;
                    return true;
                }
            }
            shapes_out = nullptr;
            return false;
        }
        
        ~canvas_t()
        {
            for (auto p: gate_shapes) delete p;
            for (auto p: edge_shapes) delete p;
        }
    };
}