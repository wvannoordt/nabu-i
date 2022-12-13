#pragma once

#include <SFML/Graphics.hpp>
#include "nabu.h"

namespace nbi
{
    struct gate_shapes_t
    {
        sf::CircleShape in0, in1, out, aux;
        sf::ConvexShape body;
        sf::ConvexShape r0, r1, ro;
        nabu::operation op;
        float r        = 0.45;
        float r_select = 1.3;
        float r_aux  = 0.25;
        float r_rect = 0.05;
        float outline_thick = 0.09;
        float rotate_angle = 0.0;
        sf::Vector2f position;
        
        sf::Vector2f get_center_r(const sf::CircleShape& sirc)
        {
            sf::Transform trans_loc = sf::Transform::Identity;
            trans_loc.rotate(rotate_angle, position);
            sf::Vector2f output = sirc.getPosition() + sirc.getRadius()*sf::Vector2f(1,1);
            output = trans_loc.transformPoint(output);
            return output;
        }
        sf::Vector2f get_center(const sf::CircleShape& sirc)
        {
            sf::Vector2f output = sirc.getPosition() + sirc.getRadius()*sf::Vector2f(1,1);
            return output;
        }
        
        gate_shapes_t(){}
        gate_shapes_t(const nabu::operation& op_in, const sf::Vector2f& pos, assets_t* assets)
        {
            op = op_in;
            aux = sf::CircleShape(r_aux);
            in0 = sf::CircleShape(r);
            in1 = sf::CircleShape(r);
            out = sf::CircleShape(r);
            sf::Uint8 alpha = 255;
            sf::Color body_color = assets->colors.gate_color;
            sf::Color node_color = assets->colors.bad_state_color;
            sf::Color outl_color = assets->colors.border_color;
            auto set_style = [&](sf::Shape& s) -> void
            {
                s.setOutlineThickness(outline_thick);
                s.setFillColor(body_color);
                s.setOutlineColor(outl_color);
            };
            set_style(in0);
            set_style(in1);
            set_style(out);
            set_style(aux);
            set_style(body);
            in0.setFillColor(node_color);
            in1.setFillColor(node_color);
            out.setFillColor(node_color);
            
            auto set_rstyle = [&outl_color](sf::Shape& s) -> void
            {
                s.setFillColor(outl_color);
            };
            set_rstyle(r0);
            set_rstyle(r1);
            set_rstyle(ro);
            set_position(pos);
        }
        
        void select(assets_t* assets, sf::Shape* shape)
        {
            shape->setOutlineColor(assets->colors.select_color);
            if (shape == &body) aux.setOutlineColor(assets->colors.select_color);
        }
        
        void preselect(assets_t* assets, sf::Shape* shape)
        {
            shape->setOutlineColor(assets->colors.preselect_color);
            if (shape == &body) aux.setOutlineColor(assets->colors.preselect_color);
        }
        
        void deselect(assets_t* assets)
        {
            in0.setOutlineColor(assets->colors.border_color);
            in1.setOutlineColor(assets->colors.border_color);
            out.setOutlineColor(assets->colors.border_color);
            aux.setOutlineColor(assets->colors.border_color);
            body.setOutlineColor(assets->colors.border_color);
        }
        
