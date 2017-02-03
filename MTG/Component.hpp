#pragma once

#include <functional>
#include "Constants.hpp"
#include "Property.hpp"
#include "Energy.hpp"
#include "Mana.hpp"
#include "Enchant.hpp"

namespace MTG {
namespace Component {
    
struct CardComponent {
    Constants::CardType    type;
    Constants::CardSubtype subType;
};

struct EnchantCreatureComponent {
    int powerBonus;
    int toughnessBonus;
};

struct CreatureComponent {
    int power;
    int toughness;
    Constants::Race race;
};

struct EvasionAbilityComponent {
    Constants::EvasionAbility evasion;
};

struct CreaturesYouControlHaveEvasion {
    Constants::EvasionAbility evasion;
};
    
} // namespace Component
} // namespace MTG
