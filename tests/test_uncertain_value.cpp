#include <doctest/doctest.h>
#include "../include/calculisto/uncertain_value/uncertain_value.hpp"
    using calculisto::uncertain_value::uncertain_value_t;

TEST_CASE("uncertain_value.hpp")
{
    SUBCASE("uncertain_value_t")
    {
            auto const
        a = 2.0;
            auto const
        b = 3.0; 
            auto const
        A = uncertain_value_t {4.0, 0.01};
            auto const
        B = uncertain_value_t {5.0, 0.02};
            using
        U = const uncertain_value_t <double>;
        {
                auto const
            C = A;
            // doctest is not c++20 constraints ready.
            CHECK((C == A));
        }
        CHECK((A != B));
        CHECK((A < B));
        CHECK((A != a));
        CHECK((a < A));
        {
                auto const
            C = a * A + b * B;
            static_assert (std::is_same_v <decltype (C), U>);
            CHECK(C.value == a * A.value + b * B.value);
            CHECK(C.variance == a * a * A.variance + b * b * B.variance);
        }
        {
                auto const
            C = a * A - b * B;
            static_assert (std::is_same_v <decltype (C), U>);
            CHECK(C.value == a * A.value - b * B.value);
            CHECK(C.variance == a * a * A.variance + b * b * B.variance);
        }
        {
                auto const
            C = A + B;
            static_assert (std::is_same_v <decltype (C), U>);
            CHECK(C.value == A.value + B.value);
            CHECK(C.variance == A.variance + B.variance);
                auto
            D = A;
            D += B;
            CHECK(D.value == C.value);
            CHECK(D.variance == C.variance);
        }
        {
                auto const
            C = A - B;
            static_assert (std::is_same_v <decltype (C), U>);
            CHECK(C.value == A.value - B.value);
            CHECK(C.variance == A.variance + B.variance);
                auto
            D = A;
            D -= B;
            CHECK(D.value == C.value);
            CHECK(D.variance == C.variance);
        }
        {
                auto const
            C = A * B;
            static_assert (std::is_same_v <decltype (C), U>);
            CHECK(C.value == A.value * B.value);
            CHECK(C.variance == 
                  A.variance * B.value * B.value 
                + B.variance * A.value * A.value
            );
                auto
            D = A;
            D *= B;
            CHECK(D.value == C.value);
            CHECK(D.variance == C.variance);
        }
        {
                auto const
            C = A / B;
            static_assert (std::is_same_v <decltype (C), U>);
            CHECK(C.value == A.value / B.value);
            CHECK(C.variance == 
                  A.variance / B.value / B.value 
                + B.variance * A.value * A.value / B.value / B.value 
                    / B.value / B.value 
            );
                auto
            D = A;
            D /= B;
            CHECK(D.value == C.value);
            CHECK(D.variance == C.variance);
        }
        {
                auto const
            C = A + a;
            static_assert (std::is_same_v <decltype (C), U>);
            CHECK(C.value == A.value + a);
            CHECK(C.variance == A.variance);
                auto
            D = A;
            D += a;
            CHECK(D.value == C.value);
            CHECK(D.variance == C.variance);
        }
        {
                auto const
            C = A - a;
            static_assert (std::is_same_v <decltype (C), U>);
            CHECK(C.value == A.value - a);
            CHECK(C.variance == A.variance);
                auto
            D = A;
            D -= a;
            CHECK(D.value == C.value);
            CHECK(D.variance == C.variance);
        }
        {
                auto const
            C = A * a;
            static_assert (std::is_same_v <decltype (C), U>);
            CHECK(C.value == A.value * a);
            CHECK(C.variance == A.variance * a * a);
                auto
            D = A;
            D *= a;
            CHECK(D.value == C.value);
            CHECK(D.variance == C.variance);
        }{
                auto const
            C = A / a;
            static_assert (std::is_same_v <decltype (C), U>);
            CHECK(C.value == A.value / a);
            CHECK(C.variance == A.variance / a / a);
                auto
            D = A;
            D /= a;
            CHECK(D.value == C.value);
            CHECK(D.variance == C.variance);
        }
        {
                auto const
            C = pow (A, B);
            static_assert (std::is_same_v <decltype (C), U>);
            CHECK(C.value == pow (A.value, B.value));
            CHECK(C.variance == doctest::Approx (C.value * C.value * (
                  pow (B.value / A.value, 2) * A.variance
                + pow (log (A.value), 2) * B.variance
            )));
                auto
            D = pow (A, U {B.value, 0});
                auto
            E = pow (A, B.value);
            CHECK (D.value == E.value);
            CHECK (D.variance == E.variance);
                auto
            F = pow (U {A.value, 0}, B);
                auto
            G = pow (A.value, B);
            CHECK (F.value == G.value);
            CHECK (F.variance == G.variance);
        }
        {
                auto const
            C = log (A);
            static_assert (std::is_same_v <decltype (C), U>);
            CHECK(C.value == log (A.value));
            CHECK(C.variance == A.variance / A.value / A.value);
        }
        {
                auto const
            C = exp (A);
            static_assert (std::is_same_v <decltype (C), U>);
            CHECK(C.value == exp (A.value));
            CHECK(C.variance == A.variance * exp (A.value) * exp (A.value));
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
    SUBCASE("bnary function")
    {
            const uncertain_value_t <double>
        a { 10, 1 };
            const uncertain_value_t <double>
        b { 20, 1 };
            const auto
        c = fmin (a, b);
        CHECK(c == a);
            const auto
        d = fmin (b, 2);
        CHECK(d == uncertain_value_t <double> { 2., 0. });
    };
    /*
    SUBCASE("template pow")
    {
            constexpr auto
        x = pow <2> (uncertain_value_t { 2.0, 0.1 });
        static_assert (x.value == 4.0);
        // Approx
        // static_assert (x.uncertainty == 0.4);
    }
    */
}
