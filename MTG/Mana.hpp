#pragma once

namespace MTG {
namespace Mana {
using CColor = Constants::Color;

#pragma pack(push,1)
// Base class for mana. Occupies one byte where the first three bits represents
// the mana color and the last five bits represents the amount
class Base { 
    static constexpr uint8_t m = 1U << 4U;

    public:
    constexpr Base() : c(0), a(0) {}
    constexpr Base(CColor color, int8_t amount) : c{(uint8_t)color}, a(amount & 0x1f) {}
    constexpr CColor color()  const { return (CColor)c; }
    constexpr int8_t amount() const { return a; }
    constexpr Base& operator=(const Base &) = default;
    private:
    unsigned c:3;
    int      a:5;
};
#pragma pack(pop)

// Shortcuts for creating mana of different color
struct Black : Base { 
    constexpr Black(int8_t a) : Base{CColor::BLACK, a} {} 
    constexpr Black()                         = default;
    constexpr Black& operator=(const Black &) = default;   
};
struct Blue : Base { 
    constexpr Blue(int8_t a) : Base{CColor::BLUE, a} {} 
    constexpr Blue()                        = default;
    constexpr Blue& operator=(const Blue &) = default;
};
struct Colorless : Base { 
    constexpr Colorless(int8_t a) : Base{CColor::COLORLESS, a} {} 
    constexpr Colorless()                             = default;
    constexpr Colorless& operator=(const Colorless &) = default;
};
struct Generic : Base { 
    constexpr Generic(int8_t a) : Base{CColor::GENERIC, a} {} 
    constexpr Generic()                           = default;
    constexpr Generic& operator=(const Generic &) = default;
};
struct Green : Base { 
    constexpr Green(int8_t a) : Base{CColor::GREEN, a} {} 
    constexpr Green()                         = default;
    constexpr Green& operator=(const Green &) = default;
};
struct Red : Base { 
    constexpr Red(int8_t a) : Base{CColor::RED, a} {} 
    constexpr Red()                       = default;
    constexpr Red& operator=(const Red &) = default;
};
struct White : Base { 
    constexpr White(int8_t a) : Base{CColor::WHITE, a} {} 
    constexpr White()                         = default;
    constexpr White& operator=(const White &) = default;
};

// Cost class, in alphabetical order
struct Collection {
    constexpr Collection() = default;
    constexpr Collection(const Collection &) = default;
    Black     black;
    Blue      blue;
    Colorless colorless;
    Generic   generic;
    Green     green;
    Red       red;
    White     white;
};

} // namespace Mana
} // namespace MTG
