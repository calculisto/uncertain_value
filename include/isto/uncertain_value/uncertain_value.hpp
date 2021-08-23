#pragma once
#include <numbers>
#include <isto/template_pow/template_pow.hpp>

    namespace isto::uncertain_value
{
    template <class T>
    struct
uncertain_value_t;

    template <class T>
    constexpr bool
is_uncertain_value_v = false;

    template <class T>
    constexpr bool
is_uncertain_value_v <uncertain_value_t <T>> = true;

    template <class T>
    concept
uncertain_value = is_uncertain_value_v <T>;

// (The uncertainty is a variance.)
    template <class T>
    struct
uncertain_value_t
{
    static_assert (!is_uncertain_value_v <T>);
        using
    value_type = T;
        value_type
    value = T {};
        value_type
    uncertainty = static_cast <T> (0);

        template <
              class U
            , class = std::enable_if_t <std::is_convertible_v <U, T>>
        >
        constexpr
        explicit (false)
    uncertain_value_t (uncertain_value_t <U> const& a)
        : value { a.value }
        , uncertainty { a.uncertainty }
    {}

        constexpr
    uncertain_value_t () = default;

        constexpr
        explicit (false)
    uncertain_value_t (T const& a)
        : value { a }
    {}
        constexpr
        explicit (false)
    uncertain_value_t (T const& a, T const& b)
        : value { a }
        , uncertainty { b }
    {}
};
    template <class T>
uncertain_value_t (T, T) -> uncertain_value_t <T>;
// Regularity.
    template <
          class T
        , class U
        , class = std::enable_if_t <std::equality_comparable_with <T, U>>
    >
    constexpr auto
operator == (uncertain_value_t <T> const& a, uncertain_value_t <U> const& b)
{
    return a.value == b.value && a.uncertainty == b.uncertainty;
}
    template <
          class T
        , class U
        , class = std::enable_if_t <std::equality_comparable_with <T, U>>
    >
    constexpr auto
operator != (uncertain_value_t <T> const& a, uncertain_value_t <U> const& b)
{
    return a.value != b.value || a.uncertainty != b.uncertainty;
}
// Ok, we're regular.

