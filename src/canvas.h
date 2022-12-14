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
        
        template <typename stream_t> stream_t& write(stream_t& stream)
        {
            nabu::write(machine, stream);
            auto& all_gates = machine.get_gates();
            stream << "\n<x>\n";
            for (int j = 0; j < all_gates.size(); ++j)
            {
                gate_shapes_t* handle = gate_to_shape.at(all_gates[j]);
                sf::Vector2f pos = handle->get_position();
                stream << pos.x << " " << pos.y << " " << handle->get_rotation() << "\n";
            }
            stream << "</x>\n";
            return stream;
        }
        
        template <typename stream_t> stream_t& read(stream_t& stream)
        {
            //todo: handle errors
            std::string line;
            if (!std::getline(stream, line)) { print("ERR", __FILE__, __LINE__); abort(); }
            {std::istringstream iss(line); nabu::read(machine, iss);}
            auto& all_gates = machine.get_gates();
            if (!std::getline(stream, line)) { print("ERR", __FILE__, __LINE__); abort(); }
            if (line != "<x>") { print("ERR", __FILE__, __LINE__, line); abort(); }
            for (int j = 0; j < all_gates.size(); j++)
            {
                if (!std::getline(stream, line)) { print("ERR", __FILE__, __LINE__); abort(); }
                sf::Vector2f pos;
                float angl;
                std::istringstream iss(line);
                iss >> pos.x;
                iss >> pos.y;
                iss >> angl;
                gate_shapes_t* shapes = add_gate(all_gates[j], pos, angl);
                if (iss.fail())  { print("ERR", __FILE__, __LINE__); abort(); }
            }
            if (!std::getline(stream, line)) { print("ERR", __FILE__, __LINE__); abort(); }
            if (line != "</x>") { print("ERR", __FILE__, __LINE__); abort(); }
            
            //add edges
            auto& all_edges = machine.get_edges();
            for (int j = 0; j < all_edges.size(); ++j)
            {
                nabu::edge_t* edge = all_edges[j];
                std::set<std::pair<gate_shapes_t*, sf::Shape*>> handles;
                gate_shapes_t* c_handle = gate_to_shape.at(edge->control->owner);
                handles.insert({c_handle, &c_handle->out});
                for (auto inode: edge->out)
                {
                    std::pair<gate_shapes_t*, sf::Shape*> data;
                    nabu::gate_t* gate = inode->owner;
                    data.first = gate_to_shape.at(gate);
                    if (inode == &gate->in(0)) data.second = &data.first->in0;
                    if (inode == &gate->in(1)) data.second = &data.first->in1;
                    handles.insert(data);
                }
                create_edge_from_node_selection_i(&handles, edge);
            }
            
            return stream;
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
        
        gate_shapes_t* add_gate(const nabu::operation& oper, const sf::Vector2f& position, const float& angle)
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
            return shapes;
        }
        
        gate_shapes_t* add_gate(nabu::gate_t* gate, const sf::Vector2f& position, const float& angle)
        {
            gate_shapes_t* shapes = new gate_shapes_t(gate->gate_operation, position, assets);
            gate_shapes.push_back(shapes);
            nabu::gate_t* new_gate = gate;
            
            //need to keep track of the correspondence between gates and the shapes that they 
            gate_shapes_t* new_shapes = shapes;
            shape_to_gate.insert({new_shapes, new_gate});
            gate_to_shape.insert({new_gate, new_shapes});
            last_added_gate = gate;
            new_shapes->set_rotation(angle);
            return new_shapes;
        }
        
        void delete_edge(edge_shapes_t* handle)
        {
            nabu::edge_t* edge = shape_to_edge.at(handle);
            machine.remove(edge);
            auto it1 = std::find(edge_shapes.begin(), edge_shapes.end(), handle);
            edge_shapes.erase(it1);
            auto it2 = edge_to_shape.find(edge);
            if (it2 != edge_to_shape.end()) edge_to_shape.erase(it2);
            auto it3 = shape_to_edge.find(handle);
            if (it3 != shape_to_edge.end()) shape_to_edge.erase(it3);
            delete handle;
        }
        
        void recompute_edge(edge_shapes_t* handle)
        {
            nabu::edge_t* edge = shape_to_edge.at(handle);
            gate_shapes_t* ctrl_shapes = gate_to_shape.at(edge->control->owner);
            sf::Vector2f control_point = ctrl_shapes->get_center_r(ctrl_shapes->out);
            std::vector<sf::Vector2f> i_pts;
            for (auto i: edge->out)
            {
                sf::Vector2f ipt;
                nabu::gate_t* gate = i->owner;
                gate_shapes_t* ihandle = gate_to_shape.at(gate);
                if (i == &gate->in(0)) ipt = ihandle->get_center_r(ihandle->in0);
                if (i == &gate->in(1)) ipt = ihandle->get_center_r(ihandle->in1);
                i_pts.push_back(ipt);
            }
            *handle = edge_shapes_t(assets, control_point, i_pts);
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
                    //delete "hanging" edge
                    edge_shapes_t* e_handle = edge_to_shape.at(p.first);
                    delete_edge(e_handle);
                }
                else if (p.first != nullptr)
                {
                    //we update the shapes to remove the ghost edge
                    edge_shapes_t* e_handle = edge_to_shape.at(p.first);
                    recompute_edge(e_handle);
                }
            }
            delete handle;
        }
        
        std::vector<gate_shapes_t*> get_shapes_in_bounding_box(float xmin, float xmax, float ymin, float ymax)
        {
            std::vector<gate_shapes_t*> output;
            //slow, optimize later
            for (auto s: gate_shapes)
            {
                if (s->in_selection_bounding_box(xmin, xmax, ymin, ymax))
                {
                    output.push_back(s);
                }
            }
            return output;
        }
        
        void handle_node_delete(gate_shapes_t* handle, sf::Shape* shape)
        {
            nabu::gate_t* gate = shape_to_gate.at(handle);
            if (shape == &handle->out && gate->output.edge != nullptr)
            {
                edge_shapes_t* e_handle = edge_to_shape.at(gate->output.edge);
                delete_edge(e_handle);
            }
            else
            {
                nabu::inode_t* inode = nullptr;
                if (shape == &handle->in0) inode = &gate->in(0);
                if (shape == &handle->in1) inode = &gate->in(1);
                nabu::edge_t* edge = inode->edge;
                if (edge != nullptr)
                {
                    edge_shapes_t* e_handle = edge_to_shape.at(edge);
                    if (edge->detach(*inode)) delete_edge(e_handle);
                    else recompute_edge(e_handle);
                }
            }
        }

        void clear()
        {
            machine.clear();
            gate_shapes.clear();
            edge_shapes.clear();
            shape_to_gate.clear();
            gate_to_shape.clear();
            shape_to_edge.clear();
            edge_to_shape.clear();
        }
        
        void delete_items(std::set<gate_shapes_t*>* handles, std::set<std::pair<gate_shapes_t*, sf::Shape*>>* nodes)
        {
            if (handles->size() > 0)
            {
                for (auto handle: *handles)
                {
                    delete_gate(handle);
                }
                handles->clear();
            }
            if (nodes->size() > 0)
            {
                for (auto p: *nodes)
                {
                    handle_node_delete(p.first, p.second);
                    p.first->deselect(assets);
                }
                nodes->clear();
            }
        }
        
        void create_edge_from_node_selection(std::set<std::pair<gate_shapes_t*, sf::Shape*>>* handles)
        {
            create_edge_from_node_selection_i(handles, nullptr);
        }
        edge_shapes_t* create_edge_from_node_selection_i(std::set<std::pair<gate_shapes_t*, sf::Shape*>>* handles, nabu::edge_t* edge_in)
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
                nabu::edge_t* new_edge;
                if (edge_in == nullptr) new_edge = machine.add_edge();
                else new_edge = edge_in;
                new_edge->attach(*control_node);
                for (auto p: inodes) new_edge->attach(*p);
                edge_shapes_t* new_edge_shapes = new edge_shapes_t(assets, control_point, i_pts);
                edge_shapes.push_back(new_edge_shapes);
                shape_to_edge.insert({new_edge_shapes, new_edge});
                edge_to_shape.insert({new_edge, new_edge_shapes});
                return new_edge_shapes;
            }
            return nullptr;
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
    
    template <typename condition_t> std::set<gate_shapes_t*>
    copy_canvas(canvas_t& source, canvas_t& destination, const condition_t& condition, const sf::Vector2f& offset = sf::Vector2f(0,0))
    {
        std::map<gate_shapes_t*, gate_shapes_t*> shape_table;
        std::set<gate_shapes_t*> output;
        for (auto prev: source.gate_shapes)
        {
            if (condition(prev))
            {
                auto next = destination.add_gate(prev->op, prev->get_position() + offset, prev->get_rotation());
                shape_table.insert({prev, next});
                nabu::gate_t* old_gate = source.shape_to_gate.at(prev);
                nabu::gate_t* new_gate = destination.shape_to_gate.at(next);
                new_gate->out().node_state = old_gate->out().node_state;
                new_gate->in(0).node_state = old_gate->in(0).node_state;
                new_gate->in(1).node_state = old_gate->in(1).node_state;
                output.insert(next);
            }
        }
        for (auto entry: shape_table)
        {
            nabu::edge_t* edge = source.shape_to_gate.at(entry.first)->out().edge;
            if (edge != nullptr)
            {
                std::set<std::pair<gate_shapes_t*, sf::Shape*>> handles;
                handles.insert({entry.second, &entry.second->out});
                for (auto inode: edge->out)
                {
                    nabu::gate_t* igate = inode->owner;
                    gate_shapes_t* ishapes = source.gate_to_shape.at(igate);
                    if (condition(ishapes))
                    {
                        gate_shapes_t* n_ishapes = shape_table.at(ishapes);
                        sf::Shape* shp = &n_ishapes->in0;
                        if (inode == &igate->in(1)) shp = &n_ishapes->in1;
                        handles.insert({n_ishapes, shp});
                    }
                }
                destination.create_edge_from_node_selection(&handles);
            }
        }
        return output;
    }
}