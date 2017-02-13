#include "common.hpp"
#include "gamestate.hpp"
#include "factory.hpp"

#include "catch.hpp"

namespace       m   = MTG;
namespace       spd = spdlog;
using namespace entityx;

/*
  5. Turn Structure
*/

SCENARIO("500. General","") {
    /*
      500.1. A turn consists of five phases, in this order: beginning, precombat
      main, combat, postcombat main, and ending. Each of these phases takes
      place every turn, even if nothing happens during the phase. The beginning,
      combat, and ending phases are further broken down into steps, which
      proceed in order.
    */
    REQUIRE( m::Phase::UNDEFINED                 == m::Phase(0) );
    REQUIRE( m::Phase::BEGINNING                 == m::Phase(1 << 0) );
    REQUIRE( m::Phase::PRECOMBAT_MAIN            == m::Phase(1 << 1) );
    REQUIRE( m::Phase::COMBAT                    == m::Phase(1 << 2) );
    REQUIRE( m::Phase::POSTCOMBAT_MAIN           == m::Phase(1 << 3) );
    REQUIRE( m::Phase::ENDING                    == m::Phase(1 << 4) );
    
    REQUIRE( m::Step::UNDEFINED                  == m::Step(0) );
    REQUIRE( m::Step::UNTAP                      == m::Step(1 << 0) );
    REQUIRE( m::Step::UPKEEP                     == m::Step(1 << 1) );
    REQUIRE( m::Step::DRAW                       == m::Step(1 << 2) );
    REQUIRE( m::Step::BEGINNING_OF_COMBAT        == m::Step(1 << 3) );
    REQUIRE( m::Step::DECLARE_ATTACKERS          == m::Step(1 << 4) );
    REQUIRE( m::Step::DECLARE_BLOCKERS           == m::Step(1 << 5) );
    REQUIRE( m::Step::FIRST_STRIKE_COMBAT_DAMAGE == m::Step(1 << 6) );
    REQUIRE( m::Step::COMBAT_DAMAGE              == m::Step(1 << 7) );
    REQUIRE( m::Step::END_OF_COMBAT              == m::Step(1 << 8) );
    REQUIRE( m::Step::END                        == m::Step(1 << 9) );
    REQUIRE( m::Step::CLEANUP                    == m::Step(1 << 10) );

    
    /*
      500.2. A phase or step in which players receive priority ends when the
      stack is empty and all players pass in succession. Simply having the stack
      become empty doesn't cause such a phase or step to end; all players have
      to pass in succession with the stack empty. Because of this, each player
      gets a chance to add new things to the stack before that phase or step
      ends.

      500.3. A step in which no players receive priority ends when all specified
      actions that take place during that step are completed. The only such
      steps are the untap step (see rule 502) and certain cleanup steps (see
      rule 514).

      500.4. When a step or phase ends, any unused mana left in a player's mana
      pool empties. This turn-based action doesn't use the stack.
    */    
    GIVEN("a game with one active and one inactive player, each with some "
          " mana in their mana pool, and an empty stack") {
        m::GameState game;
        auto active   = m::makePlayer(game.entities, "active");
        auto inactive = m::makePlayer(game.entities, "inactive");
        game.start(active,inactive);
        game.setActivePlayer(active);
        REQUIRE( game.stack().empty() );
        GIVEN("current phase is beginning") {
            game.skipPhase(m::Phase::BEGINNING);
            GIVEN("current step is untap") {
                game.skipStep(m::Step::UNTAP);
                WHEN("all specified action that take place during untap is "
                     "completed") {
                    game.nextStep();
                    THEN("the phase should still be beginning") {
                        CHECK( m::Phase::BEGINNING == game.phase() );
                    }
                    THEN("the upkeep step should start") {
                        CHECK( m::Step::UPKEEP == game.step() );
                    }
                }
                
            }
            GIVEN("current step is upkeep") {
                game.skipStep(m::Step::UPKEEP);
                WHEN("all players pass") {
                    game.pass(active);
                    game.pass(inactive);
                    game.nextStep();
                    THEN("the phase should still be beginning") {
                        CHECK( m::Phase::BEGINNING == game.phase() );
                    }
                    THEN("the draw step should should start") {
                        CHECK( m::Step::DRAW == game.step() );
                    }
                }
            }
            GIVEN("current step is draw") {
                game.skipStep(m::Step::DRAW);
                WHEN("all players pass") {
                    game.pass(active);
                    game.pass(inactive);
                    game.nextStep();
                    THEN("the precombat main phase should start") {
                        CHECK( m::Phase::PRECOMBAT_MAIN == game.phase() );
                    }
                    THEN("the step should be undefined") {
                        CHECK( m::Step::UNDEFINED == game.step() );
                    }
                }
            }
        }
        GIVEN("current phase is precombat main") {
            game.skipPhase(m::Phase::PRECOMBAT_MAIN);
            WHEN("all players pass") {
                game.pass(active);
                game.pass(inactive);
                game.nextStep();
                THEN("the combat phase should start") {
                    CHECK( m::Phase::COMBAT == game.phase() );
                }
                THEN("the beginning of combat step should start") {
                    CHECK( m::Step::BEGINNING_OF_COMBAT == game.step() );
                }
            }
        }
        GIVEN("current phase is combat") {
            game.skipPhase(m::Phase::COMBAT);
            GIVEN("current step is beginning of combat") {
                game.skipStep(m::Step::BEGINNING_OF_COMBAT);
                WHEN("all players pass") {
                    game.pass(active);
                    game.pass(inactive);
                    game.nextStep();
                    THEN("the phase should still be combat") {
                        CHECK( m::Phase::COMBAT == game.phase() );
                    }
                    THEN("the declare attackers step should start") {
                        CHECK( m::Step::DECLARE_ATTACKERS == game.step() );

                    }
                }
            }
            GIVEN("current step is declare attackers") {
                game.skipStep(m::Step::DECLARE_ATTACKERS);
                WHEN("all players pass") {
                    game.pass(active);
                    game.pass(inactive);
                    game.nextStep();
                    THEN("the phase should still be combat") {
                        CHECK( m::Phase::COMBAT == game.phase() );
                    }
                    THEN("the declare blockers step should start") {
                        CHECK( m::Step::DECLARE_BLOCKERS == game.step() );
                    }
                }
            }
            GIVEN("current step is declare blockers") {
                game.skipStep(m::Step::DECLARE_BLOCKERS);
                WHEN("all players pass") {
                    game.pass(active);
                    game.pass(inactive);
                    game.nextStep();
                    THEN("the phase should still be combat") {
                        CHECK( m::Phase::COMBAT == game.phase() );
                    }                    
                    THEN("the combat damage step should start") {
                        CHECK( m::Step::COMBAT_DAMAGE == game.step() );
                    }
                }
            }
            GIVEN("current step is first strike combat damage") {
                game.skipStep(m::Step::FIRST_STRIKE_COMBAT_DAMAGE);
                WHEN("all players pass") {
                    game.pass(active);
                    game.pass(inactive);
                    game.nextStep();
                    THEN("the phase should still be combat") {
                        CHECK( m::Phase::COMBAT == game.phase() );
                    }
                    THEN("the ene combat damage step should start") {
                        CHECK( m::Step::COMBAT_DAMAGE == game.step() );
                    }
                }
            }
            GIVEN("current step is combat damage") {
                game.skipStep(m::Step::COMBAT_DAMAGE);
                WHEN("all players pass") {
                    game.pass(active);
                    game.pass(inactive);
                    game.nextStep();
                    THEN("the phase should still be combat") {
                        CHECK( m::Phase::COMBAT == game.phase() );
                    }                    
                    THEN("the end of combat step should start") {
                        CHECK( m::Step::END_OF_COMBAT == game.step() );
                    }
                }
            }
            GIVEN("current step is end of combat") {
                game.skipStep(m::Step::END_OF_COMBAT);
                WHEN("all players pass") {
                    game.pass(active);
                    game.pass(inactive);
                    game.nextStep();
                    THEN("the postcombat main phase should start") {
                        CHECK( m::Phase::POSTCOMBAT_MAIN == game.phase() );
                    }
                    THEN("the step should be undefined") {
                        CHECK( m::Step::UNDEFINED == game.step() );
                    }
                }
            }
        }
        GIVEN("current phase is postcombat main") {
            game.skipPhase(m::Phase::POSTCOMBAT_MAIN);
            WHEN("all players pass") {
                game.pass(active);
                game.pass(inactive);
                game.nextStep();
                THEN("the ending phase should start") {
                    CHECK( m::Phase::ENDING == game.phase() );
                }
                THEN("the end step should start") {
                    CHECK( m::Step::END == game.step() );
                }
            }
        }
        GIVEN("current phase is ending") {
            game.skipPhase(m::Phase::ENDING);
            GIVEN("current step is end") {
                WHEN("all players pass") {
                    game.pass(active);
                    game.pass(inactive);
                    game.nextStep();
                    THEN("the phase should still be ending") {
                        CHECK( m::Phase::ENDING  == game.phase() );
                    }
                    THEN("the cleanup step should start") {
                        CHECK( m::Step::CLEANUP == game.step() );
                    }
                }
            }
            GIVEN("current step is cleanup") {
            }
        }
    }
    /*
      500.5. When a phase or step ends, any effects scheduled to last "until end
      of" that phase or step expire. When a phase or step begins, any effects
      scheduled to last "until" that phase or step expire. Effects that last
      "until end of combat" expire at the end of the combat phase, not at the
      beginning of the end of combat step. Effects that last "until end of turn"
      are subject to special rules; see rule 514.2.

      500.6. When a phase or step begins, any abilities that trigger "at the
      beginning of" that phase or step trigger. They are put on the stack the
      next time a player would receive priority. (See rule 116, "Timing and
      Priority.")

      500.7. Some effects can give a player extra turns. They do this by adding
      the turns directly after the specified turn. If a player is given multiple
      extra turns, the extra turns are added one at a time. If multiple players
      are given extra turns, the extra turns are added one at a time, in APNAP
      order (see rule 101.4). The most recently created turn will be taken
      first.

      500.8. Some effects can add phases to a turn. They do this by adding the
      phases directly after the specified phase. If multiple extra phases are
      created after the same phase, the most recently created phase will occur
      first.

      500.9. Some effects can add steps to a phase. They do this by adding the
      steps directly after a specified step or directly before a specified
      step. If multiple extra steps are created after the same step, the most
      recently created step will occur first.

      500.10. Some effects can cause a step, phase, or turn to be skipped. To
      skip a step, phase, or turn is to proceed past it as though it didn't
      exist. See rule 614.10.

      500.11. No game events can occur between turns, phases, or steps.
    */
}

