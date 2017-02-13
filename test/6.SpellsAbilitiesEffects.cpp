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
      includes proposal of the spell (rules 601.2a�d) and
      determination and payment of costs (rules 601.2f�h). To cast
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

      601.2i Once the steps described in 601.2a�h are completed,
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

SCENARIO("602. Activating Activated Abilities","") {
    /*
      602.1. Activated abilities have a cost and an effect. They are
      written as "[Cost]: [Effect.] [Activation instructions (if
      any).]"

      602.1a The activation cost is everything before the colon
      (:). An ability's activation cost must be paid by the player who
      is activating it.  Example: The activation cost of an ability
      that reads "{2}, {T}: You gain 1 life" is two mana of any type
      plus tapping the permanent that has the ability.

      602.1b Some text after the colon of an activated ability states
      instructions that must be followed while activating that
      ability. Such text may state which players can activate that
      ability, may restrict when a player can activate the ability, or
      may define some aspect of the activation cost. This text is not
      part of the ability's effect. It functions at all times. If an
      activated ability has any activation instructions, they appear
      last, after the ability's effect.

      602.1c An activated ability is the only kind of ability that can
      be activated. If an object or rule refers to activating an
      ability without specifying what kind, it must be referring to an
      activated ability.

      602.1d Previously, the action of using an activated ability was
      referred to on cards as "playing" that ability. Cards that were
      printed with that text have received errata in the Oracle card
      reference so they now refer to "activating" that ability.

      602.2. To activate an ability is to put it onto the stack and
      pay its costs, so that it will eventually resolve and have its
      effect. Only an object's controller (or its owner, if it doesn't
      have a controller) can activate its activated ability unless the
      object specifically says otherwise. Activating an ability
      follows the steps listed below, in order. If, at any point
      during the activation of an ability, a player is unable to
      comply with any of those steps, the activation is illegal; the
      game returns to the moment before that ability started to be
      activated (see rule 720, "Handling Illegal
      Actions"). Announcements and payments can't be altered after
      they've been made.

      602.2a The player announces that he or she is activating the
      ability. If an activated ability is being activated from a
      hidden zone, the card that has that ability is revealed. That
      ability is created on the stack as an object that's not a
      card. It becomes the topmost object on the stack. It has the
      text of the ability that created it, and no other
      characteristics. Its controller is the player who activated the
      ability. The ability remains on the stack until it's countered,
      it resolves, or an effect moves it elsewhere.

      602.2b The remainder of the process for activating an ability is
      identical to the process for casting a spell listed in rules
      601.2b�i. Those rules apply to activating an ability just as
      they apply to casting a spell. An activated ability's analog to
      a spell's mana cost (as referenced in rule 601.2f) is its
      activation cost.

      602.3. Some abilities specify that one of their controller's
      opponents does something the controller would normally do while
      it's being activated, such as choose a mode or choose
      targets. In these cases, the opponent does so when the ability's
      controller normally would do so.

      602.3a If there is more than one opponent who could make such a
      choice, the ability's controller decides which of those
      opponents will make the choice.

      602.3b If the ability instructs its controller and another
      player to do something at the same time as the ability is being
      activated, the ability's controller goes first, then the other
      player. This is an exception to rule 101.4.

      602.4. Activating an ability that alters costs won't affect
      spells and abilities that are already on the stack.

      602.5. A player can't begin to activate an ability that's
      prohibited from being activated.

      602.5a A creature's activated ability with the tap symbol ({T})
      or the untap symbol ({Q}) in its activation cost can't be
      activated unless the creature has been under its controller's
      control since the start of his or her most recent turn. Ignore
      this rule for creatures with haste (see rule 702.10).

      602.5b If an activated ability has a restriction on its use (for
      example, "Activate this ability only once each turn"), the
      restriction continues to apply to that object even if its
      controller changes.

      602.5c If an object acquires an activated ability with a
      restriction on its use from another object, that restriction
      applies only to that ability as acquired from that object. It
      doesn't apply to other, identically worded abilities.

      602.5d Activated abilities that read "Activate this ability only
      any time you could cast a sorcery" mean the player must follow
      the timing rules for casting a sorcery spell, though the ability
      isn't actually a sorcery. The player doesn't actually need to
      have a sorcery card that he or she could cast.

      602.5e Activated abilities that read "Activate this ability only
      any time you could cast an instant" mean the player must follow
      the timing rules for casting an instant spell, though the
      ability isn't actually an instant. The player doesn't actually
      need to have an instant card that he or she could cast.
*/
}

