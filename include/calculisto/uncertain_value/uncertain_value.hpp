#pragma once
#include <numbers>
#include <concepts>
#include <compare>
#include <cmath>
#include <complex>

    namespace calculisto::uncertain_value
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
    variance = static_cast <T> (0);

        template <class U>
        requires std::convertible_to <U, T>
        constexpr
        explicit (false)
    uncertain_value_t (uncertain_value_t <U> const& a)
        : value { a.value }
        , variance { a.variance }
    {}

        constexpr
    uncertain_value_t () = default;

        constexpr
        explicit (false)
    uncertain_value_t (T const& a)
        : value { a }
    {}

    // ATTENTION: the parameter u (the uncertainty) is considered a variance,
    // the square of a standard deviation!
        constexpr
        explicit (false)
    uncertain_value_t (T const& v, T const& u)
        : value { v }
        , variance { u }
    {}
};
    template <class T>
uncertain_value_t (T, T) -> uncertain_value_t <T>;

    template <class T>
    requires std::swappable <T>
    constexpr void
swap (uncertain_value_t <T>& a, uncertain_value_t <T>& b) noexcept
{
        using std::swap;
    swap (a.value, b.value);
    swap (a.variance, b.variance);
}

    template <class T, class U>
    requires std::swappable_with <T&, U&>
    constexpr void
swap (uncertain_value_t <T>& a, uncertain_value_t <U>& b) noexcept
{
        using std::swap;
    swap (a.value, b.value);
    swap (a.variance, b.variance);
}

    template <class T, class U>
    requires std::equality_comparable_with <T, U>
    constexpr auto
operator == (uncertain_value_t <T> const& a, uncertain_value_t <U> const& b)
{
    return a.value == b.value && a.variance == b.variance;
}

    template <class T, class U>
    requires (!is_uncertain_value_v <U> && std::equality_comparable_with <T, U>)
    constexpr auto
operator == (uncertain_value_t <T> const& a, U const& b)
{
    return a.value == b;
}

    template <class T, class U>
    requires (!is_uncertain_value_v <T> && std::equality_comparable_with <T, U>)
    constexpr auto
operator == (T const& a, uncertain_value_t <U> const& b)
{
    return a == b.value;
}

    template <class T, class U>
    requires std::three_way_comparable_with <T, U>
    constexpr auto
operator <=> (uncertain_value_t <T> const& a, uncertain_value_t <U> const& b)
{
    return a.value <=> b.value;
}
    template <class T, class U>
    requires (!is_uncertain_value_v <U> && std::totally_ordered_with <T, U>)
    constexpr auto
operator <=> (uncertain_value_t <T> const& a, U const& b)
{
    return a.value <=> b;
}
    template <class T, class U>
    requires (!is_uncertain_value_v <T> && std::totally_ordered_with <T, U>)
    constexpr auto
operator <=> (T const& a, uncertain_value_t <U> const& b)
{
    return a <=> b.value;
}

// Arithmetic operations.
    template <
          class T
        , class U = decltype (+std::declval <T> ())
    >
    constexpr uncertain_value_t <U>
operator + (uncertain_value_t <T> const& a)
{
    return { +a.value, +a.variance };
}
    template <
          class T
        , class U = decltype (-std::declval <T> ())
    >
    constexpr uncertain_value_t <U>
operator - (uncertain_value_t <T> const& a)
{
    return { -a.value, -a.variance };
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
    return { a.value + b.value, a.variance + b.variance };
}
    template <
          class T
        , class U
        , class V = decltype (std::declval <T> () - std::declval <U> ())
    >
    constexpr uncertain_value_t <V>
