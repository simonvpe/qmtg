#include "common.hpp"

#include "catch.hpp"

namespace m   = MTG;
namespace spd = spdlog;
using namespace entityx;

SCENARIO("Untap step","[turn]"){
    /*
      All phased-in-permanents phase out and all phased-out permanents
      phase in. This is a turn-based action and does not use the stack.

      Next the active player untaps all permanents he or she controls.
      This is a turn-based action and does not use the stack. (NOTE:
      spells and effects may prevent permanents from untapping normally)

      No player recieves priority during the untap step. No spells can
      be cast and no abilities can be activated. Abilities that trigger
      during this step are held until the upkeep step.
     */
}

SCENARIO("Upkeep step","[turn]"){
    /*
      All abilities that trigger at the beginning of the upkeep step
      and abilities that triggered during the untap step go on the stack,
      then the active player gets priority.
     */
}

SCENARIO("Draw step","[turn]"){
    /*
      The active player draws a card. This is a turn-based action and does
      not use the stack.

      Next, all abilities that trigger at the beginning of the draw step
      go on the stack, then the active player gets priority.
     */
}

SCENARIO("Main phase step","[turn]"){
    /*
      NOTE: This step appears twice: after the draw step and after the
      end of combat step.

      All abilities that trigger at the beginning of the main phase go
      on the stack, then the active player gets priority.

      The main phases are normally the only phases in which a player
      may cast artifact, creature, enchantment, planeswalker, and
      sorcery spells.

      The main phases are also the only phases in which a player may
      play a land. The active player may play one land from his or her
      hand only if he or she hasn't played a land from his or her hand
      this turn and only if the stack is empty. Playing a land is not
      considered a spell and therefore cannot be responded to with
      spells or abilities.
     */
}

SCENARIO("Beginning of combat step","[turn]"){
    /*
      All abilities that trigger at the beginning of combat.
     */
}

SCENARIO("Declare attackers step","[turn]"){
    /*
     The active player declares attackers. This is a turn-based action
     and does not use the stack.

     Next all abilities that trigger on attackers being declared go
     on the stack, then the active player gets priority.
    */
}

SCENARIO("Declare blockers step","[turn]"){
    /*
      The defending player declares blockers. This is a turn-based action
      and does not use the stack.

      Next all abilities that trigger on blockers being declared go on the
      stack, then the active player gets priority.
     */
}

SCENARIO("First strike / normal / additional combat damage steps","[turn]"){
    /*
      All combat damage is dealt simultaneously. This is a turn-based
      action and does not use the stack.

      All abilities that trigger on damage being dealt go on the stack,
      then the active player gets priority.

      If any attacking or blocking creatures have first or double strike
      there is an additional cobat damage step.
    */
}

SCENARIO("End of combat step","[turn]"){
    /*
      All abilities that trigger at the beginning of the end step go on
      the stack, then the active player gets priority.
     */
}

SCENARIO("End step","[turn]"){
    /*
      All abilities that trigger at the beginning of the end step go on
      the stack, then the active player gets priority.
     */
}

SCENARIO("Cleanup step","[turn]"){
    /*
      If the active player's hand contains more cards than his or her
      maximum hand size, he or she discards enough cards to reduce his
      or her hand size to that number. This is a turn-based action and
      does not use the stack.

      All damage marged on permanents is removed and all "until end of turn"
      and "this turn" effects end. These are turn-based actions that happen
      simultaneously and do not use the stack.

      No player receives priority during the cleanup step. No spells can be
      cast and no abilities can be activated.
    */
}