SCENARIO("603. Handling Triggered Abilities","") {
    /*
      603.1. Triggered abilities have a trigger condition and an
      effect. They are written as "[When/Whenever/At] [trigger
      condition or event], [effect]."

      603.2. Whenever a game event or game state matches a triggered
      ability's trigger event, that ability automatically
      triggers. The ability doesn't do anything at this point.

      603.2a Because they aren't cast or activated, triggered
      abilities can trigger even when it isn't legal to cast spells
      and activate abilities. Effects that prevent abilities from
      being activated don't affect them.

      603.2b When a phase or step begins, all abilities that trigger
      "at the beginning of" that phase or step trigger.

      603.2c An ability triggers only once each time its trigger event
      occurs. However, it can trigger repeatedly if one event contains
      multiple occurrences.  Example: A permanent has an ability whose
      trigger condition reads, "Whenever a land is put into a
      graveyard from the battlefield, . . . ." If someone casts a
      spell that destroys all lands, the ability will trigger once for
      each land put into the graveyard during the spell's resolution.

      603.2d Some trigger events use the word "becomes" (for example,
      "becomes attached" or "becomes blocked"). These trigger only at
      the time the named event happens�they don't trigger if that
      state already exists or retrigger if it persists. An ability
      that triggers when a permanent "becomes tapped" or "becomes
      untapped" doesn't trigger if the permanent enters the
      battlefield in that state.  Example: An ability that triggers
      when a permanent "becomes tapped" triggers only when the status
      of a permanent that's already on the battlefield changes from
      untapped to tapped.

      603.2e If a triggered ability's trigger condition is met, but
      the object with that triggered ability is at no time visible to
      all players, the ability does not trigger.

      603.2f An ability triggers only if its trigger event actually
      occurs. An event that's prevented or replaced won't trigger
      anything.  Example: An ability that triggers on damage being
      dealt won't trigger if all the damage is prevented.

      603.3. Once an ability has triggered, its controller puts it on
      the stack as an object that's not a card the next time a player
      would receive priority. See rule 116, "Timing and Priority." The
      ability becomes the topmost object on the stack. It has the text
      of the ability that created it, and no other characteristics. It
      remains on the stack until it's countered, it resolves, a rule
      causes it to be removed from the stack, or an effect moves it
      elsewhere.

      603.3a A triggered ability is controlled by the player who
      controlled its source at the time it triggered, unless it's a
      delayed triggered ability. To determine the controller of a
      delayed triggered ability, see rules 603.7d�f.

      603.3b If multiple abilities have triggered since the last time
      a player received priority, each player, in APNAP order, puts
      triggered abilities he or she controls on the stack in any order
      he or she chooses. (See rule 101.4.) Then the game once again
      checks for and resolves state-based actions until none are
      performed, then abilities that triggered during this process go
      on the stack. This process repeats until no new state-based
      actions are performed and no abilities trigger. Then the
      appropriate player gets priority.

      603.3c If a triggered ability is modal, its controller announces
      the mode choice when he or she puts the ability on the stack. If
      one of the modes would be illegal (due to an inability to choose
      legal targets, for example), that mode can't be chosen. If no
      mode can be chosen, the ability is removed from the stack. (See
      rule 700.2.)

      603.3d The remainder of the process for putting a triggered
      ability on the stack is identical to the process for casting a
      spell listed in rules 601.2c�d. If a choice is required when
      the triggered ability goes on the stack but no legal choices can
      be made for it, or if a rule or a continuous effect otherwise
      makes the ability illegal, the ability is simply removed from
      the stack.

      603.4. A triggered ability may read "When/Whenever/At [trigger
      event], if [condition], [effect]." When the trigger event
      occurs, the ability checks whether the stated condition is
      true. The ability triggers only if it is; otherwise it does
      nothing. If the ability triggers, it checks the stated condition
      again as it resolves. If the condition isn't true at that time,
      the ability is removed from the stack and does nothing. Note
      that this mirrors the check for legal targets. This rule is
      referred to as the "intervening �if' clause" rule. (The word
      "if" has only its normal English meaning anywhere else in the
      text of a card; this rule only applies to an "if" that
      immediately follows a trigger condition.)  Example: Felidar
      Sovereign reads, "At the beginning of your upkeep, if you have
      40 or more life, you win the game." Its controller's life total
      is checked as that player's upkeep begins. If that player has 39
      or less life, the ability doesn't trigger at all. If that player
      has 40 or more life, the ability triggers and goes on the
      stack. As the ability resolves, that player's life total is
      checked again. If that player has 39 or less life at this time,
      the ability is removed from the stack and has no effect. If that
      player has 40 or more life at this time, the ability resolves
      and that player wins the game.

      603.5. Some triggered abilities' effects are optional (they
      contain "may," as in "At the beginning of your upkeep, you may
      draw a card"). These abilities go on the stack when they
      trigger, regardless of whether their controller intends to
      exercise the ability's option or not. The choice is made when
      the ability resolves. Likewise, triggered abilities that have an
      effect "unless" something is true or a player chooses to do
      something will go on the stack normally; the "unless" part of
      the ability is dealt with when the ability resolves.

      603.6. Trigger events that involve objects changing zones are
      called "zone-change triggers." Many abilities with zone-change
      triggers attempt to do something to that object after it changes
      zones. During resolution, these abilities look for the object in
      the zone that it moved to. If the object is unable to be found
      in the zone it went to, the part of the ability attempting to do
      something to the object will fail to do anything. The ability
      could be unable to find the object because the object never
      entered the specified zone, because it left the zone before the
      ability resolved, or because it is in a zone that is hidden from
      a player, such as a library or an opponent's hand. (This rule
      applies even if the object leaves the zone and returns again
      before the ability resolves.) The most common zone-change
      triggers are enters-the-battlefield triggers and
      leaves-the-battlefield triggers.

      603.6a Enters-the-battlefield abilities trigger when a permanent
      enters the battlefield. These are written, "When [this object]
      enters the battlefield, . . . " or "Whenever a [type] enters the
      battlefield, . . ." Each time an event puts one or more
      permanents onto the battlefield, all permanents on the
      battlefield (including the newcomers) are checked for any
      enters-the-battlefield triggers that match the event.

      603.6b Continuous effects that modify characteristics of a
      permanent do so the moment the permanent is on the battlefield
      (and not before then). The permanent is never on the battlefield
      with its unmodified characteristics. Continuous effects don't
      apply before the permanent is on the battlefield, however (see
      rule 603.6d).  Example: If an effect reads "All lands are
      creatures" and a land card is played, the effect makes the land
      card into a creature the moment it enters the battlefield, so it
      would trigger abilities that trigger when a creature enters the
      battlefield. Conversely, if an effect reads "All creatures lose
      all abilities" and a creature card with an
      enters-the-battlefield triggered ability enters the battlefield,
      that effect will cause it to lose its abilities the moment it
      enters the battlefield, so the enters-the-battlefield ability
      won't trigger.

      603.6c Leaves-the-battlefield abilities trigger when a permanent
      moves from the battlefield to another zone, or when a phased-in
      permanent leaves the game because its owner leaves the
      game. These are written as, but aren't limited to, "When [this
      object] leaves the battlefield, . . ." or "Whenever [something]
      is put into a graveyard from the battlefield, . . . ." (See also
      rule 603.10.) An ability that attempts to do something to the
      card that left the battlefield checks for it only in the first
      zone that it went to. An ability that triggers when a card is
      put into a certain zone "from anywhere" is never treated as a
      leaves-the-battlefield ability, even if an object is put into
      that zone from the battlefield.

      603.6d Some permanents have text that reads "[This permanent]
      enters the battlefield with . . . ," "As [this permanent] enters
      the battlefield . . . ," "[This permanent] enters the
      battlefield as . . . ," or "[This permanent] enters the
      battlefield tapped." Such text is a static ability�not a
      triggered ability�whose effect occurs as part of the event
      that puts the permanent onto the battlefield.

      603.6e Some Auras have triggered abilities that trigger on the
      enchanted permanent leaving the battlefield. These triggered
      abilities can find the new object that permanent card became in
      the zone it moved to; they can also find the new object the Aura
      card became in its owner's graveyard after state-based actions
      have been checked. See rule 400.7.

      603.7. An effect may create a delayed triggered ability that can
      do something at a later time. A delayed triggered ability will
      contain "when," "whenever," or "at," although that word won't
      usually begin the ability.

      603.7a Delayed triggered abilities are created during the
      resolution of spells or abilities, as the result of a
      replacement effect being applied, or as a result of a static
      ability that allows a player to take an action. A delayed
      triggered ability won't trigger until it has actually been
      created, even if its trigger event occurred just
      beforehand. Other events that happen earlier may make the
      trigger event impossible.  Example: Part of an effect reads
      "When this creature leaves the battlefield," but the creature in
      question leaves the battlefield before the spell or ability
      creating the effect resolves. In this case, the delayed ability
      never triggers.  Example: If an effect reads "When this creature
      becomes untapped" and the named creature becomes untapped before
      the effect resolves, the ability waits for the next time that
      creature untaps.

      603.7b A delayed triggered ability will trigger only once�the
      next time its trigger event occurs�unless it has a stated
      duration, such as "this turn."

      603.7c A delayed triggered ability that refers to a particular
      object still affects it even if the object changes
      characteristics. However, if that object is no longer in the
      zone it's expected to be in at the time the delayed triggered
      ability resolves, the ability won't affect it. (Note that if
      that object left that zone and then returned, it's a new object
      and thus won't be affected. See rule 400.7.)  Example: An
      ability that reads "Exile this creature at the beginning of the
      next end step" will exile the permanent even if it's no longer a
      creature during the next end step. However, it won't do anything
      if the permanent left the battlefield before then.

      603.7d If a spell creates a delayed triggered ability, the
      source of that delayed triggered ability is that spell. The
      controller of that delayed triggered ability is the player who
      controlled that spell as it resolved.

      603.7e If an activated or triggered ability creates a delayed
      triggered ability, the source of that delayed triggered ability
      is the same as the source of that other ability. The controller
      of that delayed triggered ability is the player who controlled
      that other ability as it resolved.

      603.7f If a static ability generates a replacement effect which
      causes a delayed triggered ability to be created, the source of
      that delayed triggered ability is the object with that static
      ability. The controller of that delayed triggered ability is the
      same as the controller of that object at the time the
      replacement effect was applied.

      603.7g If a static ability allows a player to take an action and
      creates a delayed triggered ability if that player does so, the
      source of that delayed triggered ability is the object with that
      static ability. The controller of that delayed triggered ability
      is the same as the controller of that object at the time the
      action was taken.

      603.8. Some triggered abilities trigger when a game state (such
      as a player controlling no permanents of a particular card type)
      is true, rather than triggering when an event occurs. These
      abilities trigger as soon as the game state matches the
      condition. They'll go onto the stack at the next available
      opportunity. These are called state triggers. (Note that state
      triggers aren't the same as state-based actions.) A
      state-triggered ability doesn't trigger again until the ability
      has resolved, has been countered, or has otherwise left the
      stack. Then, if the object with the ability is still in the same
      zone and the game state still matches its trigger condition, the
      ability will trigger again.  Example: A permanent's ability
      reads, "Whenever you have no cards in hand, draw a card." If its
      controller plays the last card from his or her hand, the ability
      will trigger once and won't trigger again until it has
      resolved. If its controller casts a spell that reads "Discard
      your hand, then draw that many cards," the ability will trigger
      during the spell's resolution because the player's hand was
      momentarily empty.

      603.9. Some triggered abilities trigger specifically when a
      player loses the game. These abilities trigger when a player
      loses or leaves the game, regardless of the reason, unless that
      player leaves the game as the result of a draw. See rule 104.3.

      603.10. Normally, objects that exist immediately after an event
      are checked to see if the event matched any trigger conditions,
      and continuous effects that exist at that time are used to
      determine what the trigger conditions are and what the objects
      involved in the event look like. However, some triggered
      abilities are exceptions to this rule; the game "looks back in
      time" to determine if those abilities trigger, using the
      existence of those abilities and the appearance of objects
      immediately prior to the event. The list of exceptions is as
      follows:

      603.10a Some zone-change triggers look back in time. These are
      leaves-the-battlefield abilities, abilities that trigger when a
      card leaves a graveyard, and abilities that trigger when an
      object that all players can see is put into a hand or library.
      Example: Two creatures are on the battlefield along with an
      artifact that has the ability "Whenever a creature dies, you
      gain 1 life." Someone plays a spell that destroys all artifacts,
      creatures, and enchantments. The artifact's ability triggers
      twice, even though the artifact goes to its owner's graveyard at
      the same time as the creatures.

      603.10b Abilities that trigger when a permanent phases out look
      back in time.

      603.10c Abilities that trigger specifically when an object
      becomes unattached look back in time.

      603.10d Abilities that trigger when a player loses control of an
      object look back in time.

      603.10e Abilities that trigger when a spell is countered look
      back in time.

      603.10f Abilities that trigger when a player loses the game look
      back in time.

      603.10g Abilities that trigger when a player planeswalks away
      from a plane look back in time.

      603.11. Some objects have a static ability that's linked to one
      or more triggered abilities. (See rule 607, "Linked Abilities.")
      These objects combine the abilities into one paragraph, with the
      static ability first, followed by each triggered ability that's
      linked to it. A very few objects have triggered abilities which
      are written with the trigger condition in the middle of the
      ability, rather than at the beginning.  Example: An ability that
      reads "Reveal the first card you draw each turn. Whenever you
      reveal a basic land card this way, draw a card" is a static
      ability linked to a triggered ability.
    */
}

