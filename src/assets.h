#pragma once

#include <string>

#include "color_palette.h"
#include "utils.h"

namespace nbi
{
    struct assets_t
    {
        color_palette_t colors;
        
        assets_t(){}
        assets_t(const std::string& asset_dir)
        {
            colors.read(utils::combine_dir(asset_dir, "colors.ptl"));
        }
    };
}