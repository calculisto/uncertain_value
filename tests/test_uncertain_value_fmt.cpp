#include <doctest/doctest.h>
#include "../include/isto/uncertain_value/uncertain_value_fmt.hpp"
    using isto::uncertain_value::uncertain_value_t;

TEST_CASE("uncertain_value_fmt.hpp")
{
        auto
    x = uncertain_value_t { 10.1, 1.21 };
        auto
    s1 = fmt::format ("{}", x);
    CHECK(s1 == "10.1±1.1");
        auto
    s2 = fmt::format ("{:.0}", x);
    CHECK(s2 == "1e+01±1");
}
