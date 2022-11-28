#pragma once

#include "nabu.h"
#include "shape_buffer.h"

namespace nbi
{
    //type that holds nabu types and shapes
    struct interactive_instance_t
    {
        nabu::machine_t machine;
        shape_buffer_t edge_layer;
        shape_buffer_t gate_layer;
        
        nabu::machine_t& get_machine()   {return machine;}
        shape_buffer_t& get_edge_layer() {return edge_layer;}
        shape_buffer_t& get_gate_layer() {return gate_layer;}
    };
}