SCENARIO("604. Handling Static Abilities","") {
    /*
      604.1. Static abilities do something all the time rather than
      being activated or triggered. They are written as statements,
      and they're simply true.

      604.2. Static abilities create continuous effects, some of which
      are prevention effects or replacement effects. These effects are
      active as long as the permanent with the ability remains on the
      battlefield and has the ability, or as long as the object with
      the ability remains in the appropriate zone, as described in
      rule 112.6.

      604.3. Some static abilities are characteristic-defining
      abilities. A characteristic-defining ability conveys information
      about an object's characteristics that would normally be found
      elsewhere on that object (such as in its mana cost, type line,
      or power/toughness box) or overrides information found elsewhere
      on that object. Characteristic-defining abilities function in
      all zones. They also function outside the game.

      604.3a A static ability is a characteristic-defining ability if
      it meets the following criteria: (1) It defines an object's
      colors, subtypes, power, or toughness; (2) it is printed on the
      card it affects, it was granted to the token it affects by the
      effect that created the token, or it was acquired by the object
      it affects as the result of a copy effect or text-changing
      effect; (3) it does not directly affect the characteristics of
      any other objects; (4) it is not an ability that an object
      grants to itself; and (5) it does not set the values of such
      characteristics only if certain conditions are met.

      604.4. Many Auras, Equipment, and Fortifications have static
      abilities that modify the object they're attached to, but those
      abilities don't target that object. If an Aura, Equipment, or
      Fortification is moved to a different object, the ability stops
      applying to the original object and starts modifying the new
      one.

      604.5. Some static abilities apply while a spell is on the
      stack. These are often abilities that refer to countering the
      spell. Also, abilities that say "As an additional cost to cast
      . . . ," "You may pay [cost] rather than pay [this object]'s
      mana cost," and "You may cast [this object] without paying its
      mana cost" work while a spell is on the stack.

      604.6. Some static abilities apply while a card is in any zone
      that you could cast or play it from (usually your hand). These
      are limited to those that read, "You may [cast/play] [this card]
      . . . ," "You can't [cast/play] [this card] . . . ," and
      "[Cast/Play] [this card] only . . . ."

      604.7. Unlike spells and other kinds of abilities, static
      abilities can't use an object's last known information for
      purposes of determining how their effects are applied.
*/
}

SCENARIO("605. Mana Abilities","") {
    /*
      605.1. Some activated abilities and some triggered abilities are
      mana abilities, which are subject to special rules. Only
      abilities that meet either of the following two sets of criteria
      are mana abilities, regardless of what other effects they may
      generate or what timing restrictions (such as "Activate this
      ability only any time you could cast an instant") they may have.

      605.1a An activated ability is a mana ability if it meets all of
      the following criteria: it doesn't have a target, it could put
      mana into a player's mana pool when it resolves, and it's not a
      loyalty ability. (See rule 606, "Loyalty Abilities.")

      605.1b A triggered ability is a mana ability if it meets all of
      the following criteria: it doesn't have a target, it triggers
      from the resolution of an activated mana ability (see rule
      106.11a), and it could put mana into a player's mana pool when
      it resolves.

      605.2. A mana ability remains a mana ability even if the game
      state doesn't allow it to produce mana.  Example: A permanent
      has an ability that reads "{T}: Add {G} to your mana pool for
      each creature you control." This is still a mana ability even if
      you control no creatures or if the permanent is already tapped.

      605.3. Activating an activated mana ability follows the rules
      for activating any other activated ability (see rule 602.2),
      with the following exceptions:

      605.3a A player may activate an activated mana ability whenever
      he or she has priority, whenever he or she is casting a spell or
      activating an ability that requires a mana payment, or whenever
      a rule or effect asks for a mana payment, even if it's in the
      middle of casting or resolving a spell or activating or
      resolving an ability.

      605.3b An activated mana ability doesn't go on the stack, so it
      can't be targeted, countered, or otherwise responded to. Rather,
      it resolves immediately after it is activated. (See rule
      405.6c.)

      605.3c Once a player begins to activate a mana ability, that
      ability can't be activated again until it has resolved.

      605.4. Triggered mana abilities follow all the rules for other
      triggered abilities (see rule 603, "Handling Triggered
      Abilities"), with the following exception:

      605.4a A triggered mana ability doesn't go on the stack, so it
      can't be targeted, countered, or otherwise responded to. Rather,
      it resolves immediately after the mana ability that triggered
      it, without waiting for priority.  Example: An enchantment
      reads, "Whenever a player taps a land for mana, that player adds
      one mana to his or her mana pool of any type that land
      produced." If a player taps lands for mana while casting a
      spell, the additional mana is added to the player's mana pool
      immediately and can be used to pay for the spell.

      605.5. Abilities that don't meet the criteria specified in rules
      605.1a�b and spells aren't mana abilities.

      605.5a An ability with a target is not a mana ability, even if
      it could put mana into a player's mana pool when it
      resolves. The same is true for a triggered ability that could
      produce mana but triggers from an event other than activating a
      mana ability, or a triggered ability that triggers from
      activating a mana ability but couldn't produce mana. These
      follow the normal rules for activated or triggered abilities, as
      appropriate.

      605.5b A spell can never be a mana ability, even if it could put
      mana into a player's mana pool when it resolves. It's cast and
      resolves just like any other spell. Some older cards were
      printed with the card type "mana source"; these cards have
      received errata in the Oracle card reference and are now
      instants.
    */
}

SCENARIO("606. Loyalty Abilities","") {
    /*
      606.1. Some activated abilities are loyalty abilities, which are
      subject to special rules.

      606.2. An activated ability with a loyalty symbol in its cost is
      a loyalty ability. Normally, only planeswalkers have loyalty
      abilities.

      606.3. A player may activate a loyalty ability of a permanent he
      or she controls any time he or she has priority and the stack is
      empty during a main phase of his or her turn, but only if no
      player has previously activated a loyalty ability of that
      permanent that turn.

      606.4. The cost to activate a loyalty ability of a permanent is
      to put on or remove from that permanent a certain number of
      loyalty counters, as shown by the loyalty symbol in the
      ability's cost.

      606.5. A loyalty ability with a negative loyalty cost can't be
      activated unless the permanent has at least that many loyalty
      counters on it.
*/
}

SCENARIO("607. Linked Abilities","") {
    /*
      607.1. An object may have two abilities printed on it such that
      one of them causes actions to be taken or objects or players to
      be affected and the other one directly refers to those actions,
      objects, or players. If so, these two abilities are linked: the
      second refers only to actions that were taken or objects or
      players that were affected by the first, and not by any other
      ability.

      607.1a An ability printed on an object within another ability
      that grants that ability to that object is still considered to
      be "printed on" that object for these purposes.

      607.1b An ability printed on an object that fulfills both
      criteria described in rule 607.1 is linked to itself.

      607.2. There are different kinds of linked abilities.

      607.2a If an object has an activated or triggered ability
      printed on it that instructs a player to exile one or more cards
      and an ability printed on it that refers either to "the exiled
      cards" or to cards "exiled with [this object]," these abilities
      are linked. The second ability refers only to cards in the exile
      zone that were put there as a result of an instruction to exile
      them in the first ability.

      607.2b If an object has an ability printed on it that generates
      a replacement effect which causes one or more cards to be exiled
      and an ability printed on it that refers either to "the exiled
      cards" or to cards "exiled with [this object]," these abilities
      are linked. The second ability refers only to cards in the exile
      zone that were put there as a direct result of a replacement
      event caused by the first ability. See rule 614, "Replacement
      Effects."

      607.2c If an object has an activated or triggered ability
      printed on it that puts one or more objects onto the battlefield
      and an ability printed on it that refers to objects "put onto
      the battlefield with [this object]" or "created with [this
      object]," those abilities are linked. The second can refer only
      to objects put onto the battlefield as a result of the first.

      607.2d If an object has an ability printed on it that causes a
      player to "choose a [value]" or "name a card" and an ability
      printed on it that refers to "the chosen [value]," "the last
      chosen [value]," or "the named card," those abilities are
      linked. The second ability refers only to a choice made as a
      result of the first ability.

      607.2e If an object has an ability printed on it that causes a
      player to choose from between two or more words that otherwise
      have no rules meaning and an ability printed on it that refers
      to a choice involving one or more of those words, those
      abilities are linked. The second can refer only to a choice made
      as a result of the first ability.

      607.2f If an object has an ability printed on it that causes a
      player to pay a cost as it enters the battlefield and an ability
      printed on it that refers to the cost paid "as [this object]
      entered the battlefield," these abilities are linked. The second
      ability refers only to a cost paid as a result of the first
      ability.

      607.2g If an object has both a static ability and one or more
      triggered abilities printed on it in the same paragraph, each of
      those triggered abilities is linked to the static ability. Each
      triggered ability refers only to actions taken as a result of
      the static ability. See rule 603.11.

      607.2h If an object has a kicker ability printed on it and an
      ability printed on it that refers to whether that object was
      kicked, those abilities are linked. The second refers only to
      whether the intent to pay the kicker cost listed in the first
      was declared as the object was cast as a spell. If a kicker
      ability lists multiple costs, it will have multiple abilities
      linked to it. Each of those abilities will specify which kicker
      cost it refers to. See rule 702.32, "Kicker."

      607.2i If an object has an ability printed on it that causes a
      player to pay a variable additional cost as it's cast and an
      ability printed on it that refers to the cost paid "as [this
      object] was cast," these abilities are linked. The second refers
      only to the value chosen for the cost listed in the first as the
      object was cast as a spell. See rule 601.2b.

      607.2j The two abilities represented by the champion keyword are
      linked abilities. See rule 702.71, "Champion."

      607.2k Abilities preceded by an anchor word are linked to the
      ability that allows a player to choose that anchor word. See
      rule 614.12b.

      607.2m If an object has a static ability printed on it that
      allows a player to exile one or more cards "before you shuffle
      your deck to start the game" and an ability printed on it that
      refers to cards "exiled with cards named [this object's name],"
      the second ability is linked to the first ability of any objects
      that had the specified name before the game began.

      607.3. If, within a pair of linked abilities, one ability refers
      to a single object as "the exiled card," "a card exiled with
      [this card]," or a similar phrase, and the other ability has
      exiled multiple cards (usually because it was copied), the
      ability refers to each of the exiled cards. If that ability asks
      for any information about the exiled card, such as a
      characteristic or converted mana cost, it gets multiple
      answers. If these answers are used to determine the value of a
      variable, the sum of the answers is used. If that ability
      performs any actions on the exiled card, it performs that action
      on each exiled card.

      607.4. An ability may be part of more than one pair of linked
      abilities.  Example: Paradise Plume has the following three
      abilities: "As Paradise Plume enters the battlefield, choose a
      color," "Whenever a player casts a spell of the chosen color,
      you may gain 1 life," and "{T}: Add one mana of the chosen color
      to your mana pool." The first and second abilities are
      linked. The first and third abilities are linked.

      607.5. If an object acquires a pair of linked abilities as part
      of the same effect, the abilities will be similarly linked to
      one another on that object even though they weren't printed on
      that object. They can't be linked to any other ability,
      regardless of what other abilities the object may currently have
      or may have had in the past.  Example: Arc-Slogger has the
      ability "{R}, Exile the top ten cards of your library:
      Arc-Slogger deals 2 damage to target creature or player."
      Sisters of Stone Death has the ability "{B}{G}: Exile target
      creature blocking or blocked by Sisters of Stone Death" and the
      ability "{2}{B}: Put a creature card exiled with Sisters of
      Stone Death onto the battlefield under your control."
      Quicksilver Elemental has the ability "{U}: Quicksilver
      Elemental gains all activated abilities of target creature until
      end of turn." If a player has Quicksilver Elemental gain
      Arc-Slogger's ability, activates it, then has Quicksilver
      Elemental gain Sisters of Stone Death's abilities, activates the
      exile ability, and then activates the return-to-the-battlefield
      ability, only the creature card Quicksilver Elemental exiled
      with Sisters of Stone Death's ability can be returned to the
      battlefield. Creature cards Quicksilver Elemental exiled with
      Arc-Slogger's ability can't be returned.
    */
}

