#pragma once
#include <type_traits>

namespace MTG {

/**
 *    Inherit from this class using CRTP to provide
 *    a unique type for an int metric.
 *    
 *    @tparam Super the superclass
 **/
template<typename Super>
struct Metric {
    Metric(int v = 0) : value{v} {}
    Metric(const Metric&) = default;
    bool    operator==(const Metric& rhs) const { return value == rhs.value; }
    bool    operator!=(const Metric& rhs) const { return value != rhs.value; }
    bool    operator> (int rhs)           const { return value > rhs; }
    bool    operator< (int rhs)           const { return value < rhs; }
    bool    operator<=(int rhs)           const { return value <= rhs; }
    bool    operator>=(int rhs)           const { return value >= rhs; }
    Metric  operator- (int rhs)           const { return { value - rhs }; }
    Metric  operator+ (int rhs)           const { return { value + rhs }; }
    Metric &operator-=(int rhs)                 { value -= rhs; return *this; }
    Metric &operator+=(int rhs)                 { value += rhs; return *this; }
    int value;    
};

/**
 * OR operator for an enum
 * 
 * @tparam TEnum the enum type
 * @param  a     the left  operand
 * @param  b     the right operand
 **/
template<typename TEnum>
TEnum operator|(TEnum a, TEnum b ) {
    using T = std::underlying_type_t<TEnum>;
    return (TEnum)((T)a | (T)b);
 }

/**
 * AND operator for an enum
 * 
 * @tparam TEnum the enum type
 * @param  a     the left  operand
 * @param  b     the right operand
 **/
template<typename TEnum>    
TEnum operator&(TEnum a, TEnum b ) {
    using T = std::underlying_type_t<TEnum>;
    return (TEnum)((T)a & (T)b);
 }
} // namespace MTG
