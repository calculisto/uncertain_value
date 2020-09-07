#include <doctest/doctest.h>
#include "../include/isto/uncertain_value/uncertain_value.hpp"
    using isto::uncertain_value::uncertain_value_t;

TEST_CASE("uncertain_value.hpp")
{
    SUBCASE("uncertain_value_t")
    {
            auto
        a = 2.0;
            auto
        b = 3.0; 
            auto
        A = uncertain_value_t {4.0, 0.01};
            auto
        B = uncertain_value_t {5.0, 0.02};
            using
        U = uncertain_value_t <double>;
        {
                auto
            C = a * A + b * B;
            static_assert (std::is_same_v <decltype (C), U>);
            CHECK(C.value == a * A.value + b * B.value);
            CHECK(C.uncertainty == a * a * A.uncertainty + b * b * B.uncertainty);
        }
        {
                auto
            C = a * A - b * B;
            static_assert (std::is_same_v <decltype (C), U>);
            CHECK(C.value == a * A.value - b * B.value);
            CHECK(C.uncertainty == a * a * A.uncertainty + b * b * B.uncertainty);
        }
        {
                auto
            C = A * B;
            static_assert (std::is_same_v <decltype (C), U>);
            CHECK(C.value == A.value * B.value);
            CHECK(C.uncertainty == 
                  A.uncertainty * B.value * B.value 
                + B.uncertainty * A.value * A.value
            );
        }
        {
                auto
            C = A / B;
            static_assert (std::is_same_v <decltype (C), U>);
            CHECK(C.value == A.value / B.value);
            CHECK(C.uncertainty == 
                  A.uncertainty / B.value / B.value 
                + B.uncertainty * A.value * A.value / B.value / B.value / B.value / B.value 
            );
        }
        {
                auto
            C = pow (A, B);
            static_assert (std::is_same_v <decltype (C), U>);
            CHECK(C.value == pow (A.value, B.value));
            CHECK(C.uncertainty == doctest::Approx (C.value * C.value * (
                  pow (B.value / A.value, 2) * A.uncertainty
                + pow (log (A.value), 2) * B.uncertainty
            )));
                auto
            D = pow (A, U {B.value, 0});
                auto
            E = pow (A, B.value);
            CHECK (D.value == E.value);
            CHECK (D.uncertainty == E.uncertainty);
                auto
            F = pow (U {A.value, 0}, B);
                auto
            G = pow (A.value, B);
            CHECK (F.value == G.value);
            CHECK (F.uncertainty == G.uncertainty);
        }
        {
                auto
            C = log (A);
            static_assert (std::is_same_v <decltype (C), U>);
            CHECK(C.value == log (A.value));
            CHECK(C.uncertainty == A.uncertainty / A.value / A.value);
        }
        {
                auto
            C = exp (A);
            static_assert (std::is_same_v <decltype (C), U>);
            CHECK(C.value == exp (A.value));
            CHECK(C.uncertainty == A.uncertainty * exp (A.value) * exp (A.value));
        }

    }; 
    SUBCASE("mixed arithmetic")
    {
            constexpr auto
        a = uncertain_value_t <int> { 10, 1 };
            constexpr auto
        b = uncertain_value_t <double> { 1.0, 0.1 };
        static_assert (std::is_same_v <decltype (a + b), uncertain_value_t <double>>);
        static_assert (std::is_same_v <decltype (a - b), uncertain_value_t <double>>);
        static_assert (std::is_same_v <decltype (a * b), uncertain_value_t <double>>);
        static_assert (std::is_same_v <decltype (a / b), uncertain_value_t <double>>);
    };
    SUBCASE("Zero value")
    {
            using 
        uv = uncertain_value_t <double>;
            constexpr auto
        x = static_cast <uv> (0);
        static_assert (x == uncertain_value_t { 0.0, 0.0 });
    };   
    SUBCASE("conversion")
    {
            uncertain_value_t <int>
        a { 10, 1 };
        // FIXME: This is narrowing...
        /*
            uncertain_value_t <double>
        b { a };
        */
    };
}