SCENARIO("608. Resolving Spells and Abilities","") {
    /*
      608.1. Each time all players pass in succession, the spell or
      ability on top of the stack resolves. (See rule 609, "Effects.")

      608.2. If the object that's resolving is an instant spell, a
      sorcery spell, or an ability, its resolution may involve several
      steps. The steps described in rules 608.2a and 608.2b are
      followed first. The steps described in rules 608.2c�j are
      then followed as appropriate, in no specific order. The step
      described in rule 608.2k is followed last.

      608.2a If a triggered ability has an intervening "if" clause, it
      checks whether the clause's condition is true. If it isn't, the
      ability is removed from the stack and does nothing. Otherwise,
      it continues to resolve. See rule 603.4.

      608.2b If the spell or ability specifies targets, it checks
      whether the targets are still legal. A target that's no longer
      in the zone it was in when it was targeted is illegal. Other
      changes to the game state may cause a target to no longer be
      legal; for example, its characteristics may have changed or an
      effect may have changed the text of the spell. If the source of
      an ability has left the zone it was in, its last known
      information is used during this process. The spell or ability is
      countered if all its targets, for every instance of the word
      "target," are now illegal. If the spell or ability is not
      countered, it will resolve normally. Illegal targets, if any,
      won't be affected by parts of a resolving spell's effect for
      which they're illegal. Other parts of the effect for which those
      targets are not illegal may still affect them. If the spell or
      ability creates any continuous effects that affect game rules
      (see rule 613.10), those effects don't apply to illegal
      targets. If part of the effect requires information about an
      illegal target, it fails to determine any such information. Any
      part of the effect that requires that information won't happen.
      Example: Sorin's Thirst is a black instant that reads, "Sorin's
      Thirst deals 2 damage to target creature and you gain 2 life."
      If the creature isn't a legal target during the resolution of
      Sorin's Thirst (say, if the creature has gained protection from
      black or left the battlefield), then Sorin's Thirst is
      countered. Its controller doesn't gain any life.  Example:
      Plague Spores reads, "Destroy target nonblack creature and
      target land. They can't be regenerated." Suppose the same
      animated land is chosen both as the nonblack creature and as the
      land, and the color of the creature land is changed to black
      before Plague Spores resolves. Plagues Spores isn't countered
      because the black creature land is still a legal target for the
      "target land" part of the spell. The "destroy target nonblack
      creature" part of the spell won't affect that permanent, but the
      "destroy target land" part of the spell will still destroy
      it. It can't be regenerated.

      608.2c The controller of the spell or ability follows its
      instructions in the order written. However, replacement effects
      may modify these actions. In some cases, later text on the card
      may modify the meaning of earlier text (for example, "Destroy
      target creature. It can't be regenerated" or "Counter target
      spell. If that spell is countered this way, put it on top of its
      owner's library instead of into its owner's graveyard.") Don't
      just apply effects step by step without thinking in these
      cases�read the whole text and apply the rules of English to
      the text.

      608.2d If an effect of a spell or ability offers any choices
      other than choices already made as part of casting the spell,
      activating the ability, or otherwise putting the spell or
      ability on the stack, the player announces these while applying
      the effect. The player can't choose an option that's illegal or
      impossible, with the exception that having a library with no
      cards in it doesn't make drawing a card an impossible action
      (see rule 120.3). If an effect divides or distributes something,
      such as damage or counters, as a player chooses among any number
      of untargeted players and/or objects, the player chooses the
      amount and division such that at least one player or object is
      chosen if able, and each chosen player or object receives at
      least one of whatever is being divided. (Note that if an effect
      divides or distributes something, such as damage or counters, as
      a player chooses among some number of target objects and/or
      players, the amount and division were determined as the spell or
      ability was put onto the stack rather than at this time; see
      rule 601.2d.)  Example: A spell's instruction reads, "You may
      sacrifice a creature. If you don't, you lose 4 life." A player
      who controls no creatures can't choose the sacrifice option.

      608.2e Some spells and abilities have multiple steps or actions,
      denoted by separate sentences or clauses, that involve multiple
      players. In these cases, the choices for the first action are
      made in APNAP order, and then the first action is processed
      simultaneously. Then the choices for the second action are made
      in APNAP order, and then that action is processed
      simultaneously, and so on. See rule 101.4.

      608.2f If an effect gives a player the option to pay mana, he or
      she may activate mana abilities before taking that action. If an
      effect specifically instructs or allows a player to cast a spell
      during resolution, he or she does so by following the steps in
      rules 601.2a�i, except no player receives priority after it's
      cast. That spell becomes the topmost object on the stack, and
      the currently resolving spell or ability continues to resolve,
      which may include casting other spells this way. No other spells
      can normally be cast and no other abilities can normally be
      activated during resolution.

      608.2g If an effect requires information from the game (such as
      the number of creatures on the battlefield), the answer is
      determined only once, when the effect is applied. If the effect
      requires information from a specific object, including the
      source of the ability itself, the effect uses the current
      information of that object if it's in the public zone it was
      expected to be in; if it's no longer in that zone, or if the
      effect has moved it from a public zone to a hidden zone, the
      effect uses the object's last known information. See rule
      112.7a. If an ability states that an object does something, it's
      the object as it exists�or as it most recently
      existed�that does it, not the ability.

      608.2h If an effect refers to certain characteristics, it checks
      only for the value of the specified characteristics, regardless
      of any related ones an object may also have.  Example: An effect
      that reads "Destroy all black creatures" destroys a
      white-and-black creature, but one that reads "Destroy all
      nonblack creatures" doesn't.

      608.2i If an ability's effect refers to a specific untargeted
      object that has been previously referred to by that ability's
      cost or trigger condition, it still affects that object even if
      the object has changed characteristics.  Example: Wall of Tears
      says "Whenever Wall of Tears blocks a creature, return that
      creature to its owner's hand at end of combat." If Wall of Tears
      blocks a creature, then that creature ceases to be a creature
      before the triggered ability resolves, the permanent will still
      be returned to its owner's hand.

      608.2j If an instant spell, sorcery spell, or ability that can
      legally resolve leaves the stack once it starts to resolve, it
      will continue to resolve fully.

      608.2k As the final part of an instant or sorcery spell's
      resolution, the spell is put into its owner's graveyard. As the
      final part of an ability's resolution, the ability is removed
      from the stack and ceases to exist.

      608.3. If the object that's resolving is a permanent spell, its
      resolution involves a single step (unless it's an Aura). The
      spell card becomes a permanent and is put onto the battlefield
      under the control of the spell's controller.

      608.3a If the object that's resolving is an Aura spell, its
      resolution involves two steps. First, it checks whether the
      target specified by its enchant ability is still legal, as
      described in rule 608.2b. (See rule 702.5, "Enchant.") If so,
      the spell card becomes a permanent and is put onto the
      battlefield under the control of the spell's controller attached
      to the object it was targeting.

      608.3b If a permanent spell resolves but its controller can't
      put it onto the battlefield, that player puts it into its
      owner's graveyard.  Example: Worms of the Earth has the ability
      "Lands can't enter the battlefield." Clone says "You may have
      Clone enter the battlefield as a copy of any creature on the
      battlefield." If a player casts Clone and chooses to copy Dryad
      Arbor (a land creature) while Worms of the Earth is on the
      battlefield, Clone can't enter the battlefield from the
      stack. It's put into its owner's graveyard.
    */
}

