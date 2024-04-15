#pragma once
#include <fmt/format.h>
#include "uncertain_value.hpp"
    using calculisto::uncertain_value::uncertain_value_t;

    template <class T>
    struct
fmt::formatter <uncertain_value_t <T>>
    : fmt::formatter <T>
{
        using
    base_formatter = fmt::formatter <T>;

        template <class FormatContext>
        auto
    format (uncertain_value_t <T> const& u, FormatContext& ctx) 
    {
            using std::sqrt;
        base_formatter::format (u.value, ctx);
        fmt::format_to (ctx.out (), "±");
        base_formatter::format (sqrt (u.uncertainty), ctx);
        return ctx.out ();
    }
};