        void set_position(const sf::Vector2f& pos)
        {
            position = pos;
            auto create_connection = [&](sf::ConvexShape& rect, const sf::Vector2f& p1, const sf::Vector2f& p2) -> void
            {
                sf::Vector2f dx(p2.y-p1.y, p1.x-p2.x);
                float norm = -std::sqrt(dx.x*dx.x + dx.y*dx.y);
                dx /= norm;
                rect.setPointCount(4);
                rect.setPoint(0, p1 + r_rect*dx);
                rect.setPoint(1, p2 + r_rect*dx);
                rect.setPoint(2, p2 - r_rect*dx);
                rect.setPoint(3, p1 - r_rect*dx);
            };
            
            sf::Vector2f pos_copy = pos;
            // sf::Vector2f pos_copy(0,0);
            //TODO: make these not look like complete garbage
            switch(op)
            {
                case nabu::op_id:
                {
                    in0.setPosition(pos_copy.x - r - 2.8, pos_copy.y - r);
                    out.setPosition(pos_copy.x - r + 2.8, pos_copy.y - r);
                    body.setPointCount(3);
                    body.setPoint(0, pos_copy + sf::Vector2f(-1.0,  1.0));
                    body.setPoint(1, pos_copy + sf::Vector2f( 1.0,  0.0));
                    body.setPoint(2, pos_copy + sf::Vector2f(-1.0, -1.0));
                    create_connection(r0, get_center(in0), get_center(in0)+sf::Vector2f(2.1, 0.0));
                    create_connection(ro, get_center(out) - sf::Vector2f(2.1, 0.0), get_center(out));
                    break;
                }
                case nabu::op_inv:
                {
                    in0.setPosition(pos_copy.x - r - 2.8, pos_copy.y - r);
                    out.setPosition(pos_copy.x - r + 2.8, pos_copy.y - r);
                    aux.setPosition(pos_copy + sf::Vector2f( 1.0,  0.0) - sf::Vector2f(r_aux, r_aux));
                    body.setPointCount(3);
                    body.setPoint(0, pos_copy + sf::Vector2f(-1.0,  1.0));
                    body.setPoint(1, pos_copy + sf::Vector2f( 1.0,  0.0));
                    body.setPoint(2, pos_copy + sf::Vector2f(-1.0, -1.0));
                    create_connection(r0, get_center(in0), get_center(in0)+sf::Vector2f(2.1, 0.0));
                    create_connection(ro, get_center(out) - sf::Vector2f(2.1, 0.0), get_center(out));
                    break;
                }
                case nabu::op_or:
                {
                    in0.setPosition(pos_copy.x - r - 2.8, pos_copy.y - r - 1.0);
                    in1.setPosition(pos_copy.x - r - 2.8, pos_copy.y - r + 1.0);
                    out.setPosition(pos_copy.x - r + 2.8, pos_copy.y - r);
                    body.setPointCount(13);
                    
                    body.setPoint(1,  pos_copy + sf::Vector2f(-0.7,  0.9));
                    body.setPoint(0,  pos_copy + sf::Vector2f(-0.4,  0.4));
                    
                    body.setPoint(2,  pos_copy + sf::Vector2f(-1.0,  1.3));
                    body.setPoint(3,  pos_copy + sf::Vector2f(-0.5,  1.2));
                    body.setPoint(4,  pos_copy + sf::Vector2f( 0.3,  0.9));
                    body.setPoint(5,  pos_copy + sf::Vector2f( 0.8,  0.5));
                    body.setPoint(6,  pos_copy + sf::Vector2f( 1.0,  0.0));
                    body.setPoint(7,  pos_copy + sf::Vector2f( 0.8, -0.5));
                    body.setPoint(8,  pos_copy + sf::Vector2f( 0.3, -0.9));
                    body.setPoint(9,  pos_copy + sf::Vector2f(-0.5, -1.2));
                    body.setPoint(10, pos_copy + sf::Vector2f(-1.0, -1.3));
                    
                    body.setPoint(12, pos_copy + sf::Vector2f(-0.4, -0.4));
                    body.setPoint(11, pos_copy + sf::Vector2f(-0.7, -0.9));
                    
                    create_connection(r0, get_center(in0), get_center(in0)+sf::Vector2f(2.1, 0.0));
                    create_connection(r1, get_center(in1), get_center(in1)+sf::Vector2f(2.1, 0.0));
                    create_connection(ro, get_center(out) - sf::Vector2f(2.1, 0.0), get_center(out));
                    break;
                }
                case nabu::op_and:
                {
                    in0.setPosition(pos_copy.x - r - 2.8, pos_copy.y - r - 1.0);
                    in1.setPosition(pos_copy.x - r - 2.8, pos_copy.y - r + 1.0);
                    out.setPosition(pos_copy.x - r + 2.8, pos_copy.y - r);
                    body.setPointCount(9);
                    body.setPoint(0, pos_copy + sf::Vector2f(-1.0,  1.3));
                    body.setPoint(1, pos_copy + sf::Vector2f(-0.5,  1.2));
                    body.setPoint(2, pos_copy + sf::Vector2f( 0.3,  0.9));
                    body.setPoint(3, pos_copy + sf::Vector2f( 0.8,  0.5));
                    body.setPoint(4, pos_copy + sf::Vector2f( 1.0,  0.0));
                    body.setPoint(5, pos_copy + sf::Vector2f( 0.8, -0.5));
                    body.setPoint(6, pos_copy + sf::Vector2f( 0.3, -0.9));
                    body.setPoint(7, pos_copy + sf::Vector2f(-0.5, -1.2));
                    body.setPoint(8, pos_copy + sf::Vector2f(-1.0, -1.3));
                    
                    create_connection(r0, get_center(in0), get_center(in0)+sf::Vector2f(2.1, 0.0));
                    create_connection(r1, get_center(in1), get_center(in1)+sf::Vector2f(2.1, 0.0));
                    create_connection(ro, get_center(out) - sf::Vector2f(2.1, 0.0), get_center(out));
                    break;
                }
            }
        }
        