        template <
              class T
            , class U
            , class = std::enable_if_t <!is_uncertain_value_v <U>>
        >
    //requires (!uncertain_value <U> && std::equality_comparable_with <T, U>)
    constexpr auto
operator == (uncertain_value_t <T> const& a, U const& b)
{
    return a.value == b;
}
        template <
              class T
            , class U
            , class = std::enable_if_t <!is_uncertain_value_v <U>>
        >
    //requires (!uncertain_value <U> && std::equality_comparable_with <T, U>)
    constexpr auto
operator != (uncertain_value_t <T> const& a, U const& b)
{
    return a.value != b;
}
        template <
              class T
            , class U
            , class = std::enable_if_t <!is_uncertain_value_v <T>>
        >
    //requires (!uncertain_value <T> && std::equality_comparable_with <T, U>)
    constexpr auto
operator == (T const& a, uncertain_value_t <U> const& b)
{
    return a == b.value;
}
        template <
              class T
            , class U
            , class = std::enable_if_t <!is_uncertain_value_v <T>>
        >
    //requires (!uncertain_value <T> && std::equality_comparable_with <T, U>)
    constexpr auto
operator != (T const& a, uncertain_value_t <U> const& b)
{
    return a != b.value;
}
    template <class T, class U>
    //requires std::totally_ordered_with <T, U>
    constexpr auto
operator < (uncertain_value_t <T> const& a, uncertain_value_t <U> const& b)
{
    return a.value < b.value;
}
    template <class T, class U>
    //requires std::totally_ordered_with <T, U>
    constexpr auto
operator > (uncertain_value_t <T> const& a, uncertain_value_t <U> const& b)
{
    return a.value > b.value;
}
    template <class T, class U>
    //requires std::totally_ordered_with <T, U>
    constexpr auto
operator <= (uncertain_value_t <T> const& a, uncertain_value_t <U> const& b)
{
    return a.value <= b.value;
}
    template <class T, class U>
    //requires std::totally_ordered_with <T, U>
    constexpr auto
operator >= (uncertain_value_t <T> const& a, uncertain_value_t <U> const& b)
{
    return a.value >= b.value;
}
    template <class T, class U>
    //requires (!is_uncertain_value_v <U> && std::totally_ordered_with <T, U>)
    constexpr auto
operator < (uncertain_value_t <T> const& a, U const& b)
{
    return a.value < b;
}
    template <class T, class U>
    //requires (!is_uncertain_value_v <U> && std::totally_ordered_with <T, U>)
    constexpr auto
operator > (uncertain_value_t <T> const& a, U const& b)
{
    return a.value > b;
}
    template <class T, class U>
    //requires (!is_uncertain_value_v <U> && std::totally_ordered_with <T, U>)
    constexpr auto
operator <= (uncertain_value_t <T> const& a, U const& b)
{
    return a.value <= b;
}
    template <class T, class U>
    //requires (!is_uncertain_value_v <U> && std::totally_ordered_with <T, U>)
    constexpr auto
operator >= (uncertain_value_t <T> const& a, U const& b)
{
    return a.value >= b;
}
    template <class T, class U>
    //requires (!is_uncertain_value_v <T> && std::totally_ordered_with <T, U>)
    constexpr auto
operator < (T const& a, uncertain_value_t <U> const& b)
{
    return a < b.value;
}
    template <class T, class U>
    //requires (!is_uncertain_value_v <T> && std::totally_ordered_with <T, U>)
    constexpr auto
operator > (T const& a, uncertain_value_t <U> const& b)
{
    return a > b.value;
}
    template <class T, class U>
    //requires (!is_uncertain_value_v <T> && std::totally_ordered_with <T, U>)
    constexpr auto
operator <= (T const& a, uncertain_value_t <U> const& b)
{
    return a <= b.value;
}
    template <class T, class U>
    //requires (!is_uncertain_value_v <T> && std::totally_ordered_with <T, U>)
    constexpr auto
operator >= (T const& a, uncertain_value_t <U> const& b)
{
    return a >= b.value;
}
// Ok, we're strict_totally_ordered
// Arithmetic operations.
    template <
          class T
        , class U = decltype (+std::declval <T> ())
    >
    constexpr uncertain_value_t <U>
operator + (uncertain_value_t <T> const& a)
{
    return { +a.value, +a.uncertainty };
}
    template <
          class T
        , class U = decltype (-std::declval <T> ())
    >
    constexpr uncertain_value_t <U>
operator - (uncertain_value_t <T> const& a)
{
    return { -a.value, -a.uncertainty };
}
// 3.2.2. Binary, internal.
    template <
          class T
        , class U
        , class V = decltype (std::declval <T> () + std::declval <U> ())
    >
    constexpr uncertain_value_t <V>
operator + (uncertain_value_t <T> const& a, uncertain_value_t <U> const& b)
{
    return { a.value + b.value, a.uncertainty + b.uncertainty };
}
    template <
          class T
        , class U
        , class V = decltype (std::declval <T> () - std::declval <U> ())
    >
    constexpr uncertain_value_t <V>
operator - (uncertain_value_t <T> const& a, uncertain_value_t <U> const& b)
{
    return { a.value - b.value, a.uncertainty + b.uncertainty };
}
    template <
          class T
        , class U
        , class V = decltype (std::declval <T> () * std::declval <U> ())
    >
    constexpr uncertain_value_t <V>
operator * (uncertain_value_t <T> const& a, uncertain_value_t <U> const& b)
{
    return { 
          a.value * b.value 
        , a.uncertainty * b.value * b.value + b.uncertainty * a.value * a.value 
    };
}
    template <
          class T
        , class U
        , class V = decltype (std::declval <T> () / std::declval <U> ())
    >
    constexpr uncertain_value_t <V>
operator / (uncertain_value_t <T> const& a, uncertain_value_t <U> const& b)
{
    // Zero has no uncertainty.
    if (a.value == static_cast <T> (0)) return { 0, 0 };
    return { 
          a.value / b.value 
        , a.value * a.value / b.value / b.value * (a.uncertainty / a.value / a.value + b.uncertainty / b.value / b.value) 
    };
}
// Compound assignement
    template <
          class T
        , class U
        , class V = decltype (std::declval <T> () + std::declval <U> ())
    >
    constexpr uncertain_value_t <V>&
operator += (uncertain_value_t <T>& a, uncertain_value_t <U> const& b)
{
    a.value += b.value;
    a.uncertainty += b.uncertainty;
    return a;
}
    template <
          class T
        , class U
        , class V = decltype (std::declval <T> () + std::declval <U> ())
    >
    constexpr uncertain_value_t <V>&
operator -= (uncertain_value_t <T>& a, uncertain_value_t <U> const& b)
{
    a.value -= b.value;
    a.uncertainty += b.uncertainty;
    return a;
}
    template <
          class T
        , class U
        , class V = decltype (std::declval <T> () + std::declval <U> ())
    >
    constexpr uncertain_value_t <V>&
operator *= (uncertain_value_t <T>& a, uncertain_value_t <U> const& b)
{
    a.uncertainty *= b.value * b.value;
    a.uncertainty += b.uncertainty * a.value * a.value;
    a.value *= b.value;
    return a;
}
    template <
          class T
        , class U
        , class V = decltype (std::declval <T> () + std::declval <U> ())
    >
    constexpr uncertain_value_t <V>&
operator /= (uncertain_value_t <T>& a, uncertain_value_t <U> const& b)
{
    a.uncertainty += b.uncertainty * a.value * a.value / b.value / b.value;
    a.uncertainty /= b.value * b.value;
    a.value /= b.value;
    return a;
}
// 3.2.3. Binary, external.
    template <
          class T
        , class U
//        , class = std::enable_if_t <is_arithmetic_type_v <U>>
        , class V = decltype (std::declval <T> () + std::declval <U> ())
    >
    constexpr uncertain_value_t <V>
operator + (uncertain_value_t <T> const& a, U const& b)
{
    return { a.value + b, a.uncertainty };
}
    template <
          class T
        , class U
//        , class = std::enable_if_t <is_arithmetic_type_v <T>>
        , class V = decltype (std::declval <T> () + std::declval <U> ())
    >
    constexpr uncertain_value_t <V>
operator + (T const& a, uncertain_value_t <U> const& b)
{
    return { a + b.value, b.uncertainty };
}
    template <
          class T
        , class U
//        , class = std::enable_if_t <is_arithmetic_type_v <U>>
        , class V = decltype (std::declval <T> () - std::declval <U> ())
    >
    constexpr uncertain_value_t <V>
operator - (uncertain_value_t <T> const& a, U const& b)
{
    return { a.value - b, a.uncertainty };
}
    template <
          class T
        , class U
//        , class = std::enable_if_t <is_arithmetic_type_v <T>>
        , class V = decltype (std::declval <T> () - std::declval <U> ())
    >
    constexpr uncertain_value_t <V>
operator - (T const& a, uncertain_value_t <U> const& b)
{
    return { a - b.value, b.uncertainty };
}
    template <
          class T
        , class U
//        , class = std::enable_if_t <is_arithmetic_type_v <U>>
        , class V = decltype (std::declval <T> () * std::declval <U> ())
    >
    constexpr uncertain_value_t <V>
operator * (uncertain_value_t <T> const& a, U const& b)
{
    return { a.value * b, a.uncertainty * b * b };
}
    template <
          class T
        , class U
//        , class = std::enable_if_t <is_arithmetic_type_v <T>>
        , class V = decltype (std::declval <T> () * std::declval <U> ())
    >
    constexpr uncertain_value_t <V>
operator * (T const& a, uncertain_value_t <U> const& b)
{
    return { a * b.value, b.uncertainty * a * a };
}
    template <
          class T
        , class U
//        , class = std::enable_if_t <is_arithmetic_type_v <U>>
        , class V = decltype (std::declval <T> () / std::declval <U> ())
    >
    constexpr uncertain_value_t <V>
operator / (uncertain_value_t <T> const& a, U const& b)
{
    return { a.value / b, a.uncertainty / b / b };
}
    template <
          class T
        , class U
//        , class = std::enable_if_t <is_arithmetic_type_v <T>>
        , class V = decltype (std::declval <T> () / std::declval <U> ())
    >
    constexpr uncertain_value_t <V>
operator / (T const& a, uncertain_value_t <U> const& b)
{
    return { a / b.value, b.uncertainty * a * a / pow (b.value, 4)};
}
// Compound assignement
    template <
          class T
        , class U
//        , class = std::enable_if_t <is_arithmetic_type_v <U>>
        , class V = decltype (std::declval <T> () + std::declval <U> ())
    >
    constexpr uncertain_value_t <V>
operator += (uncertain_value_t <T>& a, U const& b)
{
    a.value += b;
    return a;
}
    template <
          class T
        , class U
//        , class = std::enable_if_t <is_arithmetic_type_v <U>>
        , class V = decltype (std::declval <T> () + std::declval <U> ())
    >
    constexpr uncertain_value_t <V>
operator -= (uncertain_value_t <T>& a, U const& b)
{
    a.value -= b;
    return a;
}
    template <
          class T
        , class U
//        , class = std::enable_if_t <is_arithmetic_type_v <U>>
        , class V = decltype (std::declval <T> () + std::declval <U> ())
    >
    constexpr uncertain_value_t <V>
operator *= (uncertain_value_t <T>& a, U const& b)
{
    a.value *= b;
    a.uncertainty *= b * b;
    return a;
}
    template <
          class T
        , class U
//        , class = std::enable_if_t <is_arithmetic_type_v <U>>
        , class V = decltype (std::declval <T> () + std::declval <U> ())
    >
    constexpr uncertain_value_t <V>
operator /= (uncertain_value_t <T>& a, U const& b)
{
    a.value /= b;
    a.uncertainty /= b * b;
    return a;
}

// 3.2.4. cmath functions
    using
      std::fabs
    , std::fmod
    , std::remainder
    , std::remquo
    , std::fma
    , std::fmax
    , std::fmin
    , std::fdim
    , std::nan
    , std::exp
    , std::exp2
    , std::expm1
    , std::log
    , std::log10
    , std::log2
    , std::log1p
    , std::pow
    , std::sqrt
    , std::cbrt
    , std::hypot
    , std::hypot
    , std::sin
    , std::cos
    , std::tan
    , std::asin
    , std::acos
    , std::atan
    , std::atan2
    , std::sinh
    , std::cosh
    , std::tanh
    , std::asinh
    , std::acosh
    , std::atanh
    , std::erf
    , std::erfc
    , std::tgamma
    , std::lgamma
    , std::ceil
    , std::floor
    , std::trunc
    , std::round
    , std::nearbyint
    , std::rint
    , std::frexp
    , std::ldexp
    , std::modf
    , std::scalbn
    , std::ilogb
    , std::logb
    , std::nextafter
    , std::copysign
    ; 
// 3.2.4.1 Unary.
    template <
          class T
        , class U = decltype (fabs (std::declval <T> ()))
    >
    constexpr uncertain_value_t <U>
fabs (uncertain_value_t <T> const& a)
{
    return { fabs (a.value) };
}
    template <
          class T
        , class U = decltype (exp (std::declval <T> ()))
    >
    constexpr uncertain_value_t <U>
exp (uncertain_value_t <T> a)
{
    a.value = exp (a.value);
    a.uncertainty = a.uncertainty * a.value * a.value;
    return a;
}
    template <
          class T
        , class U = decltype (exp2 (std::declval <T> ()))
    >
    constexpr uncertain_value_t <U>
exp2 (uncertain_value_t <T> a)
{
    a.value = exp2 (a.value);
    a.uncertainty = a.uncertainty * a.value * a.value 
         * std::numbers::ln2_v <T> * std::numbers::ln2_v <T>;
    return a;
}
    template <
          class T
        , class U = decltype (expm1 (std::declval <T> ()))
    >
    constexpr uncertain_value_t <U>
expm1 (uncertain_value_t <T> const a)
{
        auto
    x = exp (a.value);
    return { expm1 (a.value), a.uncertainty * x * x};
}
    template <
          class T
        , class U = decltype (log (std::declval <T> ()))
    >
    constexpr uncertain_value_t <U>
log (uncertain_value_t <T> a)
{
    a.uncertainty = a.uncertainty / a.value / a.value;
    a.value = log (a.value);
    return a;
}
    template <
          class T
        , class U = decltype (log10 (std::declval <T> ()))
    >
    constexpr uncertain_value_t <U>
log10 (uncertain_value_t <T> a)
{
    a.uncertainty = a.uncertainty / a.value / a.value
        / std::numbers::ln10_v <T> / std::numbers::ln10_v <T>;
    a.value = log10 (a.value);
    return a;
}
    template <
          class T
        , class U = decltype (log2 (std::declval <T> ()))
    >
    constexpr uncertain_value_t <U>
log2 (uncertain_value_t <T> a)
{
    a.uncertainty = a.uncertainty / a.value / a.value
        / std::numbers::ln2_v <T> / std::numbers::ln2_v <T>;
    a.value = log2 (a.value);
    return a;
}
    template <
          class T
        , class U = decltype (log1p (std::declval <T> ()))
    >
    constexpr uncertain_value_t <U>
log1p (uncertain_value_t <T> a)
{
    a.uncertainty = a.uncertainty / a.value / a.value;
    a.value = log1p (a.value);
    return a;
}
    template <
          class T
        , class U = decltype (sqrt (std::declval <T> ()))
    >
    constexpr uncertain_value_t <U>
sqrt (uncertain_value_t <T> a)
{
    a.uncertainty = a.uncertainty / a.value / static_cast <T> (4);
    a.value = sqrt (a.value);
    return a;
}
    template <
          class T
        , class U = decltype (cbrt (std::declval <T> ()))
    >
    constexpr uncertain_value_t <U>
cbrt (uncertain_value_t <T> a)
{
    a.uncertainty = a.uncertainty 
        / pow (a.value, static_cast <T> (4.0 / 3.0))
        / static_cast <T> (9)
    ;
    a.value = cbrt (a.value);
    return a;
}
    template <
          class T
        , class U = decltype (sin (std::declval <T> ()))
    >
    constexpr uncertain_value_t <U>
sin (uncertain_value_t <T> a)
{
    a.uncertainty = a.uncertainty * cos (a.value) * cos (a.value);
    a.value = sin (a.value);
    return a;
}
    template <
          class T
        , class U = decltype (cos (std::declval <T> ()))
    >
    constexpr uncertain_value_t <U>
cos (uncertain_value_t <T> a)
{
    a.uncertainty = a.uncertainty * sin (a.value) * sin (a.value);
    a.value = cos (a.value);
    return a;
}
    template <
          class T
        , class U = decltype (tan (std::declval <T> ()))
    >
    constexpr uncertain_value_t <U>
tan (uncertain_value_t <T> a)
{
    a.uncertainty = a.uncertainty / cos (a.value) / cos (a.value);
    a.value = tan (a.value);
    return a;
}
/* TODO
UNARY_FUNCTION(asin)
UNARY_FUNCTION(acos)
UNARY_FUNCTION(atan)
UNARY_FUNCTION(sinh)
UNARY_FUNCTION(cosh)
UNARY_FUNCTION(tanh)
UNARY_FUNCTION(asinh)
UNARY_FUNCTION(acosh)
UNARY_FUNCTION(atanh)
UNARY_FUNCTION(erf)
UNARY_FUNCTION(erfc)
UNARY_FUNCTION(tgamma)
UNARY_FUNCTION(lgamma)
UNARY_FUNCTION(ceil)
UNARY_FUNCTION(floor)
UNARY_FUNCTION(trunc)
UNARY_FUNCTION(round)
UNARY_FUNCTION(nearbyint)
UNARY_FUNCTION(rint)
    template <class T>
    constexpr int
ilogb (uncertain_value_t <T> const& a)
{
    return { ilogb (a.value) };
} 
UNARY_FUNCTION(logb)
#undef UNARY_FUNCTION
*/
// 3.2.4.1 Binary.
#define BINARY_FUNCTION(f)                                                  \
    template <                                                              \
          class T                                                           \
        , class U                                                           \
        , class V = decltype (f (std::declval <T> (), std::declval <U> ())) \
    >                                                                       \
    constexpr uncertain_value_t <V>                                         \
f (uncertain_value_t <T> const& a, uncertain_value_t <U> const& b)          \
{                                                                           \
    return { f (a.value, b.value) };                                        \
}                                                                           \
    template <                                                              \
          class T                                                           \
        , class U                                                           \
        , class V = decltype (f (std::declval <T> (), std::declval <U> ())) \
    >                                                                       \
    constexpr uncertain_value_t <V>                                         \
f (uncertain_value_t <T> const& a, U const& b)                              \
{                                                                           \
    return { f (a.value, b) };                                              \
}                                                                           \
    template <                                                              \
          class T                                                           \
        , class U                                                           \
        , class V = decltype (f (std::declval <T> (), std::declval <U> ())) \
    >                                                                       \
    constexpr uncertain_value_t <V>                                         \
f (T const& a, uncertain_value_t <U> const& b)                              \
{                                                                           \
    return { f (a, b.value) };                                              \
}