SCENARIO("609. Effects","") {
    /*
      609.1. An effect is something that happens in the game as a
      result of a spell or ability. When a spell, activated ability,
      or triggered ability resolves, it may create one or more
      one-shot or continuous effects. Static abilities may create one
      or more continuous effects. Text itself is never an effect.

      609.2. Effects apply only to permanents unless the instruction's
      text states otherwise or they clearly can apply only to objects
      in one or more other zones.  Example: An effect that changes all
      lands into creatures won't alter land cards in players'
      graveyards. But an effect that says spells cost more to cast
      will apply only to spells on the stack, since a spell is always
      on the stack while a player is casting it.

      609.3. If an effect attempts to do something impossible, it does
      only as much as possible.  Example: If a player is holding only
      one card, an effect that reads "Discard two cards" causes him or
      her to discard only that card. If an effect moves cards out of
      the library (as opposed to drawing), it moves as many as
      possible.

      609.4. Some effects state that a player may do something "as
      though" some condition were true or a creature can do something
      "as though" some condition were true. This applies only to the
      stated effect. For purposes of that effect, treat the game
      exactly as if the stated condition were true. For all other
      purposes, treat the game normally.

      609.4a If two effects state that a player may (or a creature
      can) do the same thing "as though" different conditions were
      true, both conditions could apply. If one "as though" effect
      satisfies the requirements for another "as though" effect, then
      both effects will apply.  Example: A player controls Vedalken
      Orrery, an artifact that says "You may cast nonland cards as
      though they had flash." That player casts Shaman's Trance, an
      instant that says, in part, "You may play cards from other
      players' graveyards as though they were in your graveyard." The
      player may cast a sorcery with flashback from another player's
      graveyard as though it were in that player's graveyard and as
      though it had flash.

      609.5. If an effect could result in a tie, the text of the spell
      or ability that created the effect will specify what to do in
      the event of a tie. The Magic game has no default for ties.

      609.6. Some continuous effects are replacement effects or
      prevention effects. See rules 614 and 615.

      609.7. Some effects apply to damage from a source�for
      example, "The next time a red source of your choice would deal
      damage to you this turn, prevent that damage."

      609.7a If an effect requires a player to choose a source of
      damage, he or she may choose a permanent; a spell on the stack
      (including a permanent spell); any object referred to by an
      object on the stack, by a replacement or prevention effect
      that's waiting to apply, or by a delayed triggered ability
      that's waiting to trigger (even if that object is no longer in
      the zone it used to be in); or a face-up object in the command
      zone. A source doesn't need to be capable of dealing damage to
      be a legal choice. The source is chosen when the effect is
      created. If the player chooses a permanent, the effect will
      apply to the next damage dealt by that permanent, regardless of
      whether it's combat damage or damage dealt as the result of a
      spell or ability. If the player chooses a permanent spell, the
      effect will apply to any damage dealt by that spell and any
      damage dealt by the permanent that spell becomes when it
      resolves.

      609.7b Some effects from resolved spells and abilities prevent
      or replace damage from sources with certain properties, such as
      a creature or a source of a particular color. When the source
      would deal damage, the "shield" rechecks the source's
      properties. If the properties no longer match, the damage isn't
      prevented or replaced. If for any reason the shield prevents no
      damage or replaces no damage, the shield isn't used up.

      609.7c Some effects from static abilities prevent or replace
      damage from sources with certain properties. For these effects,
      the prevention or replacement applies to sources that are
      permanents with that property and to any sources that aren't on
      the battlefield that have that property.
    */
}

SCENARIO("610. One-Shot Effects","") {
    /*
      610.1. A one-shot effect does something just once and doesn't
      have a duration. Examples include dealing damage, destroying a
      permanent, creating a token, and moving an object from one zone
      to another.

      610.2. Some one-shot effects create a delayed triggered ability,
      which instructs a player to do something later in the game
      (usually at a specific time) rather than as the spell or ability
      that's creating the one-shot effect resolves. See rule 603.7.

      610.3. Some one-shot effects cause an object to change zones
      "until" a specified event occurs. A second one-shot effect is
      created immediately after the specified event. This second
      one-shot effect returns the object to its previous zone.

      610.3a If the specified event has already occurred when the
      initial one-shot effect would cause the object to change zones,
      the object doesn't move.

      610.3b An object returned to the battlefield this way returns
      under its owner's control unless otherwise specified.

      610.3c If multiple one-shot effects are created this way
      immediately after one or more simultaneous events, those
      one-shot effects are also simultaneous.  Example: Two Banisher
      Priests have each exiled a card. All creatures are destroyed at
      the same time by Day of Judgment. The two exiled cards are
      returned to the battlefield at the same time.
    */
}

SCENARIO("611. Continuous Effects","") {
    /*

      611.1. A continuous effect modifies characteristics of objects,
      modifies control of objects, or affects players or the rules of
      the game, for a fixed or indefinite period.

      611.2. A continuous effect may be generated by the resolution of
      a spell or ability.

      611.2a A continuous effect generated by the resolution of a
      spell or ability lasts as long as stated by the spell or ability
      creating it (such as "until end of turn"). If no duration is
      stated, it lasts until the end of the game.

      611.2b Some continuous effects generated by the resolution of a
      spell or ability have durations worded "for as long as . . . ."
      If the "for as long as" duration never starts, or it ends before
      the moment the effect would first be applied, the effect does
      nothing. It doesn't start and immediately stop again, and it
      doesn't last forever.  Example: Master Thief has the ability
      "When Master Thief enters the battlefield, gain control of
      target artifact for as long as you control Master Thief." If you
      lose control of Master Thief before the ability resolves, it
      does nothing, because its duration�as long as you control
      Master Thief�was over before the effect began.

      611.2c If a continuous effect generated by the resolution of a
      spell or ability modifies the characteristics or changes the
      controller of any objects, the set of objects it affects is
      determined when that continuous effect begins. After that point,
      the set won't change. (Note that this works differently than a
      continuous effect from a static ability.) A continuous effect
      generated by the resolution of a spell or ability that doesn't
      modify the characteristics or change the controller of any
      objects modifies the rules of the game, so it can affect objects
      that weren't affected when that continuous effect began. If a
      single continuous effect has parts that modify the
      characteristics or changes the controller of any objects and
      other parts that don't, the set of objects each part applies to
      is determined independently.  Example: An effect that reads "All
      white creatures get +1/+1 until end of turn" gives the bonus to
      all permanents that are white creatures when the spell or
      ability resolves�even if they change color later�and
      doesn't affect those that enter the battlefield or turn white
      afterward.  Example: An effect that reads "Prevent all damage
      creatures would deal this turn" doesn't modify any object's
      characteristics, so it's modifying the rules of the game. That
      means the effect will apply even to damage from creatures that
      weren't on the battlefield when the continuous effect began. It
      also affects damage from permanents that become creatures later
      in the turn.

      611.2d If a resolving spell or ability that creates a continuous
      effect contains a variable such as X, the value of that variable
      is determined only once, on resolution. See rule 608.2g.

      611.2e If a resolving spell or ability both puts a nontoken
      permanent onto the battlefield and creates a continuous effect
      stating that the permanent "is [characteristic]," that
      continuous effect applies simultaneously with the permanent
      entering the battlefield. This characteristic is usually a color
      or a creature type. If the continuous effect says the permanent
      "becomes [characteristic]" or "gains [an ability]," that effect
      applies after the permanent is on the battlefield.  Example:
      Arbiter of the Ideal puts an artifact, creature, or land card
      onto the battlefield and says, in part, "That permanent is an
      enchantment in addition to its other types." An ability that
      triggers whenever an enchantment enters the battlefield would
      trigger. The permanent doesn't enter the battlefield and then
      become an enchantment.

      611.3. A continuous effect may be generated by the static
      ability of an object.

      611.3a A continuous effect generated by a static ability isn't
      "locked in"; it applies at any given moment to whatever its text
      indicates.

      611.3b The effect applies at all times that the permanent
      generating it is on the battlefield or the object generating it
      is in the appropriate zone.  Example: A permanent with the
      static ability "All white creatures get +1/+1" generates an
      effect that continuously gives +1/+1 to each white creature on
      the battlefield. If a creature becomes white, it gets this
      bonus; a creature that stops being white loses it.

      611.3c Continuous effects that modify characteristics of
      permanents do so simultaneously with the permanent entering the
      battlefield. They don't wait until the permanent is on the
      battlefield and then change it. Because such effects apply as
      the permanent enters the battlefield, they are applied before
      determining whether the permanent will cause an ability to
      trigger when it enters the battlefield.  Example: A permanent
      with the static ability "All white creatures get +1/+1" is on
      the battlefield. A creature spell that would normally create a
      1/1 white creature instead creates a 2/2 white creature. The
      creature doesn't enter the battlefield as 1/1 and then change to
      2/2.
    */
}