operator - (uncertain_value_t <T> const& a, uncertain_value_t <U> const& b)
{
    return { a.value - b.value, a.variance + b.variance };
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
        , a.variance * b.value * b.value + b.variance * a.value * a.value 
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
        , a.value * a.value / b.value / b.value * (a.variance / a.value / a.value + b.variance / b.value / b.value) 
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
    a.variance += b.variance;
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
    a.variance += b.variance;
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
    a.variance *= b.value * b.value;
    a.variance += b.variance * a.value * a.value;
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
    a.variance += b.variance * a.value * a.value / b.value / b.value;
    a.variance /= b.value * b.value;
    a.value /= b.value;
    return a;
}
// 3.2.3. Binary, external.
    template <
          class T
        , class U
        , class V = decltype (std::declval <T> () + std::declval <U> ())
    >
    constexpr uncertain_value_t <V>
operator + (uncertain_value_t <T> const& a, U const& b)
{
    return { a.value + b, a.variance };
}
    template <
          class T
        , class U
        , class V = decltype (std::declval <T> () + std::declval <U> ())
    >
    constexpr uncertain_value_t <V>
operator + (T const& a, uncertain_value_t <U> const& b)
{
    return { a + b.value, b.variance };
}
    template <
          class T
        , class U
        , class V = decltype (std::declval <T> () - std::declval <U> ())
    >
    constexpr uncertain_value_t <V>
operator - (uncertain_value_t <T> const& a, U const& b)
{
    return { a.value - b, a.variance };
}
    template <
          class T
        , class U
        , class V = decltype (std::declval <T> () - std::declval <U> ())
    >
    constexpr uncertain_value_t <V>
operator - (T const& a, uncertain_value_t <U> const& b)
{
    return { a - b.value, b.variance };
}
    template <
          class T
        , class U
        , class V = decltype (std::declval <T> () * std::declval <U> ())
    >
    constexpr uncertain_value_t <V>
operator * (uncertain_value_t <T> const& a, U const& b)
{
    return { a.value * b, a.variance * b * b };
}
    template <
          class T
        , class U
        , class V = decltype (std::declval <T> () * std::declval <U> ())
    >
    constexpr uncertain_value_t <V>
operator * (T const& a, uncertain_value_t <U> const& b)
{
    return { a * b.value, b.variance * a * a };
}
    template <
          class T
        , class U
        , class V = decltype (std::declval <T> () / std::declval <U> ())
    >
    constexpr uncertain_value_t <V>
operator / (uncertain_value_t <T> const& a, U const& b)
{
    return { a.value / b, a.variance / b / b };
}
    template <
          class T
        , class U
        , class V = decltype (std::declval <T> () / std::declval <U> ())
    >
    constexpr uncertain_value_t <V>
operator / (T const& a, uncertain_value_t <U> const& b)
{
    return { a / b.value, b.variance * a * a / pow (b.value, 4)};
}
// Compound assignement
    template <
          class T
        , class U
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
        , class V = decltype (std::declval <T> () + std::declval <U> ())
    >
    constexpr uncertain_value_t <V>
operator *= (uncertain_value_t <T>& a, U const& b)
{
    a.value *= b;
    a.variance *= b * b;
    return a;
}
    template <
          class T
        , class U
        , class V = decltype (std::declval <T> () + std::declval <U> ())
    >
    constexpr uncertain_value_t <V>
operator /= (uncertain_value_t <T>& a, U const& b)
{
    a.value /= b;
    a.variance /= b * b;
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
    a.variance = a.variance * a.value * a.value;
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
    a.variance = a.variance * a.value * a.value 
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
    return { expm1 (a.value), a.variance * x * x};
}
    template <
          class T
        , class U = decltype (log (std::declval <T> ()))
    >
    constexpr uncertain_value_t <U>