    template <
          class T
        , class U
        , class V = decltype (pow (std::declval <T> (), std::declval <U> ()))
    >
    constexpr uncertain_value_t <V>
pow (uncertain_value_t <T> a, uncertain_value_t <U> const& b)
{
    if (a.value == static_cast <T> (0))
    {
        // TODO
    }
    a.uncertainty = a.uncertainty * b.value * b.value / a.value / a.value
        + b.uncertainty * log (a.value) * log (a.value);
    a.value = pow (a.value, b.value); 
    a.uncertainty = a.uncertainty * a.value * a.value;
    return a;
}
    template <
          class T
        , class U
        , class V = decltype (pow (std::declval <T> (), std::declval <U> ()))
    >
    constexpr uncertain_value_t <V>
pow (uncertain_value_t <T> a, U const& b)
{
    if (a.value == static_cast <T> (0))
    {
        // TODO
        return static_cast <T> (0);
    }
    a.uncertainty = a.uncertainty / a.value / a.value * b * b;
    a.value = pow (a.value, b);
    a.uncertainty = a.uncertainty * a.value * a.value;
    return a;
}
    template <
          class T
        , class U
        , class V = decltype (pow (std::declval <T> (), std::declval <U> ()))
    >
    constexpr uncertain_value_t <V>
pow (T const& a, uncertain_value_t <U> b)
{
    if (a == static_cast <T> (0))
    {
        // TODO
    }
    b.uncertainty = b.uncertainty * log (a) * log (a);
    b.value = pow (a, b.value);
    b.uncertainty = b.uncertainty * b.value * b.value;
    return b;
}