SCENARIO("612. Text-Changing Effects","") {
    /*
      612.1. Some continuous effects change an object's text. This can
      apply to any words or symbols printed on that object, but
      generally affects only that object's rules text (which appears
      in its text box) and/or the text that appears in its type
      line. Such an effect is a text-changing effect.

      612.2. A text-changing effect changes only those words that are
      used in the correct way (for example, a Magic color word being
      used as a color word, a land type word used as a land type, or a
      creature type word used as a creature type). An effect that
      changes a color word or a subtype can't change a card name, even
      if that name contains a word or a series of letters that is the
      same as a Magic color word, basic land type, or creature type.

      612.2a Most spells and abilities that create creature tokens use
      creature types to define both the creature types and the names
      of the tokens. A text-changing effect that affects such a spell
      or an object with such an ability can change these words because
      they're being used as creature types, even though they're also
      being used as names.

      612.3. Effects that add or remove abilities don't change the
      text of the objects they affect, so any abilities that are
      granted to an object can't be modified by text-changing effects
      that affect that object.

      612.4. A token's subtypes and rules text are defined by the
      spell or ability that created the token. A text-changing effect
      that affects a token can change these characteristics.

      612.5. One card (Volrath's Shapeshifter) states that an object
      has the "full text" of another object. This changes not just the
      text that appears in the object's text box and type line, but
      also changes the text that represents its name, mana cost, color
      indicator, power, and toughness.

      612.6. One card (Spy Kit) states that an object has "all names
      of nonlegendary creature cards." This changes the text that
      represents the object's name. That object has the name of each
      nonlegendary creature card in the Oracle card reference that's
      legal in the format of the game it's in. (See rule 100.6.)
    */
}

SCENARIO("613. Interaction of Continuous Effects","") {
    /*
      613.1. The values of an object's characteristics are determined
      by starting with the actual object. For a card, that means the
      values of the characteristics printed on that card. For a token
      or a copy of a spell or card, that means the values of the
      characteristics defined by the effect that created it. Then all
      applicable continuous effects are applied in a series of layers
      in the following order:

      613.1a Layer 1: Copy effects are applied. See rule 706, "Copying
      Objects."

      613.1b Layer 2: Control-changing effects are applied.

      613.1c Layer 3: Text-changing effects are applied. See rule 612,
      "Text-Changing Effects."

      613.1d Layer 4: Type-changing effects are applied. These include
      effects that change an object's card type, subtype, and/or
      supertype.

      613.1e Layer 5: Color-changing effects are applied.

      613.1f Layer 6: Ability-adding effects, ability-removing
      effects, and effects that say an object can't have an ability
      are applied.

      613.1g Layer 7: Power- and/or toughness-changing effects are
      applied.

      613.2. Within layers 1�6, apply effects from
      characteristic-defining abilities first (see rule 604.3), then
      all other effects in timestamp order (see rule 613.6). Note that
      dependency may alter the order in which effects are applied
      within a layer. (See rule 613.7.)

      613.3. Within layer 7, apply effects in a series of sublayers in
      the order described below. Within each sublayer, apply effects
      in timestamp order. (See rule 613.6.) Note that dependency may
      alter the order in which effects are applied within a
      sublayer. (See rule 613.7.)

      613.3a Layer 7a: Effects from characteristic-defining abilities
      that define power and/or toughness are applied. See rule 604.3.

      613.3b Layer 7b: Effects that set power and/or toughness to a
      specific number or value are applied. Effects that refer to the
      base power and/or toughness of a creature apply in this layer.

      613.3c Layer 7c: Effects that modify power and/or toughness (but
      don't set power and/or toughness to a specific number or value)
      are applied.

      613.3d Layer 7d: Power and/or toughness changes from counters
      are applied. See rule 121, "Counters."

      613.3e Layer 7e: Effects that switch a creature's power and
      toughness are applied. Such effects take the value of power and
      apply it to the creature's toughness, and take the value of
      toughness and apply it to the creature's power.  Example: A 1/3
      creature is given +0/+1 by an effect. Then another effect
      switches the creature's power and toughness. Its new power and
      toughness is 4/1. A new effect gives the creature +5/+0. Its
      "unswitched" power and toughness would be 6/4, so its actual
      power and toughness is 4/6.  Example: A 1/3 creature is given
      +0/+1 by an effect. Then another effect switches the creature's
      power and toughness. Its new power and toughness is 4/1. If the
      +0/+1 effect ends before the switch effect ends, the creature
      becomes 3/1.  Example: A 1/3 creature is given +0/+1 by an
      effect. Then another effect switches the creature's power and
      toughness. Then another effect switches its power and toughness
      again. The two switches essentially cancel each other, and the
      creature becomes 1/4.

      613.4. The application of continuous effects as described by the
      layer system is continually and automatically performed by the
      game. All resulting changes to an object's characteristics are
      instantaneous.  Example: Honor of the Pure is an enchantment
      that reads "White creatures you control get +1/+1." Honor of the
      Pure and a 2/2 black creature are on the battlefield under your
      control. If an effect then turns the creature white (layer 5),
      it gets +1/+1 from Honor of the Pure (layer 7c), becoming
      3/3. If the creature's color is later changed to red (layer 5),
      Honor of the Pure's effect stops applying to it, and it will
      return to being 2/2.  Example: Gray Ogre, a 2/2 creature, is on
      the battlefield. An effect puts a +1/+1 counter on it (layer
      7d), making it 3/3. A spell targeting it that says "Target
      creature gets +4/+4 until end of turn" resolves (layer 7c),
      making it 7/7. An enchantment that says "Creatures you control
      get +0/+2" enters the battlefield (layer 7c), making it 7/9. An
      effect that says "Target creature becomes 0/1 until end of turn"
      is applied to it (layer 7b), making it 5/8 (0/1, with +4/+4 from
      the resolved spell, +0/+2 from the enchantment, and +1/+1 from
      the counter).

      613.5. If an effect should be applied in different layers and/or
      sublayers, the parts of the effect each apply in their
      appropriate ones. If an effect starts to apply in one layer
      and/or sublayer, it will continue to be applied to the same set
      of objects in each other applicable layer and/or sublayer, even
      if the ability generating the effect is removed during this
      process.  Example: An effect that reads "Wild Mongrel gets +1/+1
      and becomes the color of your choice until end of turn" is both
      a power- and toughness-changing effect and a color-changing
      effect. The "becomes the color of your choice" part is applied
      in layer 5, and then the "gets +1/+1" part is applied in layer
      7c.  Example: Act of Treason has an effect that reads "Gain
      control of target creature until end of turn. Untap that
      creature. It gains haste until end of turn." This is both a
      control-changing effect and an effect that adds an ability to an
      object. The "gain control" part is applied in layer 2, and then
      the "it gains haste" part is applied in layer 6.  Example: An
      effect that reads "All noncreature artifacts become 2/2 artifact
      creatures until end of turn" is both a type-changing effect and
      a power- and toughness-setting effect. The type-changing effect
      is applied to all noncreature artifacts in layer 4 and the
      power- and toughness-setting effect is applied to those same
      permanents in layer 7b, even though those permanents aren't
      noncreature artifacts by then.  Example: Svogthos, the Restless
      Tomb, is on the battlefield. An effect that says "Until end of
      turn, target land becomes a 3/3 creature that's still a land" is
      applied to it (layers 4 and 7b). An effect that says "Target
      creature gets +1/+1 until end of turn" is applied to it (layer
      7c), making it a 4/4 land creature. Then while you have ten
      creature cards in your graveyard, you activate Svogthos's
      ability: "Until end of turn, Svogthos, the Restless Tomb becomes
      a black and green Plant Zombie creature with �This creature's
      power and toughness are each equal to the number of creature
      cards in your graveyard.' It's still a land." (layers 4, 5, and
      7b). It becomes an 11/11 land creature. If a creature card
      enters or leaves your graveyard, Svogthos's power and toughness
      will be modified accordingly. If the first effect is applied to
      it again, it will become a 4/4 land creature again.

      613.6. Within a layer or sublayer, determining which order
      effects are applied in is usually done using a timestamp
      system. An effect with an earlier timestamp is applied before an
      effect with a later timestamp.

      613.6a A continuous effect generated by a static ability has the
      same timestamp as the object the static ability is on, or the
      timestamp of the effect that created the ability, whichever is
      later.

      613.6b A continuous effect generated by the resolution of a
      spell or ability receives a timestamp at the time it's created.

      613.6c An object receives a timestamp at the time it enters a
      zone.

      613.6d An Aura, Equipment, or Fortification receives a new
      timestamp at the time it becomes attached to an object or
      player.

      613.6e A permanent receives a new timestamp at the time it turns
      face up or face down.

      613.6f A double-faced permanent receives a new timestamp at the
      time it transforms.

      613.6g A face-up plane card, phenomenon card, or scheme card
      receives a timestamp at the time it's turned face up.

      613.6h A face-up vanguard card receives a timestamp at the
      beginning of the game.

      613.6i A conspiracy card receives a timestamp at the beginning
      of the game. If it's face down, it receives a new timestamp at
      the time it turns face up.

      613.6j If two or more objects would receive a timestamp
      simultaneously, such as by entering a zone simultaneously or
      becoming attached simultaneously, the active player determines
      their relative timestamp order at that time.

      613.7. Within a layer or sublayer, determining which order
      effects are applied in is sometimes done using a dependency
      system. If a dependency exists, it will override the timestamp
      system.

      613.7a An effect is said to "depend on" another if (a) it's
      applied in the same layer (and, if applicable, sublayer) as the
      other effect (see rules 613.1 and 613.3); (b) applying the other
      would change the text or the existence of the first effect, what
      it applies to, or what it does to any of the things it applies
      to; and (c) neither effect is from a characteristic-defining
      ability or both effects are from characteristic-defining
      abilities. Otherwise, the effect is considered to be independent
      of the other effect.

      613.7b An effect dependent on one or more other effects waits to
      apply until just after all of those effects have been
      applied. If multiple dependent effects would apply
      simultaneously in this way, they're applied in timestamp order
      relative to each other. If several dependent effects form a
      dependency loop, then this rule is ignored and the effects in
      the dependency loop are applied in timestamp order.

      613.7c After each effect is applied, the order of remaining
      effects is reevaluated and may change if an effect that has not
      yet been applied becomes dependent on or independent of one or
      more other effects that have not yet been applied.

      613.8. One continuous effect can override another. Sometimes the
      results of one effect determine whether another effect applies
      or what another effect does.  Example: Two effects are affecting
      the same creature: one from an Aura that says "Enchanted
      creature gains flying" and one from an Aura that says "Enchanted
      creature loses flying." Neither of these depends on the other,
      since nothing changes what they affect or what they're doing to
      it. Applying them in timestamp order means the one that was
      generated last "wins." The same process would be followed, and
      the same result reached, if either of the effects had a duration
      (such as "Target creature loses flying until end of turn") or
      came from a non-Aura source (such as "All creatures lose
      flying").  Example: One effect reads, "White creatures get
      +1/+1," and another reads, "Enchanted creature is white." The
      enchanted creature gets +1/+1 from the first effect, regardless
      of its previous color.

      613.9. Some continuous effects affect players rather than
      objects. For example, an effect might give a player protection
      from red. All such effects are applied in timestamp order after
      the determination of objects' characteristics. See also the
      rules for timestamp order and dependency (rules 613.6 and
      613.7).

      613.10. Some continuous effects affect game rules rather than
      objects. For example, effects may modify a player's maximum hand
      size, or say that a creature must attack this turn if
      able. These effects are applied after all other continuous
      effects have been applied. Continuous effects that affect the
      costs of spells or abilities are applied according to the order
      specified in rule 601.2f. All other such effects are applied in
      timestamp order. See also the rules for timestamp order and
      dependency (rules 613.6 and 613.7).
    */
}

