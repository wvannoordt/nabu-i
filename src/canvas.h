#pragma once

#include "shape_buffer.h"
#include "view.h"
#include "nabu.h"
#include "gate_shapes.h"

namespace nbi
{
    //holds all data that the user is exposed to
    struct canvas_t
    {
        assets_t* assets;
        shape_buffer_t edge_layer;
        std::vector<gate_shapes_t*> gate_shapes;
        nabu::machine_t machine;
        gate_shapes_t* last_added_shapes = nullptr;
        nabu::gate_t* last_added_gate = nullptr;
        std::map<gate_shapes_t*, nabu::gate_t*> shape_to_gate;
        std::map<nabu::gate_t*, gate_shapes_t*> gate_to_shape;
        canvas_t(){}
        canvas_t(assets_t* assets_in)
        {
            assets = assets_in;
        }
        
        void update_colors()
        {
            auto updcl = [&](const auto& node, sf::Shape& shp) -> void
            {
                switch (node.node_state)
                {
                    case nabu::off_state: {shp.setFillColor(assets->colors.off_state_color); return;}
                    case nabu::bad_state: {shp.setFillColor(assets->colors.bad_state_color); return;}
                    case nabu::on_state:  {shp.setFillColor(assets->colors.on_state_color);  return;}
                }
            };
            for (auto& p: machine.get_gates())
            {
                auto shapes = gate_to_shape.at(p);
                updcl(p->in(0), gate_to_shape.at(p)->in0);
                updcl(p->in(1), gate_to_shape.at(p)->in1);
                updcl(p->out(), gate_to_shape.at(p)->out);
            }
        }
        
        void draw(sf::RenderWindow& window, const sf::Transform& trans)
        {
            update_colors();
            edge_layer.draw(window, trans);
            for (auto& p: gate_shapes)
            {
                p->draw(window, trans);
            }
        }
        
        nabu::gate_t* add_gate(const nabu::operation& oper, const sf::Vector2f& position)
        {
            gate_shapes_t* shapes = new gate_shapes_t(oper, position, assets);
            gate_shapes.push_back(shapes);
            auto new_gate = machine.add_gate(oper);
            
            //need to keep track of the correspondence between gates and the shapes that they 
            gate_shapes_t* new_shapes = shapes;
            shape_to_gate.insert({new_shapes, new_gate});
            gate_to_shape.insert({new_gate, new_shapes});
            last_added_gate = new_gate;
            return new_gate;
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
        }
    };
}