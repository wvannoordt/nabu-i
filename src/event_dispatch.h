#pragma once

#include <map>
#include <functional>

#include <SFML/Graphics.hpp>

#include "key.h"

namespace nbi
{
    template <typename event_t, typename func_call_t>
    struct event_dispatch_t
    {
        std::map<event_t, std::vector<std::function<func_call_t>>> calls;
        void add_call(const event_t& key, const std::function<func_call_t>& call)
        {
            auto& vec = calls[key];
            vec.push_back(call);
        }
        
        template <typename... args_t> void dispatch(const event_t& key, args_t&... args)
        {
            if (calls.find(key) != calls.end())
            {
                auto& callv = calls[key];
                for (const auto& v: callv) v(args...);
            }
        }
    };
}