SCENARIO("501. Beginning Phase",""){
    /*
      501. Beginning Phase

      501.1. The beginning phase consists of three steps, in this order: untap,
      upkeep, and draw.
    */
}
SCENARIO("502. Untap Step","") {
    /*
      502. Untap Step

      502.1. First, all phased-in permanents with phasing that the
      active player controls phase out, and all phased-out permanents
      that the active player controlled when they phased out phase
      in. This all happens simultaneously. This turn-based action
      doesn't use the stack. See rule 702.25, "Phasing."

      502.2. Second, the active player determines which permanents he
      or she controls will untap. Then he or she untaps them all
      simultaneously. This turn-based action doesn't use the
      stack. Normally, all of a player's permanents untap, but effects
      can keep one or more of a player's permanents from untapping.

      502.3. No player receives priority during the untap step, so no
      spells can be cast or resolve and no abilities can be activated
      or resolve. Any ability that triggers during this step will be
      held until the next time a player would receive priority, which
      is usually during the upkeep step. (See rule 503, "Upkeep
      Step.")
    */
}
SCENARIO("503. Upkeep Step","") {
    /*
      503. Upkeep Step

      503.1. The upkeep step has no turn-based actions. Once it begins, the
      active player gets priority. (See rule 116, "Timing and Priority.")

      503.1a Any abilities that triggered during the untap step and any
      abilities that triggered at the beginning of the upkeep are put onto
      the stack before the active player gets priority; the order in which
      they triggered doesn't matter. (See rule 603, "Handling Triggered
      Abilities.")

      503.2. If a spell states that it may be cast only "after [a player's]
      upkeep step," and the turn has multiple upkeep steps, that spell may
      be cast any time after the first upkeep step ends.
    */
}
SCENARIO("504. Draw Step","") {
    /*
      504. Draw Step

      504.1. First, the active player draws a card. This turn-based
      action doesn't use the stack.

      504.2. Second, the active player gets priority. (See rule 116,
      "Timing and Priority.")
    */
}
SCENARIO("05. Main Phase","") {
    /*
      505.1. There are two main phases in a turn. In each turn, the first main
      phase (also known as the precombat main phase) and the second main phase
      (also known as the postcombat main phase) are separated by the combat
      phase (see rule 506, Combat Phase). The precombat and postcombat main
      phases are individually and collectively known as the main phase.

      505.1a Only the first main phase of the turn is a precombat main
      phase. All other main phases are postcombat main phases. This includes the
      second main phase of a turn in which the combat phase has been skipped. It
      is also true of a turn in which an effect has caused an additional combat
      phase and an additional main phase to be created.

      505.2. The main phase has no steps, so a main phase ends when all players
      pass in succession while the stack is empty. (See rule 500.2.)

      505.3. First, but only if the players are playing an Archenemy game (see
      rule 904), the active player is the archenemy, and it's the active
      player's precombat main phase, the active player sets the top card of his
      or her scheme deck in motion (see rule 701.23). This turn-based action
      doesn't use the stack.

      505.4. Second, the active player gets priority. (See rule 116, Timing and
      Priority.)

      505.4a The main phase is the only phase in which a player can normally
      cast artifact, creature, enchantment, planeswalker, and sorcery
      spells. The active player may cast these spells.

      505.4b During either main phase, the active player may play one land card
      from his or her hand if the stack is empty, if the player has priority,
      and if he or she hasn't played a land this turn (unless an effect states
      the player may play additional lands). This action doesn't use the
      stack. Neither the land nor the action of playing the land is a spell or
      ability, so it can't be countered, and players can't respond to it with
      instants or activated abilities. (See rule 305, Lands.)
    */
}

