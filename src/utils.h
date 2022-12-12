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
    
    template <typename tp1_t, typename tp2_t> constexpr static auto max(const tp1_t& t1, const tp2_t& t2)
    {
        return t1<t2?t2:t1;
    }

    template <typename tp_t, typename... tps_t> constexpr static auto max(const tp_t& t, const tps_t&... ts)
    {
        return max(t, max(ts...));
    }

    template <typename tp1_t, typename tp2_t> constexpr static auto min(const tp1_t& t1, const tp2_t& t2)
    {
        return t1<t2?t1:t2;
    }

    template <typename tp_t, typename... tps_t> constexpr static auto min(const tp_t& t, const tps_t&... ts)
    {
        return min(t, min(ts...));
    }
}