SCENARIO("614. Replacement Effects","") {
    /*
      614.1. Some continuous effects are replacement effects. Like
      prevention effects (see rule 615), replacement effects apply
      continuously as events happen�they aren't locked in ahead of
      time. Such effects watch for a particular event that would
      happen and completely or partially replace that event with a
      different event. They act like "shields" around whatever they're
      affecting.

      614.1a Effects that use the word "instead" are replacement
      effects. Most replacement effects use the word "instead" to
      indicate what events will be replaced with other events.

      614.1b Effects that use the word "skip" are replacement
      effects. These replacement effects use the word "skip" to
      indicate what events, steps, phases, or turns will be replaced
      with nothing.

      614.1c Effects that read "[This permanent] enters the
      battlefield with . . . ," "As [this permanent] enters the
      battlefield . . . ," or "[This permanent] enters the battlefield
      as . . . " are replacement effects.

      614.1d Continuous effects that read "[This permanent] enters the
      battlefield . . ." or "[Objects] enter the battlefield . . ."
      are replacement effects.

      614.1e Effects that read "As [this permanent] is turned face up
      . . . ," are replacement effects.

      614.2. Some replacement effects apply to damage from a
      source. See rule 609.7.

      614.3. There are no special restrictions on casting a spell or
      activating an ability that generates a replacement effect. Such
      effects last until they're used up or their duration has
      expired.

      614.4. Replacement effects must exist before the appropriate
      event occurs�they can't "go back in time" and change
      something that's already happened. Spells or abilities that
      generate these effects are often cast or activated in response
      to whatever would produce the event and thus resolve before that
      event would occur.  Example: A player can activate an ability to
      regenerate a creature in response to a spell that would destroy
      it. Once the spell resolves, though, it's too late to regenerate
      the creature.

      614.5. A replacement effect doesn't invoke itself repeatedly; it
      gets only one opportunity to affect an event or any modified
      events that may replace it.  Example: A player controls two
      permanents, each with an ability that reads "If a creature you
      control would deal damage to a creature or player, it deals
      double that damage to that creature or player instead." A
      creature that normally deals 2 damage will deal 8 damage�not
      just 4, and not an infinite amount.

      614.6. If an event is replaced, it never happens. A modified
      event occurs instead, which may in turn trigger abilities. Note
      that the modified event may contain instructions that can't be
      carried out, in which case the impossible instruction is simply
      ignored.

      614.7. If a replacement effect would replace an event, but that
      event never happens, the replacement effect simply doesn't do
      anything.

      614.7a If a source would deal 0 damage, it does not deal damage
      at all. Replacement effects that would increase the damage dealt
      by that source, or would have that source deal that damage to a
      different object or player, have no event to replace, so they
      have no effect.

      614.8. Regeneration is a destruction-replacement effect. The
      word "instead" doesn't appear on the card but is implicit in the
      definition of regeneration. "Regenerate [permanent]" means "The
      next time [permanent] would be destroyed this turn, instead
      remove all damage marked on it and tap it. If it's an attacking
      or blocking creature, remove it from combat." Abilities that
      trigger from damage being dealt still trigger even if the
      permanent regenerates. See rule 701.13.

      614.9. Some effects replace damage dealt to one creature,
      planeswalker, or player with the same damage dealt to another
      creature, planeswalker, or player; such effects are called
      redirection effects. If either creature or planeswalker is no
      longer on the battlefield when the damage would be redirected,
      or is no longer a creature or planeswalker when the damage would
      be redirected, the effect does nothing. If damage would be
      redirected to or from a player who has left the game, the effect
      does nothing.

      614.10. An effect that causes a player to skip an event, step,
      phase, or turn is a replacement effect. "Skip [something]" is
      the same as "Instead of doing [something], do nothing." Once a
      step, phase, or turn has started, it can no longer be
      skipped�any skip effects will wait until the next occurrence.

      614.10a Anything scheduled for a skipped step, phase, or turn
      won't happen. Anything scheduled for the "next" occurrence of
      something waits for the first occurrence that isn't skipped. If
      two effects each cause a player to skip his or her next
      occurrence, that player must skip the next two; one effect will
      be satisfied in skipping the first occurrence, while the other
      will remain until another occurrence can be skipped.

      614.10b Some effects cause a player to skip a step, phase, or
      turn, then take another action. That action is considered to be
      the first thing that happens during the next step, phase, or
      turn to actually occur.

      614.11. Some effects replace card draws. These effects are
      applied even if no cards could be drawn because there are no
      cards in the affected player's library.

      614.11a If an effect replaces a draw within a sequence of card
      draws, all actions required by the replacement are completed, if
      possible, before resuming the sequence.

      614.11b If an effect would have a player both draw a card and
      perform an additional action on that card, and the draw is
      replaced, the additional action is not performed on any cards
      that are drawn as a result of that replacement effect.

      614.12. Some replacement effects modify how a permanent enters
      the battlefield. (See rules 614.1c�d.) Such effects may come
      from the permanent itself if they affect only that permanent (as
      opposed to a general subset of permanents that includes
      it). They may also come from other sources. To determine which
      replacement effects apply and how they apply, check the
      characteristics of the permanent as it would exist on the
      battlefield, taking into account replacement effects that have
      already modified how it enters the battlefield (see rule 616.1),
      continuous effects generated by the resolution of spells or
      abilities that changed the permanent's characteristics on the
      stack (see rule 400.7a), and continuous effects from the
      permanent's own static abilities, but ignoring continuous
      effects from any other source that would affect it.  Example:
      Voice of All says "As Voice of All enters the battlefield,
      choose a color" and "Voice of All has protection from the chosen
      color." An effect creates a token that's a copy of Voice of
      All. As that token is created, the token's controller chooses a
      color for it.  Example: Yixlid Jailer says "Cards in graveyards
      lose all abilities." Scarwood Treefolk says "Scarwood Treefolk
      enters the battlefield tapped." A Scarwood Treefolk that's put
      onto the battlefield from a graveyard enters the battlefield
      tapped.  Example: Orb of Dreams is an artifact that says
      "Permanents enter the battlefield tapped." It won't affect
      itself, so Orb of Dreams enters the battlefield untapped.

      614.12a If a replacement effect that modifies how a permanent
      enters the battlefield requires a choice, that choice is made
      before the permanent enters the battlefield.

      614.12b Some replacement effects cause a permanent to enter the
      battlefield with its controller's choice of one of two
      abilities, each marked with an anchor word and preceded by a
      bullet point. "[Anchor word] � [ability]" means "As long as
      [anchor word] was chosen as this permanent entered the
      battlefield, this permanent has [ability]." The abilities
      preceded by anchor words are each linked to the ability that
      causes a player to choose between them. See rule 607, "Linked
      Abilities."

      614.13. An effect that modifies how a permanent enters the
      battlefield may cause other objects to change zones.

      614.13a While applying an effect that modifies how a permanent
      enters the battlefield, you may have to choose a number of
      objects that will also change zones. You can't choose the object
      that will become that permanent or any other object entering the
      battlefield at the same time as that object.  Example: Sutured
      Ghoul says, in part, "As Sutured Ghoul enters the battlefield,
      exile any number of creature cards from your graveyard." If
      Sutured Ghoul and Runeclaw Bear enter the battlefield from your
      graveyard at the same time, you can't choose to exile either of
      them when applying Sutured Ghoul's replacement effect.

      614.13b The same object can't be chosen to change zones more
      than once when applying replacement effects that modify how a
      single permanent enters the battlefield.  Example: Jund (a plane
      card) says, "Whenever a player casts a black, red, or green
      creature spell, it gains devour 5." A player controls Runeclaw
      Bear and casts Thunder-Thrash Elder, a red creature spell with
      devour 3. As Thunder-Thrash Elder enters the battlefield, its
      controller can choose to sacrifice Runeclaw Bear when applying
      the devour 3 effect or when applying the devour 5 effect, but
      not both. Thunder-Thrash Elder will enter the battlefield with
      zero, three, or five +1/+1 counters, depending on this choice.

      614.14. An object may have one ability printed on it that
      generates a replacement effect which causes one or more cards to
      be exiled, and another ability that refers either to "the exiled
      cards" or to cards "exiled with [this object]." These abilities
      are linked: the second refers only to cards in the exile zone
      that were put there as a direct result of the replacement event
      caused by the first. If another object gains a pair of linked
      abilities, the abilities will be similarly linked on that
      object. They can't be linked to any other ability, regardless of
      what other abilities the object may currently have or may have
      had in the past. See rule 607, "Linked Abilities."

      614.15. Some replacement effects are not continuous
      effects. Rather, they are an effect of a resolving spell or
      ability that replace part or all of that spell or ability's own
      effect(s). Such effects are called self-replacement effects. The
      text creating a self-replacement effect is usually part of the
      ability whose effect is being replaced, but the text can be a
      separate ability, particularly when preceded by an ability
      word. When applying replacement effects to an event,
      self-replacement effects are applied before other replacement
      effects.
    */
}