SCENARIO("506. Combat Step","") {
    /*
      506.1. The combat phase has five steps, which proceed in order:
      beginning of combat, declare attackers, declare blockers, combat
      damage, and end of combat. The declare blockers and combat
      damage steps are skipped if no creatures are declared as
      attackers or put onto the battlefield attacking (see rule
      508.5). There are two combat damage steps if any attacking or
      blocking creature has first strike (see rule 702.7) or double
      strike (see rule 702.4).

      506.2. During the combat phase, the active player is the
      attacking player; creatures that player controls may
      attack. During the combat phase of a two-player game, the
      nonactive player is the defending player; that player and
      planeswalkers he or she controls may be attacked.

      506.2a During the combat phase of a multiplayer game, there may
      be one or more defending players, depending on the variant being
      played and the options chosen for it. Unless all the attacking
      player's opponents automatically become defending players during
      the combat phase, the attacking player chooses one of his or her
      opponents as a turn-based action during the beginning of combat
      step. (Note that the choice may be dictated by the variant being
      played or the options chosen for it.) That player becomes the
      defending player. See rule 802, "Attack Multiple Players
      Option," rule 803, "Attack Left and Attack Right Options," and
      rule 809, "Emperor Variant."

      506.2b In the Two-Headed Giant multiplayer variant, the
      nonactive team is the defending team. See rule 810, "Two-Headed
      Giant Variant."

      506.3. Only a creature can attack or block. Only a player or a
      planeswalker can be attacked.

      506.3a If an effect would put a noncreature permanent onto the
      battlefield attacking or blocking, the permanent does enter the
      battlefield but it's never considered to be an attacking or
      blocking permanent.

      506.3b If an effect would put a creature onto the battlefield
      attacking under the control of any player except an attacking
      player, that creature does enter the battlefield, but it's never
      considered to be an attacking creature.

      506.3c If an effect would put a creature onto the battlefield
      attacking either a player not in the game or a planeswalker no
      longer on the battlefield or no longer a planeswalker, that
      creature does enter the battlefield, but it's never considered
      to be an attacking creature.

      506.3d If an effect would put a creature onto the battlefield
      blocking but the creature it would block isn't attacking either
      the first creature's controller or a planeswalker that player
      controls, that creature does enter the battlefield, but it's
      never considered to be a blocking creature.

      506.4. A permanent is removed from combat if it leaves the
      battlefield, if its controller changes, if it phases out, if an
      effect specifically removes it from combat, if it's a
      planeswalker that's being attacked and stops being a
      planeswalker, or if it's an attacking or blocking creature that
      regenerates (see rule 701.13) or stops being a creature. A
      creature that's removed from combat stops being an attacking,
      blocking, blocked, and/or unblocked creature. A planeswalker
      that's removed from combat stops being attacked.

      506.4a Once a creature has been declared as an attacking or
      blocking creature, spells or abilities that would have kept that
      creature from attacking or blocking don't remove the creature
      from combat.

      506.4b Tapping or untapping a creature that's already been
      declared as an attacker or blocker doesn't remove it from combat
      and doesn't prevent its combat damage.

      506.4c If a creature is attacking a planeswalker, removing that
      planeswalker from combat doesn't remove that creature from
      combat. It continues to be an attacking creature, although it is
      attacking neither a player nor a planeswalker. It may be
      blocked. If it is unblocked, it will deal no combat damage.

      506.4d A permanent that's both a blocking creature and a
      planeswalker that's being attacked is removed from combat if it
      stops being both a creature and a planeswalker. If it stops
      being one of those card types but continues to be the other, it
      continues to be either a blocking creature or a planeswalker
      that's being attacked, whichever is appropriate.

      506.5. A creature attacks alone if it's the only creature
      declared as an attacker during the declare attackers step. A
      creature is attacking alone if it's attacking but no other
      creatures are. A creature blocks alone if it's the only creature
      declared as a blocker during the declare blockers step. A
      creature is blocking alone if it's blocking but no other
      creatures are.

      506.6. Some spells state that they may be cast "only
      [before/after] [a particular point in the combat phase]," in
      which that point may be "attackers are declared," "blockers are
      declared," "the combat damage step," "the end of combat step,"
      "the combat phase," or "combat."

      506.6a A spell that states it may be cast "only before (or
      after) attackers are declared" is referring to the turn-based
      action of declaring attackers. It may be cast only before (or
      after) the declare attackers step begins, regardless of whether
      any attackers are actually declared. (See rule 508.)

      506.6b A spell that states it may be cast "only before (or
      after) blockers are declared" is referring to the turn-based
      action of declaring blockers. It may be cast only before (or
      after) the declare blockers step begins, regardless of whether
      any blockers are actually declared. (See rule 509.)

      506.6c Some spells state that they may be cast only "during
      combat" or "during a certain player's combat phase" in addition
      to the criteria described in rule 506.6. If a turn has multiple
      combat phases, such spells may be cast at an appropriate time
      during any of them.

      506.6d Some spells state that they may be cast "only before (or
      after) [a particular point in the combat phase]," but don't meet
      the additional criteria described in rule 506.6c. If a turn has
      multiple combat phases, such spells may be cast that turn only
      before (or after) the stated point of the first combat phase.

      506.6e If a spell states that it may be cast "only before [a
      particular point in the combat phase]," but the stated point
      doesn't exist within the relevant combat phase because the
      declare blockers step and the combat damage step are skipped
      (see rule 508.5), then the spell may be cast only before the
      declare attackers step ends. If the stated point doesn't exist
      because the relevant combat phase has been skipped, then the
      spell may be cast only before the precombat main phase ends.

      506.6f If a spell states that it may be cast "only during combat
      after blockers are declared," but the declare blockers step is
      skipped that combat phase (see rule 508.5), then the spell may
      not be cast during that combat phase.

      506.6g Rules 506.6 and 506.6a�f apply to abilities that state
      that they may be activated only at certain times with respect to
      combat just as they apply to spells that state that they may be
      cast only at certain times with respect to combat.
    */
}

