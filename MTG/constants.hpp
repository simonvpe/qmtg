#pragma once

namespace MTG {    
enum class Zone {
    UNDEFINED    = 0,
    HAND         = (1 << 0),
    BATTLEFIELD  = (1 << 1),
    LIBRARY      = (1 << 2),
    GRAVEYARD    = (1 << 3),
    COMMAND      = (1 << 4),
    STACK        = (1 << 5)
};
    
enum class CreatureType {
    UNDEFINED = 0,
    HUMAN     = (1 << 0),
    KNIGHT    = (1 << 1)
};

enum class CreatureClass {
    UNDEFINED = 0,
    SOLDIER   = (1 << 0),
    TOKEN     = (1 << 1),
    ALLY      = (1 << 2)
};
        
enum class ManualTriggerProperties {
    UNDEFINED   = 0,
    CARD_SLOT_0 = (1 << 0),
    CARD_SLOT_1 = (1 << 1),
    CARD_SLOT_2 = (1 << 2)
};

enum class TriggerTiming {
    UNDEFINED          = 0,
    ENTERS_BATTLEFIELD = (1 << 0)
};
    
enum class Timing {
    UNDEFINED = 0,
    INSTANT   = (1 << 0),
    SORCERY   = (1 << 1)
};

enum class TriggerLifetime {
    UNDEFINED   = 0,
    END_OF_TURN = (1 << 0),
    ONCE        = (1 << 1),
    PARENT      = (1 << 2)
};

enum class Phase {
    UNDEFINED       = 0,
    BEGINNING       = (1 << 0),
    PRECOMBAT_MAIN  = (1 << 1),
    COMBAT          = (1 << 2),
    POSTCOMBAT_MAIN = (1 << 3),
    ENDING          = (1 << 4)
};

enum class Step {
    UNDEFINED           = 0,
    UNTAP               = (1 << 0),
    UPKEEP              = (1 << 1),
    DRAW                = (1 << 2),
    BEGINNING_OF_COMBAT = (1 << 3),                      
    DECLARE_ATTACKERS   = (1 << 4),
    DECLARE_BLOCKERS    = (1 << 5),
    COMBAT_DAMAGE       = (1 << 6),
    END_OF_COMBAT       = (1 << 7),
    ENDING              = (1 << 8),                                
    END                 = (1 << 9)
};         
} // namespace MTG
