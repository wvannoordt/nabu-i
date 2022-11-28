#pragma once

#include <SFML/Graphics.hpp>

#include "PTL.h"

namespace nbi
{
    struct color_palette_t
    {
        sf::Color back_color;
        sf::Color off_state_color;
        sf::Color on_state_color;
        sf::Color bad_state_color;
        sf::Color border_color;
        sf::Color gate_color;
        color_palette_t(){}
        sf::Color read_color(PTL::PropertySection& section)
        {
            int r, g, b;
            r = section["R"];
            g = section["G"];
            b = section["B"];
            sf::Color output(r, g, b);
            return output;
        }
        void read(const std::string& filename)
        {
            PTL::PropertyTree tree;
            tree.Read(filename);
            this->read(tree.Head());
        }
        void read(PTL::PropertySection& section)
        {
            back_color      = read_color(section["back_color"]);
            off_state_color = read_color(section["off_state_color"]);
            on_state_color  = read_color(section["on_state_color"]);
            bad_state_color = read_color(section["bad_state_color"]);
            border_color    = read_color(section["border_color"]);
            gate_color      = read_color(section["gate_color"]);
        }
    };
}