SCENARIO("507. Beginning of Combat Step","") {
    /*
      507.1. First, if the game being played is a multiplayer game in
      which the active player's opponents don't all automatically
      become defending players, the active player chooses one of his
      or her opponents. That player becomes the defending player. This
      turn-based action doesn't use the stack. (See rule 506.2.)

      507.2. Second, the active player gets priority. (See rule 116,
      "Timing and Priority.")
    */
}

SCENARIO("508. Declare Attackers Step","") {
    /*
      508.1. First, the active player declares attackers. This
      turn-based action doesn't use the stack. To declare attackers,
      the active player follows the steps below, in order. If at any
      point during the declaration of attackers, the active player is
      unable to comply with any of the steps listed below, the
      declaration is illegal; the game returns to the moment before
      the declaration (see rule 720, "Handling Illegal Actions").

      508.1a The active player chooses which creatures that he or she
      controls, if any, will attack. The chosen creatures must be
      untapped, and each one must either have haste or have been
      controlled by the active player continuously since the turn
      began.

      508.1b If the defending player controls any planeswalkers, or
      the game allows the active player to attack multiple other
      players, the active player announces which player or
      planeswalker each of the chosen creatures is attacking.

      508.1c The active player checks each creature he or she controls
      to see whether it's affected by any restrictions (effects that
      say a creature can't attack, or that it can't attack unless some
      condition is met). If any restrictions are being disobeyed, the
      declaration of attackers is illegal.  Example: A player controls
      two creatures, each with a restriction that states "[This
      creature] can't attack alone."  It's legal to declare both as
      attackers.

      508.1d The active player checks each creature he or she controls
      to see whether it's affected by any requirements (effects that
      say a creature must attack, or that it must attack if some
      condition is met). If the number of requirements that are being
      obeyed is fewer than the maximum possible number of requirements
      that could be obeyed without disobeying any restrictions, the
      declaration of attackers is illegal. If a creature can't attack
      unless a player pays a cost, that player is not required to pay
      that cost, even if attacking with that creature would increase
      the number of requirements being obeyed.  Example: A player
      controls two creatures: one that "attacks if able" and one with
      no abilities. An effect states "No more than one creature can
      attack each turn." The only legal attack is for just the
      creature that "attacks if able" to attack. It's illegal to
      attack with the other creature, attack with both, or attack with
      neither.

      508.1e If any of the chosen creatures have banding or a "bands
      with other" ability, the active player announces which
      creatures, if any, are banded with which. (See rule 702.21,
      "Banding.")

      508.1f The active player taps the chosen creatures. Tapping a
      creature when it's declared as an attacker isn't a cost;
      attacking simply causes creatures to become tapped.

      508.1g If any of the chosen creatures require paying costs to
      attack, the active player determines the total cost to
      attack. Costs may include paying mana, tapping permanents,
      sacrificing permanents, discarding cards, and so on. Once the
      total cost is determined, it becomes "locked in." If effects
      would change the total cost after this time, ignore this change.

      508.1h If any of the costs require mana, the active player then
      has a chance to activate mana abilities (see rule 605, "Mana
      Abilities").

      508.1i Once the player has enough mana in his or her mana pool,
      he or she pays all costs in any order. Partial payments are not
      allowed.

      508.1j Each chosen creature still controlled by the active
      player becomes an attacking creature. It remains an attacking
      creature until it's removed from combat or the combat phase
      ends, whichever comes first. See rule 506.4.

      508.1k Any abilities that trigger on attackers being declared
      trigger.

      508.2. Second, the active player gets priority. (See rule 116,
      "Timing and Priority.")

      508.2a Abilities that trigger on a creature attacking trigger
      only at the point the creature is declared as an attacker. They
      will not trigger if a creature attacks and then that creature's
      characteristics change to match the ability's trigger condition.
      Example: A permanent has the ability "Whenever a green creature
      attacks, destroy that creature at end of combat." If a blue
      creature attacks and is later turned green, the ability will not
      trigger.

      508.2b Any abilities that triggered on attackers being declared
      or that triggered during the process described in rules 508.1
      are put onto the stack before the active player gets priority;
      the order in which they triggered doesn't matter. (See rule 603,
      "Handling Triggered Abilities.")

      508.3. If a creature is put onto the battlefield attacking, its
      controller chooses which defending player or which planeswalker
      a defending player controls it's attacking as it enters the
      battlefield (unless the effect that put it onto the battlefield
      specifies what it's attacking). Such creatures are "attacking"
      but, for the purposes of trigger events and effects, they never
      "attacked."

      508.3a If the effect that puts a creature onto the battlefield
      attacking specifies it's attacking a certain player, and that
      player is no longer in the game when the effect resolves, the
      creature is put onto the battlefield but is never considered an
      attacking creature. The same is true if the effect specifies a
      creature is put onto the battlefield attacking a planeswalker
      and that planeswalker is no longer on the battlefield or is no
      longer a planeswalker when the effect resolves.

      508.3b A creature that's put onto the battlefield attacking
      isn't affected by requirements or restrictions that apply to the
      declaration of attackers.

      508.4. If an ability of an attacking creature refers to a
      defending player, or a spell or ability refers to both an
      attacking creature and a defending player, then unless otherwise
      specified, the defending player it's referring to is the player
      that creature was attacking at the time it became an attacking
      creature that combat, or the controller of the planeswalker that
      creature was attacking at the time it became an attacking
      creature that combat.

      508.4a In a multiplayer game, any rule, object, or effect that
      refers to a "defending player" refers to one specific defending
      player, not to all of the defending players. If a spell or
      ability could apply to multiple attacking creatures, the
      appropriate defending player is individually determined for each
      of those attacking creatures. If there are multiple defending
      players that could be chosen, the controller of the spell or
      ability chooses one.

      508.5. If no creatures are declared as attackers or put onto the
      battlefield attacking, skip the declare blockers and combat
      damage steps.
    */
}

