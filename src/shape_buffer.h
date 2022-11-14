#pragma once

#include <vector>
#include <map>

#include <SFML/Graphics.hpp>

namespace nbi
{
    struct shape_buffer_t
    {
        std::vector<sf::Shape*> shapes;
        template <typename shape_t, typename... params_t> shape_t* add(const params_t&... params)
        {
            //todo: check if we can get away with avoiding object slicing here
            auto output = new shape_t(params...);
            shapes.push_back(output);
            return output;
        }
        
        ~shape_buffer_t()
        {
            for (auto s: shapes) delete s;
            shapes.clear();
        }
        
        void draw(sf::RenderWindow& win, const sf::Transform& trans)
        {
            for (auto s: shapes) win.draw(*s, trans);
        }
    };
    
}