log (uncertain_value_t <T> a)
{
    a.variance = a.variance / a.value / a.value;
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
    a.variance = a.variance / a.value / a.value
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
    a.variance = a.variance / a.value / a.value
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
    a.variance = a.variance / a.value / a.value;
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
    a.variance = a.variance / a.value / static_cast <T> (4);
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
    a.variance = a.variance 
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
    a.variance = a.variance * cos (a.value) * cos (a.value);
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
    a.variance = a.variance * sin (a.value) * sin (a.value);
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
    a.variance = a.variance / cos (a.value) / cos (a.value);
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
        , class V = std::common_type_t <T, U>
    >
    constexpr auto
fmin (uncertain_value_t <T> a, uncertain_value_t <U> b)
{
    if (a.value < b.value) 
        return uncertain_value_t <V> { a.value, a.variance };
    return uncertain_value_t <V> { b.value, b.variance };
}
    template <
          class T 
        , class U
        , class V = std::common_type_t <T, U>
    >
    constexpr auto
fmin (uncertain_value_t <T> a, U const& b)
{
    if (a.value < b) 
        return uncertain_value_t <V> { a.value, a.variance };
    return uncertain_value_t <V> { b, 0 };
}
    template <
          class T 
        , class U
        , class V = std::common_type_t <T, U>
    >
    constexpr auto
fmin (T const& a, uncertain_value_t <U> b)
{
    if (a < b.value) 
        return uncertain_value_t <V> { a, 0 };
    return uncertain_value_t <V> { b.value, b.variance };
}

    template <
          class T
        , class U
        , class V = decltype (pow (std::declval <T> (), std::declval <U> ()))
    >
    constexpr uncertain_value_t <V>
pow (uncertain_value_t <T> a, uncertain_value_t <U> const& b)
{
    if (b.value == 0)
    {
        a.value = 1;
        a.variance = 0;
        return a;
    }
    if (a.value == 0)
    {
        a.value = 0;
        a.variance = 0;
        return a;
    }
    a.variance = a.variance * b.value * b.value / a.value / a.value
        + b.variance * log (a.value) * log (a.value);
    a.value = pow (a.value, b.value); 
    a.variance = a.variance * a.value * a.value;
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
    if (b == 0)
    {
        a.value = 1;
        a.variance = 0;
        return a;
    }
    if (a.value == 0)
    {
        a.value = 0;
        a.variance = 0;
        return a;
    }
    a.variance = a.variance / a.value / a.value * b * b;
    a.value = pow (a.value, b);
    a.variance = a.variance * a.value * a.value;
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
    if (b.value == 0)
    {
        b.value = 1;
        b.variance = 0;
        return b;
    }
    if (a == 0)
    {
        b.value = 0;
        b.variance = 0;
        return b;
    }
    b.variance = b.variance * log (a) * log (a);
    b.value = pow (a, b.value);
    b.variance = b.variance * b.value * b.value;
    return b;
}

/*
    template <
          auto Exponent
        , class T
    >
    constexpr auto
pow (uncertain_value_t <T> a)
{
        using calculisto::template_pow::pow;
    a.uncertainty = a.uncertainty / a.value / a.value * Exponent * Exponent;
    a.value = pow <Exponent> (a.value);
    a.uncertainty = a.uncertainty * a.value * a.value;
    return a;
}
*/
/*
BINARY_FUNCTION(fmod)
BINARY_FUNCTION(remainder)
BINARY_FUNCTION(fmax)
BINARY_FUNCTION(fmin)
BINARY_FUNCTION(fdim)
BINARY_FUNCTION(hypot)
BINARY_FUNCTION(atan2)
BINARY_FUNCTION(nextafter)
BINARY_FUNCTION(copysign)
*/
#undef BINARY_FUNCTION
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
// Interaction with std::complex
    template <class T>
    constexpr auto
real (uncertain_value_t <std::complex <T>> const& a)
{
    return uncertain_value_t <T> { real (a.value), real (a.variance) };
}
    template <class T>
    constexpr auto
log (uncertain_value_t <std::complex <T>> a)
{
    a.variance = a.variance / a.value / a.value;
    a.value = log (a.value);
    return a;
}
} // namespace calculisto::uncertain_value