SCENARIO("509. Declare Blockers Step","") {
    /*
      509.1. First, the defending player declares blockers. This
      turn-based action doesn't use the stack. To declare blockers,
      the defending player follows the steps below, in order. If at
      any point during the declaration of blockers, the defending
      player is unable to comply with any of the steps listed below,
      the declaration is illegal; the game returns to the moment
      before the declaration (see rule 720, "Handling Illegal
      Actions").

      509.1a The defending player chooses which creatures that he or
      she controls, if any, will block. The chosen creatures must be
      untapped. For each of the chosen creatures, the defending player
      chooses one creature for it to block that's attacking him, her,
      or a planeswalker he or she controls.

      509.1b The defending player checks each creature he or she
      controls to see whether it's affected by any restrictions
      (effects that say a creature can't block, or that it can't block
      unless some condition is met). If any restrictions are being
      disobeyed, the declaration of blockers is illegal.  A
      restriction may be created by an evasion ability (a static
      ability an attacking creature has that restricts what can block
      it). If an attacking creature gains or loses an evasion ability
      after a legal block has been declared, it doesn't affect that
      block. Different evasion abilities are cumulative.  Example: An
      attacking creature with flying and shadow can't be blocked by a
      creature with flying but without shadow.

      509.1c The defending player checks each creature he or she
      controls to see whether it's affected by any requirements
      (effects that say a creature must block, or that it must block
      if some condition is met). If the number of requirements that
      are being obeyed is fewer than the maximum possible number of
      requirements that could be obeyed without disobeying any
      restrictions, the declaration of blockers is illegal. If a
      creature can't block unless a player pays a cost, that player is
      not required to pay that cost, even if blocking with that
      creature would increase the number of requirements being obeyed.
      Example: A player controls one creature that "blocks if able"
      and another creature with no abilities. If a creature with
      menace attacks that player, the player must block with both
      creatures. Having only the first creature block violates the
      restriction created by menace (the attacking creature can't be
      blocked except by two or more creatures). Having only the second
      creature block violates both the menace restriction and the
      first creature's blocking requirement. Having neither creature
      block fulfills the restriction but not the requirement.

      509.1d If any of the chosen creatures require paying costs to
      block, the defending player determines the total cost to
      block. Costs may include paying mana, tapping permanents,
      sacrificing permanents, discarding cards, and so on. Once the
      total cost is determined, it becomes "locked in." If effects
      would change the total cost after this time, ignore this change.

      509.1e If any of the costs require mana, the defending player
      then has a chance to activate mana abilities (see rule 605,
      "Mana Abilities").

      509.1f Once the player has enough mana in his or her mana pool,
      he or she pays all costs in any order. Partial payments are not
      allowed.

      509.1g Each chosen creature still controlled by the defending
      player becomes a blocking creature. Each one is blocking the
      attacking creatures chosen for it. It remains a blocking
      creature until it's removed from combat or the combat phase
      ends, whichever comes first. See rule 506.4.

      509.1h An attacking creature with one or more creatures declared
      as blockers for it becomes a blocked creature; one with no
      creatures declared as blockers for it becomes an unblocked
      creature. This remains unchanged until the creature is removed
      from combat, an effect says that it becomes blocked or
      unblocked, or the combat phase ends, whichever comes first. A
      creature remains blocked even if all the creatures blocking it
      are removed from combat.

      509.1i Any abilities that trigger on blockers being declared
      trigger. See rule 509.4 for more information.

      509.2. Second, for each attacking creature that's become
      blocked, the active player announces that creature's damage
      assignment order, which consists of the creatures blocking it in
      an order of that player's choice. (During the combat damage
      step, an attacking creature can't assign combat damage to a
      creature that's blocking it unless each creature ahead of that
      blocking creature in its order is assigned lethal damage.) This
      turn-based action doesn't use the stack.  Example: Vastwood
      Gorger is blocked by Llanowar Elves, Runeclaw Bear, and Serra
      Angel. Vastwood Gorger's controller announces the Vastwood
      Gorger's damage assignment order as Serra Angel, then Llanowar
      Elves, then Runeclaw Bear.

      509.2a During the declare blockers step, if a blocking creature
      is removed from combat or a spell or ability causes it to stop
      blocking an attacking creature, the blocking creature is removed
      from all relevant damage assignment orders. The relative order
      among the remaining blocking creatures is unchanged.

      509.3. Third, for each blocking creature, the defending player
      announces that creature's damage assignment order, which
      consists of the creatures it's blocking in an order of that
      player's choice. (During the combat damage step, a blocking
      creature can't assign combat damage to a creature it's blocking
      unless each creature ahead of that blocked creature in its order
      is assigned lethal damage.) This turn-based action doesn't use
      the stack.

      509.3a During the declare blockers step, if an attacking
      creature is removed from combat or a spell or ability causes it
      to stop being blocked by a blocking creature, the attacking
      creature is removed from all relevant damage assignment
      orders. The relative order among the remaining attacking
      creatures is unchanged.

      509.4. Fourth, the active player gets priority. (See rule 116,
      "Timing and Priority.")

      509.4a Any abilities that triggered on blockers being declared
      or that triggered during the process described in rules
      509.13 are put onto the stack before the active player gets
      priority; the order in which they triggered doesn't matter. (See
      rule 603, "Handling Triggered Abilities.")

      509.5. Triggered abilities that trigger during the declare
      blockers step may have different trigger conditions.

      509.5a An ability that reads "Whenever [this creature] blocks,
      . . ." generally triggers only once each combat for that
      creature, even if it blocks multiple creatures. It triggers if
      the creature is declared as a blocker. It will also trigger if
      that creature becomes a blocker as the result of an effect, but
      only if it wasn't a blocking creature at that time. (See rule
      509.1g.) It won't trigger if the creature is put onto the
      battlefield blocking.

      509.5b An ability that reads "Whenever [this creature] blocks a
      creature, . . ." triggers once for each attacking creature the
      creature with the ability blocks. It triggers if the creature is
      declared as a blocker. It will also trigger if an effect causes
      that creature to block an attacking creature, but only if it
      wasn't already blocking that attacking creature at that time. It
      won't trigger if the creature is put onto the battlefield
      blocking.

      509.5c An ability that reads "Whenever [this creature] becomes
      blocked, . . ." generally triggers only once each combat for
      that creature, even if it's blocked by multiple creatures. It
      will trigger if that creature becomes blocked by at least one
      creature declared as a blocker. It will also trigger if that
      creature becomes blocked by an effect or by a creature that's
      put onto the battlefield as a blocker, but only if the attacking
      creature was an unblocked creature at that time. (See rule
      509.1h.)

      509.5d An ability that reads "Whenever [this creature] becomes
      blocked by a creature, . . ." triggers once for each creature
      that blocks the named creature. It triggers if a creature is
      declared as a blocker for the attacking creature. It will also
      trigger if an effect causes a creature to block the attacking
      creature, but only if it wasn't already blocking that attacking
      creature at that time. In addition, it will trigger if a
      creature is put onto the battlefield blocking that creature. It
      won't trigger if the creature becomes blocked by an effect
      rather than a creature.

      509.5e If an ability triggers when a creature blocks or becomes
      blocked by a particular number of creatures, the ability
      triggers if the creature blocks or is blocked by that many
      creatures when blockers are declared. Effects that add or remove
      blockers can also cause such abilities to trigger. This applies
      to abilities that trigger on a creature blocking or being
      blocked by at least a certain number of creatures as well.

      509.5f If an ability triggers when a creature with certain
      characteristics blocks, it will trigger only if the creature has
      those characteristics at the point blockers are declared, or at
      the point an effect causes it to block. If an ability triggers
      when a creature with certain characteristics becomes blocked, it
      will trigger only if the creature has those characteristics at
      the point it becomes a blocked creature. If an ability triggers
      when a creature becomes blocked by a creature with certain
      characteristics, it will trigger only if the latter creature has
      those characteristics at the point it becomes a blocking
      creature. None of those abilities will trigger if the relevant
      creature's characteristics change to match the ability's trigger
      condition later on.  Example: A creature has the ability
      "Whenever this creature becomes blocked by a white creature,
      destroy that creature at end of combat." If the creature becomes
      blocked by a black creature that is later turned white, the
      ability will not trigger.

      509.5g An ability that reads "Whenever [this creature] attacks
      and isn't blocked, . . ." triggers if no creatures are declared
      as blockers for that creature. It will trigger even if the
      creature was never declared as an attacker (for example, if it
      entered the battlefield attacking). It won't trigger if the
      attacking creature is blocked and then all its blockers are
      removed from combat.

      509.6. If a spell or ability causes a creature on the
      battlefield to block an attacking creature, the active player
      announces the blocking creature's placement in the attacking
      creature's damage assignment order. The relative order among the
      remaining blocking creatures is unchanged. Then the defending
      player announces the attacking creature's placement in the
      blocking creature's damage assignment order. The relative order
      among the remaining attacking creatures is unchanged. This is
      done as part of the blocking effect.

      509.7. If a creature is put onto the battlefield blocking, its
      controller chooses which attacking creature it's blocking as it
      enters the battlefield (unless the effect that put it onto the
      battlefield specifies what it's blocking), then the active
      player announces the new creature's placement in the blocked
      creature's damage assignment order. The relative order among the
      remaining blocking creatures is unchanged. A creature put onto
      the battlefield this way is "blocking" but, for the purposes of
      trigger events and effects, it never "blocked."  Example: Giant
      Spider is blocked by Canyon Minotaur. The defending player casts
      Flash Foliage, which creates a Saproling creature token blocking
      the Giant Spider. Giant Spider's controller announces the Giant
      Spider's damage assignment order as the Saproling token, then
      Canyon Minotaur.

      509.7a If the effect that puts a creature onto the battlefield
      blocking specifies it's blocking a certain creature and that
      creature is no longer attacking, the creature is put onto the
      battlefield but is never considered a blocking creature. The
      same is true if the controller of the creature that's put onto
      the battlefield blocking isn't a defending player for the
      specified attacking creature.

      509.7b A creature that's put onto the battlefield blocking isn't
      affected by requirements or restrictions that apply to the
      declaration of blockers.
    */
}