    template <
          auto Exponent
        , class T
    >
    constexpr auto
pow (uncertain_value_t <T> a)
{
        using isto::template_pow::pow;
    a.uncertainty = a.uncertainty / a.value / a.value * Exponent * Exponent;
    a.value = pow <Exponent> (a.value);
    a.uncertainty = a.uncertainty * a.value * a.value;
    return a;
}

/* TODO
BINARY_FUNCTION(fmod)
BINARY_FUNCTION(remainder)
BINARY_FUNCTION(fmax)
BINARY_FUNCTION(fmin)
BINARY_FUNCTION(fdim)
BINARY_FUNCTION(hypot)
BINARY_FUNCTION(atan2)
BINARY_FUNCTION(nextafter)
BINARY_FUNCTION(copysign)
#undef BINARY_FUNCTION
*/
// 3.2.4.1 Ternary.
/* TODO
#define TERNARY_FUNCTION(f)                   \
    template <                                \
          class T                             \
        , class U                             \
        , class V                             \
        , class W = decltype (f (             \
              std::declval <T> ()             \
            , std::declval <U> ()             \
            , std::declval <V> ()             \
          ))                                  \
    >                                         \
    constexpr uncertain_value_t <W>             \
f (                                           \
      uncertain_value_t <T> const& a            \
    , uncertain_value_t <U> const& b            \
    , uncertain_value_t <V> const& c            \
){                                            \
    return { f (a.value, b.value, c.value) }; \
}                                             \
    template <                                \
          class T                             \
        , class U                             \
        , class V                             \
        , class W = decltype (f (             \
              std::declval <T> ()             \
            , std::declval <U> ()             \
            , std::declval <V> ()             \
          ))                                  \
    >                                         \
    constexpr uncertain_value_t <W>             \
f (                                           \
      uncertain_value_t <T> const& a            \
    , uncertain_value_t <U> const& b            \
    ,     V  const& c                         \
){                                            \
    return { f (a.value, b.value, c      ) }; \
}                                             \
    template <                                \
          class T                             \
        , class U                             \
        , class V                             \
        , class W = decltype (f (             \
              std::declval <T> ()             \
            , std::declval <U> ()             \
            , std::declval <V> ()             \
          ))                                  \
    >                                         \
    constexpr uncertain_value_t <W>             \
f (                                           \
      uncertain_value_t <T> const& a            \
    ,     U  const& b                         \
    , uncertain_value_t <V> const& c            \
){                                            \
    return { f (a.value, b      , c.value) }; \
}                                             \
    template <                                \
          class T                             \
        , class U                             \
        , class V                             \
        , class W = decltype (f (             \
              std::declval <T> ()             \
            , std::declval <U> ()             \
            , std::declval <V> ()             \
          ))                                  \
    >                                         \
    constexpr uncertain_value_t <W>             \
f (                                           \
          T  const& a                         \
    , uncertain_value_t <U> const& b            \
    , uncertain_value_t <V> const& c            \
){                                            \
    return { f (a      , b.value, c.value) }; \
}                                             \
    template <                                \
          class T                             \
        , class U                             \
        , class V                             \
        , class W = decltype (f (             \
              std::declval <T> ()             \
            , std::declval <U> ()             \
            , std::declval <V> ()             \
          ))                                  \
    >                                         \
    constexpr uncertain_value_t <W>             \
f (                                           \
      uncertain_value_t <T> const& a            \
    ,     U  const& b                         \
    ,     V  const& c                         \
){                                            \
    return { f (a.value, b      , c      ) }; \
}                                             \
    template <                                \
          class T                             \
        , class U                             \
        , class V                             \
        , class W = decltype (f (             \
              std::declval <T> ()             \
            , std::declval <U> ()             \
            , std::declval <V> ()             \
          ))                                  \
    >                                         \
    constexpr uncertain_value_t <W>             \
f (                                           \
          T  const& a                         \
    , uncertain_value_t <U> const& b            \
    ,     V  const& c                         \
){                                            \
    return { f (a      , b.value, c      ) }; \
}                                             \
    template <                                \
          class T                             \
        , class U                             \
        , class V                             \
        , class W = decltype (f (             \
              std::declval <T> ()             \
            , std::declval <U> ()             \
            , std::declval <V> ()             \
          ))                                  \
    >                                         \
    constexpr uncertain_value_t <W>             \
f (                                           \
          T  const& a                         \
    ,     U  const& b                         \
    , uncertain_value_t <V> const& c            \
){                                            \
    return { f (a      , b      , c.value) }; \
}                                             \
TERNARY_FUNCTION(fma)
TERNARY_FUNCTION(hypot)
#undef TERNARY_FUNCTION
#define MIXED_BINARY_FUNCTION(F,TYPE)                                          \
    template <                                                                 \
          class T                                                              \
        , class V = decltype (F (std::declval <T> (), std::declval <TYPE> ())) \
    >                                                                          \
    constexpr uncertain_value_t <V>                                                   \
F (uncertain_value_t <T> const& a, TYPE b)                                            \
{                                                                              \
    return { F (a.value, b) };                                                 \
}                                                                              \
MIXED_BINARY_FUNCTION(frexp, int*)
MIXED_BINARY_FUNCTION(ldexp, int)
    template <
          class T
        , class PromotedFloatType = promoted_float_type <T>
        , class V = decltype (modf (std::declval <T> (), std::declval <PromotedFloatType*> ()))
    >
    constexpr uncertain_value_t <V>
modf (uncertain_value_t <T> const& a, uncertain_value_t <PromotedFloatType>* b)
{
    return { modf (a.value, &(b->value)) };
}
MIXED_BINARY_FUNCTION(scalbn, int)
    template <
          class T
        , class U
        , class V = decltype (remquo (
              std::declval <T> ()
            , std::declval <U> ()
            , std::declval <int*> ()
          ))
    >
    constexpr uncertain_value_t <V>
remquo (uncertain_value_t <T> const& a,  uncertain_value_t <U> const& b, int * c)
{
    return { remquo (a.value, b.value, c) };
}
    template <
          class T
        , class U
        , class V = decltype (remquo (
              std::declval <T> ()
            , std::declval <U> ()
            , std::declval <int*> ()
          ))
    >
    constexpr uncertain_value_t <V>
remquo (uncertain_value_t <T> const& a,  U const& b, int * c)
{
    return { remquo (a.value, b, c) };
}
    template <
          class T
        , class U
        , class V = decltype (remquo (
              std::declval <T> ()
            , std::declval <U> ()
            , std::declval <int*> ()
          ))
    >
    constexpr uncertain_value_t <V>
remquo (T const& a,  uncertain_value_t <U> const& b, int * c)
{
    return { remquo (a, b.value, c) };
}
*/
} // namespace isto::uncertain_value

/*
    namespace
isto::misc
{
        using namespace isto::quantity;

        template <class T>
        struct
    promoted_integer_type <uncertain_value_t <T>>
    {
            using
        type = uncertain_value_t <promoted_integer_type_t <T>>;
    };
        template <class... T>
        requires (std::disjunction_v <is_uncertain_value <T>...>)
        struct
    promoted_float_type <T...>
    {
            using
        type = uncertain_value_t <promoted_float_type_t <remove_uncertain_value_t <T>...>>;
    };
} // namespace isto::misc
//static_assert (isto::misc::is_arithmetic_v <isto::quantity::uncertain_value_t <double>>);
*/
/*
    namespace
isto::misc
{
        template <class T>
        struct
    is_arithmetic_type <isto::quantity::uncertain_value_t <T>>
    {
            static constexpr bool
        value = is_arithmetic_type_v <T>;
    };
}
  */  