        void set_rotation(const float& rot)
        {
            rotate_angle = rot;
        }
        
        float get_rotation() const
        {
            return rotate_angle;
        }
        
        sf::Vector2f get_position() const
        {
            return position;
        }
        
        bool in_selection_bounding_box(float xmin, float xmax, float ymin, float ymax)
        {
            //we require simply that the position is in the bounding box for now.
            return (xmin < position.x) && (xmax > position.x) && (ymin < position.y) && (ymax > position.y);
        }
        
        void draw(sf::RenderWindow& window, const sf::Transform& trans)
        {
            sf::Transform trans_loc = trans;
            trans_loc.rotate(rotate_angle, position);
            window.draw(r0, trans_loc);
            window.draw(ro, trans_loc);
            if ((op == nabu::op_or) || (op == nabu::op_and)) window.draw(r1, trans_loc);
            window.draw(in0, trans_loc);
            if ((op == nabu::op_or) || (op == nabu::op_and)) window.draw(in1, trans_loc);
            window.draw(body, trans_loc);
            if (op==nabu::op_inv) window.draw(aux, trans_loc);
            window.draw(out, trans_loc);
        }
        
        bool check_collide(const sf::Vector2f& pos, sf::Shape*& which_shape)
        {
            auto condi = [&](const sf::Vector2f& a, const sf::Vector2f& b, const float r_s) -> bool
            {
                sf::Vector2f dx = b - a;
                return (dx.x*dx.x + dx.y*dx.y) < (r_s*r_s);
            };
            sf::Vector2f dx_body = pos - position;
            if (condi(pos, position, r_select))
            {
                which_shape = &body;
                return true;
            }
            if (condi(pos, get_center_r(in0), r))
            {
                which_shape = &in0;
                return true;
            }
            if (condi(pos, get_center_r(out), r))
            {
                which_shape = &out;
                return true;
            }
            if ((op == nabu::op_and || op == nabu::op_or)  && condi(pos, get_center_r(in1), r))
            {
                which_shape = &in1;
                return true;
            }
            which_shape = nullptr;
            return false;
        }
        
        void add_to_buffer(shape_buffer_t& buffer, std::vector<sf::Shape*>& added_shapes)
        {
            added_shapes.clear();
            auto add_shape = [&](auto& shp) -> void
            {
                auto ptr = buffer.copy_to_buffer(shp);
                added_shapes.push_back(ptr);
            };
            add_shape(r0);
            add_shape(ro);
            if ((op == nabu::op_or) || (op == nabu::op_and)) add_shape(r1);
            add_shape(in0);
            if ((op == nabu::op_or) || (op == nabu::op_and)) add_shape(in1);
            add_shape(body);
            if (op==nabu::op_inv) add_shape(aux);
            add_shape(out);
        }
    };
}