SCENARIO("510. Combat Damage Step","") {
    /*
      510.1. First, the active player announces how each attacking
      creature assigns its combat damage, then the defending player
      announces how each blocking creature assigns its combat
      damage. This turn-based action doesn't use the stack. A player
      assigns a creature's combat damage according to the following
      rules:

      510.1a Each attacking creature and each blocking creature
      assigns combat damage equal to its power. Creatures that would
      assign 0 or less damage this way don't assign combat damage at
      all.

      510.1b An unblocked creature assigns its combat damage to the
      player or planeswalker it's attacking. If it isn't currently
      attacking anything (if, for example, it was attacking a
      planeswalker that has left the battlefield), it assigns no
      combat damage.

      510.1c A blocked creature assigns its combat damage to the
      creatures blocking it. If no creatures are currently blocking it
      (if, for example, they were destroyed or removed from combat),
      it assigns no combat damage. If exactly one creature is blocking
      it, it assigns all its combat damage to that creature. If two or
      more creatures are blocking it, it assigns its combat damage to
      those creatures according to the damage assignment order
      announced for it. This may allow the blocked creature to divide
      its combat damage. However, it can't assign combat damage to a
      creature that's blocking it unless, when combat damage
      assignments are complete, each creature that precedes that
      blocking creature in its order is assigned lethal damage. When
      checking for assigned lethal damage, take into account damage
      already marked on the creature and damage from other creatures
      that's being assigned during the same combat damage step, but
      not any abilities or effects that might change the amount of
      damage that's actually dealt. An amount of damage that's greater
      than a creature's lethal damage may be assigned to it.  Example:
      The damage assignment order of an attacking Vastwood Gorger (a
      5/6 creature) is Pride Guardian (a 0/3 creature) then Llanowar
      Elves (a 1/1 creature). Vastwood Gorger can assign 3 damage to
      the Guardian and 2 damage to the Elves, 4 damage to the Guardian
      and 1 damage to the Elves, or 5 damage to the Guardian.
      Example: The damage assignment order of an attacking Vastwood
      Gorger (a 5/6 creature) is Pride Guardian (a 0/3 creature) then
      Llanowar Elves (a 1/1 creature). During the declare blockers
      step, the defending player casts Giant Growth targeting Pride
      Guardian, which gives it +3/+3 until end of turn. Vastwood
      Gorger must assign its 5 damage to the Guardian.  Example: The
      damage assignment order of an attacking Vastwood Gorger (a 5/6
      creature) is Pride Guardian (a 0/3 creature) then Llanowar Elves
      (a 1/1 creature). During the declare blockers step, the
      defending player casts Mending Hands targeting Pride Guardian,
      which prevents the next 4 damage that would be dealt to
      it. Vastwood Gorger can assign 3 damage to the Guardian and 2
      damage to the Elves, 4 damage to the Guardian and 1 damage to
      the Elves, or 5 damage to the Guardian.  Example: The damage
      assignment order of an attacking Enormous Baloth (a 7/7
      creature) is Trained Armodon (a 3/3 creature) that already has 2
      damage marked on it, then Foriysian Brigade (a 2/4 creature that
      can block an additional creature), then Silverback Ape (a 5/5
      creature). The damage assignment order of an attacking Durkwood
      Boars (a 4/4 creature) is the same Foriysian Brigade, then
      Goblin Piker (a 2/1 creature). Among other possibilities, the
      active player may have the Baloth assign 1 damage to the
      Armodon, 1 damage to the Brigade, and 5 damage to the Ape, and
      have the Boars assign 3 damage to the Brigade and 1 damage to
      the Piker.

      510.1d A blocking creature assigns combat damage to the
      creatures it's blocking. If it isn't currently blocking any
      creatures (if, for example, they were destroyed or removed from
      combat), it assigns no combat damage. If it's blocking exactly
      one creature, it assigns all its combat damage to that
      creature. If it's blocking two or more creatures, it assigns its
      combat damage to those creatures according to the damage
      assignment order announced for it. This may allow the blocking
      creature to divide its combat damage. However, it can't assign
      combat damage to a creature that it's blocking unless, when
      combat damage assignments are complete, each creature that
      precedes that blocked creature is assigned lethal damage. When
      checking for assigned lethal damage, take into account damage
      already marked on the creature and damage from other creatures
      that's being assigned during the same combat damage step, but
      not any abilities or effects that might change the amount of
      damage that's actually dealt. An amount of damage that's greater
      than a creature's lethal damage may be assigned to it.

      510.1e Once a player has assigned combat damage from each
      attacking or blocking creature he or she controls, the total
      damage assignment (not solely the damage assignment of any
      individual attacking or blocking creature) is checked to see if
      it complies with the above rules. If it doesn't, the combat
      damage assignment is illegal; the game returns to the moment
      before that player began to assign combat damage. (See rule 720,
      "Handling Illegal Actions").

      510.2. Second, all combat damage that's been assigned is dealt
      simultaneously. This turn-based action doesn't use the stack. No
      player has the chance to cast spells or activate abilities
      between the time combat damage is assigned and the time it's
      dealt.  Example: Squadron Hawk (a 1/1 creature with flying) and
      Goblin Piker (a 2/1 creature) are attacking. Mogg Fanatic (a 1/1
      creature with the ability "Sacrifice Mogg Fanatic: Mogg Fanatic
      deals 1 damage to target creature or player") blocks the Goblin
      Piker. The defending player sacrifices Mogg Fanatic during the
      declare blockers step to deal 1 damage to the Squadron Hawk. The
      Hawk is destroyed. The Piker deals and is dealt no combat damage
      this turn. If the defending player instead left Mogg Fanatic on
      the battlefield, the Fanatic and the Piker would have dealt
      lethal damage to one another, but the Squadron Hawk couldn't
      have been dealt damage.

      510.3. Third, the active player gets priority. (See rule 116,
      "Timing and Priority.")

      510.3a Any abilities that triggered on damage being dealt or
      while state-based actions are performed afterward are put onto
      the stack before the active player gets priority; the order in
      which they triggered doesn't matter. (See rule 603, "Handling
      Triggered Abilities.")

      510.4. If at least one attacking or blocking creature has first
      strike (see rule 702.7) or double strike (see rule 702.4) as the
      combat damage step begins, the only creatures that assign combat
      damage in that step are those with first strike or double
      strike. After that step, instead of proceeding to the end of
      combat step, the phase gets a second combat damage step. The
      only creatures that assign combat damage in that step are the
      remaining attackers and blockers that had neither first strike
      nor double strike as the first combat damage step began, as well
      as the remaining attackers and blockers that currently have
      double strike. After that step, the phase proceeds to the end of
      combat step.
    */
}

