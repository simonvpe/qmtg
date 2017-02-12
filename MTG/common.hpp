#pragma once
#include <type_traits>

namespace MTG {

/**
 *    Inherit from this class using CRTP to provide
 *    a unique type for an int metric.
 *    
 *    @tparam Super the superclass
 **/
template<typename Super> class Metric {
public:
    Metric(decltype(Super::value) val = 0) { v() = val; }
    Metric(const Metric&) = default;
    bool    operator==(const Metric& rhs) const { return v() == rhs.v(); }
    bool    operator!=(const Metric& rhs) const { return v() != rhs.v(); }
    bool    operator> (int rhs)           const { return v() > rhs; }
    bool    operator< (int rhs)           const { return v() < rhs; }
    bool    operator<=(int rhs)           const { return v() <= rhs; }
    bool    operator>=(int rhs)           const { return v() >= rhs; }
    Metric  operator- (int rhs)           const { return { v() - rhs }; }
    Metric  operator+ (int rhs)           const { return { v() + rhs }; }
    Metric &operator-=(int rhs)                 { v() -= rhs; return *this; }
    Metric &operator+=(int rhs)                 { v() += rhs; return *this; }
    Metric &operator= (int rhs)                 { v()  = rhs; return *this; }
private:
    auto& v()               { return as_super().value; }
    auto  v()         const { return as_super().value; }
    Super &as_super()       { return *reinterpret_cast<Super*>(this); }
    const Super &as_super() const { return *reinterpret_cast<const Super*>(this); }
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