SCENARIO("615. Prevention Effects","") {
    /*
      615.1. Some continuous effects are prevention effects. Like
      replacement effects (see rule 614), prevention effects apply
      continuously as events happen�they aren't locked in ahead of
      time. Such effects watch for a damage event that would happen
      and completely or partially prevent the damage that would be
      dealt. They act like "shields" around whatever they're
      affecting.

      615.1a Effects that use the word "prevent" are prevention
      effects. Prevention effects use "prevent" to indicate what
      damage will not be dealt.

      615.2. Many prevention effects apply to damage from a
      source. See rule 609.7.

      615.3. There are no special restrictions on casting a spell or
      activating an ability that generates a prevention effect. Such
      effects last until they're used up or their duration has
      expired.

      615.4. Prevention effects must exist before the appropriate
      damage event occurs�they can't "go back in time" and change
      something that's already happened. Spells or abilities that
      generate these effects are often cast or activated in response
      to whatever would produce the event and thus resolve before that
      event would occur.  Example: A player can activate an ability
      that prevents damage in response to a spell that would deal
      damage. Once the spell resolves, though, it's too late to
      prevent the damage.

      615.5. Some prevention effects also include an additional
      effect, which may refer to the amount of damage that was
      prevented. The prevention takes place at the time the original
      event would have happened; the rest of the effect takes place
      immediately afterward.

      615.6. If damage that would be dealt is prevented, it never
      happens. A modified event may occur instead, which may in turn
      trigger abilities. Note that the modified event may contain
      instructions that can't be carried out, in which case the
      impossible instruction is simply ignored.

      615.7. Some prevention effects generated by the resolution of a
      spell or ability refer to a specific amount of damage�for
      example, "Prevent the next 3 damage that would be dealt to
      target creature or player this turn." These work like
      shields. Each 1 damage that would be dealt to the "shielded"
      creature or player is prevented. Preventing 1 damage reduces the
      remaining shield by 1. If damage would be dealt to the shielded
      creature or player by two or more applicable sources at the same
      time, the player or the controller of the creature chooses which
      damage the shield prevents. Once the shield has been reduced to
      0, any remaining damage is dealt normally. Such effects count
      only the amount of damage; the number of events or sources
      dealing it doesn't matter.

      615.8. Some prevention effects generated by the resolution of a
      spell or ability refer to the next time a specific source would
      deal damage. These effects prevent the next instance of damage
      from that source, regardless of how much damage that is. Once an
      instance of damage from that source has been prevented, any
      subsequent instances of damage that would be dealt by that
      source are dealt normally.

      615.9. Some prevention effects generated by static abilities
      refer to a specific amount of damage�for example, "If a
      source would deal damage to you, prevent 1 of that damage." Such
      an effect prevents only the indicated amount of damage in any
      applicable damage event at any given time. It will apply
      separately to damage from other applicable events that would
      happen at the same time, or at a different time.  Example:
      Daunting Defender says "If a source would deal damage to a
      Cleric creature you control, prevent 1 of that damage."
      Pyroclasm says "Pyroclasm deals 2 damage to each creature."
      Pyroclasm will deal 1 damage to each Cleric creature controlled
      by Daunting Defender's controller. It will deal 2 damage to each
      other creature that player controls.

      615.10. Some prevention effects prevent the next N damage that
      would be dealt to each of a number of untargeted creatures. Such
      an effect creates a prevention shield for each applicable
      creature when the spell or ability that generates that effect
      resolves.  Example: Wojek Apothecary has an ability that says
      "{T}: Prevent the next 1 damage that would be dealt to target
      creature and each other creature that shares a color with it
      this turn." When the ability resolves, it gives the target
      creature and each other creature on the battlefield that shares
      a color with it at that time a shield preventing the next 1
      damage that would be dealt to it. Changing creatures' colors
      after the ability resolves doesn't add or remove shields, and
      creatures that enter the battlefield later in the turn don't get
      the shield.

      615.11. Some effects state that damage "can't be prevented." If
      unpreventable damage would be dealt, any applicable prevention
      effects are still applied to it. Those effects won't prevent any
      damage, but any additional effects they have will take
      place. Existing damage prevention shields won't be reduced by
      damage that can't be prevented.

      615.11a A prevention effect is applied to any particular
      unpreventable damage event just once. It won't invoke itself
      repeatedly trying to prevent that damage.

      615.12. Some triggered abilities trigger when damage that would
      be dealt is prevented. Such an ability triggers each time a
      prevention effect is applied to one or more simultaneous damage
      events and prevents some or all of that damage.
    */
}

SCENARIO("616. Interaction of Replacement and/or Prevention Effects","") {
    /*
      616.1. If two or more replacement and/or prevention effects are
      attempting to modify the way an event affects an object or
      player, the affected object's controller (or its owner if it has
      no controller) or the affected player chooses one to apply,
      following the steps listed below. If two or more players have to
      make these choices at the same time, choices are made in APNAP
      order (see rule 101.4).

      616.1a If any of the replacement and/or prevention effects are
      self-replacement effects (see rule 614.15), one of them must be
      chosen. If not, proceed to rule 616.1b.

      616.1b If any of the replacement and/or prevention effects would
      modify under whose control an object would enter the
      battlefield, one of them must be chosen. If not, proceed to rule
      616.1c.

      616.1c If any of the replacement and/or prevention effects would
      cause an object to become a copy of another object as it enters
      the battlefield, one of them must be chosen. If not, proceed to
      rule 616.1d.

      616.1d Any of the applicable replacement and/or prevention
      effects may be chosen.

      616.1e Once the chosen effect has been applied, this process is
      repeated (taking into account only replacement or prevention
      effects that would now be applicable) until there are no more
      left to apply.  Example: Two permanents are on the
      battlefield. One is an enchantment that reads "If a card would
      be put into a graveyard from anywhere, instead exile it," and
      the other is a creature that reads "If [this creature] would be
      put into a graveyard from the battlefield, instead shuffle it
      into its owner's library." If the creature is destroyed, its
      controller decides which replacement to apply first; the other
      does nothing.  Example: Essence of the Wild reads "Creatures you
      control enter the battlefield as a copy of Essence of the Wild."
      A player who controls Essence of the Wild casts Rusted Sentinel,
      which normally enters the battlefield tapped. As it enters the
      battlefield, the copy effect from Essence of the Wild is applied
      first. As a result, it no longer has the ability that causes it
      to enter the battlefield tapped. Rusted Sentinel will enter the
      battlefield as an untapped copy of Essence of the Wild.

      616.2. A replacement or prevention effect can become applicable
      to an event as the result of another replacement or prevention
      effect that modifies the event.  Example: One effect reads "If
      you would gain life, draw that many cards instead," and another
      reads "If you would draw a card, return a card from your
      graveyard to your hand instead." Both effects combine
      (regardless of the order they came into existence): Instead of
      gaining 1 life, the player puts a card from his or her graveyard
      into his or her hand.
    */
}