SCENARIO("511. End of Combat Step","") {
    /*
      511.1. The end of combat step has no turn-based actions. Once it
      begins, the active player gets priority. (See rule 116, "Timing
      and Priority.")

      511.2. Abilities that trigger "at end of combat" trigger as the
      end of combat step begins. Effects that last "until end of
      combat" expire at the end of the combat phase.

      511.3. As soon as the end of combat step ends, all creatures and
      planeswalkers are removed from combat. After the end of combat
      step ends, the combat phase is over and the postcombat main
      phase begins (see rule 505).
    */
}

SCENARIO("512. Ending Phase","") {
    /*
      512.1. The ending phase consists of two steps: end and cleanup.
    */
}

SCENARIO("513. End Step","") {
    /*
      513.1. The end step has no turn-based actions. Once it begins,
      the active player gets priority. (See rule 116, "Timing and
      Priority.")

      513.1a Previously, abilities that triggered at the beginning of
      the end step were printed with the trigger condition "at end of
      turn." Cards that were printed with that text have received
      errata in the Oracle card reference to say "at the beginning of
      the end step" or "at the beginning of the next end step."

      513.2. If a permanent with an ability that triggers "at the
      beginning of the end step" enters the battlefield during this
      step, that ability won't trigger until the next turn's end
      step. Likewise, if a delayed triggered ability that triggers "at
      the beginning of the next end step" is created during this step,
      that ability won't trigger until the next turn's end step. In
      other words, the step doesn't "back up" so those abilities can
      go on the stack. This rule applies only to triggered abilities;
      it doesn't apply to continuous effects whose durations say
      "until end of turn" or "this turn." (See rule 514, "Cleanup
      Step.")
    */
}

SCENARIO("514. Cleanup Step","") {
    /*
      514.1. First, if the active player's hand contains more cards
      than his or her maximum hand size (normally seven), he or she
      discards enough cards to reduce his or her hand size to that
      number. This turn-based action doesn't use the stack.

      514.2. Second, the following actions happen simultaneously: all
      damage marked on permanents (including phased-out permanents) is
      removed and all "until end of turn" and "this turn" effects
      end. This turn-based action doesn't use the stack.

      514.3. Normally, no player receives priority during the cleanup
      step, so no spells can be cast and no abilities can be
      activated. However, this rule is subject to the following
      exception:

      514.3a At this point, the game checks to see if any state-based
      actions would be performed and/or any triggered abilities are
      waiting to be put onto the stack (including those that trigger
      "at the beginning of the next cleanup step"). If so, those
      state-based actions are performed, then those triggered
      abilities are put on the stack, then the active player gets
      priority. Players may cast spells and activate abilities. Once
      the stack is empty and all players pass in succession, another
      cleanup step begins.
*/
}

