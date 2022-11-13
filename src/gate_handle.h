#pragma once

#include <SFML/Graphics.hpp>

#include "nabu.h"

namespace nbi
{
    struct gate_handle_t
    {
        const nabu::gate_t* gate;
        gate_handle_t(){}
        gate_handle_t(const nabu::gate_t& gate_in)
        {
            gate = &gate_in;
        }
    };
}