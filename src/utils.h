#pragma once

#include <filesystem>

namespace nbi::utils
{
    namespace detail
    {
        template <typename str_t>
        static void div_eq(std::filesystem::path& p, const str_t& str)
        {
            p /= str;
        }
        
        template <typename str_t, typename... strs_t>
        static void div_eq(std::filesystem::path& p, const str_t& str, const strs_t&... strs)
        {
            p /= str;
            div_eq(p, strs...);
        }
    }
    
    template <typename... strs_t>
    static std::string combine_dir(const std::string& str, const strs_t&... strs)
    {
        std::filesystem::path p(str);
        detail::div_eq(p, strs...);
        return p;
    }
}