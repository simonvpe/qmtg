#pragma once

namespace MTG {
namespace Constants {
    
static constexpr auto MaxAbilities = 5U;

enum class Color : uint8_t { 
    GENERIC, COLORLESS, BLACK, BLUE, GREEN, RED, WHITE 
};

enum class Zone : uint8_t { 
    HAND, GRAVEYARD, LIBRARY, BATTLEFIELD, STACK, ALL, OUTSIDE 
};

enum class ZoneDetail : uint8_t {
    NONE, TOP, BOTTOM, CHOOSE 
};

enum class TurnPhase : uint8_t {
    BEGINNNG, PRECOMBAT_MAIN, COMBAT, POSTCOMBAT_MAIN, END 
};

enum class PhaseStep : uint8_t {
    UNTAP, UPKEEP, DRAW, PRECOMBAT_MAIN, BEGIN_COMBAT, DECLARE_ATTACKERS,
    DECLARE_BLOCKERS, FIRST_COMBAT_DAMAGE, COMBAT_DAMAGE, END_COMBAT,
    POSTCOMBAT_MAIN, END_TURN, CLEANUP
};

enum class TimingRule : uint8_t{
    INSTANT, SORCERY 
};

enum class TableState : uint8_t { 
    WAITING, READY_TO_START, STARTING, DRAFTING, CONSTRUCTING, 
    DUELING, SIDEBOARDING, FINISHED 
};

enum class UserNotify {
    NOT_ENOUGH_MANA
};

enum class CardType {
    ENCHANTMENT, CREATURE
};

enum class CardSubtype {
    AURA, PILOT
};

enum class Race {
    HUMAN
};

enum class EvasionAbility {
    FLYING       = 0x00000001, 
    FEAR         = FLYING       << 1,
    INTIMIDATE   = FEAR         << 1,
    HORSEMANSHIP = INTIMIDATE   << 1,
    SKULK        = HORSEMANSHIP << 1,
    SHADOW       = SKULK        << 1,
    MENACE       = SHADOW       << 1,
    STALK        = MENACE       << 1,
    LANDWALK     = STALK        << 1
};

}
}
