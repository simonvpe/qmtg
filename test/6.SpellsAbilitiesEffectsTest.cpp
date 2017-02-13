#include "common.hpp"
#include "gamestate.hpp"
#include "factory.hpp"

#include "catch.hpp"

namespace       m   = MTG;
namespace       spd = spdlog;
using namespace entityx;

SCENARIO("601. Casting Spells","") {
    /*
      601.1. Previously, the action of casting a spell, or casting a
      card as a spell, was referred to on cards as "playing" that
      spell or that card. Cards that were printed with that text have
      received errata in the Oracle card reference so they now refer
      to "casting" that spell or that card.

      601.1a Some effects still refer to "playing" a card. "Playing a
      card" means playing that card as a land or casting that card as
      a spell, whichever is appropriate.

      601.2. To cast a spell is to take it from where it is (usually
      the hand), put it on the stack, and pay its costs, so that it
      will eventually resolve and have its effect. Casting a spell
      includes proposal of the spell (rules 601.2a–d) and
      determination and payment of costs (rules 601.2f–h). To cast
      a spell, a player follows the steps listed below, in order. A
      player must be legally allowed to cast the spell to begin this
      process (see rule 601.3), ignoring any effect that would
      prohibit that spell from being cast based on information
      determined during that spell's proposal. (Such effects are
      considered during the check detailed in rule 601.2e.) If, at any
      point during the casting of a spell, a player is unable to
      comply with any of the steps listed below, the casting of the
      spell is illegal; the game returns to the moment before the
      casting of that spell was proposed (see rule 720, "Handling
      Illegal Actions").

      601.2a To propose the casting of a spell, a player first moves
      that card (or that copy of a card) from where it is to the
      stack. It becomes the topmost object on the stack. It has all
      the characteristics of the card (or the copy of a card)
      associated with it, and that player becomes its controller. The
      spell remains on the stack until it's countered, it resolves, or
      an effect moves it elsewhere.

      601.2b If the spell is modal, the player announces the mode
      choice (see rule 700.2). If the player wishes to splice any
      cards onto the spell (see rule 702.46), he or she reveals those
      cards in his or her hand. If the spell has alternative or
      additional costs that will be paid as it's being cast such as
      buyback or kicker costs (see rules 117.8 and 117.9), the player
      announces his or her intentions to pay any or all of those costs
      (see rule 601.2f). A player can't apply two alternative methods
      of casting or two alternative costs to a single spell. If the
      spell has a variable cost that will be paid as it's being cast
      (such as an {X} in its mana cost; see rule 107.3), the player
      announces the value of that variable. If the value of that
      variable is defined in the text of the spell by a choice that
      player would make later in the announcement or resolution of the
      spell, that player makes that choice at this time instead of
      that later time. If a cost that will be paid as the spell is
      being cast includes hybrid mana symbols, the player announces
      the nonhybrid equivalent cost he or she intends to pay. If a
      cost that will be paid as the spell is being cast includes
      Phyrexian mana symbols, the player announces whether he or she
      intends to pay 2 life or the corresponding colored mana cost for
      each of those symbols. Previously made choices (such as choosing
      to cast a spell with flashback from a graveyard or choosing to
      cast a creature with morph face down) may restrict the player's
      options when making these choices.

      601.2c The player announces his or her choice of an appropriate
      player, object, or zone for each target the spell requires. A
      spell may require some targets only if an alternative or
      additional cost (such as a buyback or kicker cost), or a
      particular mode, was chosen for it; otherwise, the spell is cast
      as though it did not require those targets. If the spell has a
      variable number of targets, the player announces how many
      targets he or she will choose before he or she announces those
      targets. In some cases, the number of targets will be defined by
      the spell's text. Once the number of targets the spell has is
      determined, that number doesn't change, even if the information
      used to determine the number of targets does. The same target
      can't be chosen multiple times for any one instance of the word
      "target" on the spell. However, if the spell uses the word
      "target" in multiple places, the same object, player, or zone
      can be chosen once for each instance of the word "target" (as
      long as it fits the targeting criteria). If any effects say that
      an object or player must be chosen as a target, the player
      chooses targets so that he or she obeys the maximum possible
      number of such effects without violating any rules or effects
      that say that an object or player can't be chosen as a
      target. The chosen players, objects, and/or zones each become a
      target of that spell. (Any abilities that trigger when those
      players, objects, and/or zones become the target of a spell
      trigger at this point; they'll wait to be put on the stack until
      the spell has finished being cast.)  Example: If a spell says
      "Tap two target creatures," then the same creature can't be
      chosen twice; the spell requires two different legal targets. A
      spell that says "Destroy target artifact and target land,"
      however, can target the same artifact land twice because it uses
      the word "target" in multiple places.

      601.2d If the spell requires the player to divide or distribute
      an effect (such as damage or counters) among one or more
      targets, the player announces the division. Each of these
      targets must receive at least one of whatever is being divided.

      601.2e The game checks to see if the proposed spell can legally
      be cast. If the proposed spell is illegal, the game returns to
      the moment before the casting of that spell was proposed (see
      rule 720, "Handling Illegal Actions").

      601.2f The player determines the total cost of the
      spell. Usually this is just the mana cost. Some spells have
      additional or alternative costs. Some effects may increase or
      reduce the cost to pay, or may provide other alternative
      costs. Costs may include paying mana, tapping permanents,
      sacrificing permanents, discarding cards, and so on. The total
      cost is the mana cost or alternative cost (as determined in rule
      601.2b), plus all additional costs and cost increases, and minus
      all cost reductions. If multiple cost reductions apply, the
      player may apply them in any order. If the mana component of the
      total cost is reduced to nothing by cost reduction effects, it
      is considered to be {0}. It can't be reduced to less than
      {0}. Once the total cost is determined, any effects that
      directly affect the total cost are applied. Then the resulting
      total cost becomes "locked in." If effects would change the
      total cost after this time, they have no effect.

      601.2g If the total cost includes a mana payment, the player
      then has a chance to activate mana abilities (see rule 605,
      "Mana Abilities"). Mana abilities must be activated before costs
      are paid.

      601.2h The player pays the total cost in any order. Partial
      payments are not allowed. Unpayable costs can't be paid.
      Example: You cast Altar's Reap, which costs {1}{B} and has an
      additional cost of sacrificing a creature. You sacrifice
      Thunderscape Familiar, whose effect makes your black spells cost
      {1} less to cast. Because a spell's total cost is "locked in"
      before payments are actually made, you pay {B}, not {1}{B}, even
      though you're sacrificing the Familiar.

      601.2i Once the steps described in 601.2a–h are completed,
      effects that modify the characteristics of the spell as it's
      cast are applied, then the spell becomes cast. Any abilities
      that trigger when a spell is cast or put onto the stack trigger
      at this time. If the spell's controller had priority before
      casting it, he or she gets priority.

      601.3. A player can't begin to cast a spell unless a rule or
      effect allows that player to cast it. If that player is no
      longer allowed to cast that spell after completing its proposal,
      the casting of the spell is illegal and the game returns to the
      moment before the casting of that spell was proposed (see rule
      720, "Handling Illegal Actions").

      601.3a If an effect allows a player to cast a spell as though it
      had flash only if an alternative or additional cost is paid,
      that player may begin to cast that spell as though it had flash.

      601.3b If an effect allows a player to cast a spell with certain
      characteristics as though it had flash, that player may consider
      any choices to be made during that spell's proposal that may
      cause it to have those characteristics. If any such choices
      could cause it to match these characteristics, that player may
      begin to cast that spell.  Example: An effect says that you may
      cast Aura spells as though they had flash, and you have a
      creature card with bestow in your hand. Because choosing the
      bestow ability's alternative cost causes that spell to become an
      Aura spell, you may legally begin to cast that spell as though
      it had flash.

      601.4. Some spells specify that one of their controller's
      opponents does something the controller would normally do while
      it's being cast, such as choose a mode or choose targets. In
      these cases, the opponent does so when the spell's controller
      normally would do so.

      601.4a If there is more than one opponent who could make such a
      choice, the spell's controller decides which of those opponents
      will make the choice.

      601.4b If the spell instructs its controller and another player
      to do something at the same time as the spell is being cast, the
      spell's controller goes first, then the other player. This is an
      exception to rule 101.4.

      601.5. Casting a spell that alters costs won't affect spells and
      abilities that are already on the stack.
*/
}
