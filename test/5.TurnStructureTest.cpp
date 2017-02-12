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
    */
    GIVEN("a game with one active and one inactive player and an empty stack") {
        m::GameState game;
        auto active   = m::makePlayer(game.entities, "active");
        auto inactive = m::makePlayer(game.entities, "inactive");
        game.start(active,inactive);
        game.setActivePlayer(active);
        REQUIRE( game.stack().empty() );
        
        GIVEN("current phase is beginning") {
            game.skipPhase(m::Phase::BEGINNING);

            GIVEN("current step is upkeep") {
                game.skipStep(m::Step::UPKEEP);

                WHEN("all players pass") {
                    game.pass(active);
                    game.pass(inactive);

                    THEN("the draw step should should start") {
                        CHECK( m::Phase::BEGINNING == game.phase() );
                        CHECK( m::Step::DRAW       == game.step() );
                    }
                }
            }
            GIVEN("current step is draw") {
                game.skipStep(m::Step::DRAW);

                WHEN("all players pass") {
                    game.pass(active);
                    game.pass(inactive);

                    THEN("the precombat main phase should start") {
                        CHECK( m::Phase::PRECOMBAT_MAIN == game.phase() );
                        CHECK( m::Step::UNDEFINED       == game.step() );

                    }
                }
            }
        }
        GIVEN("current phase is precombat main") {
            game.skipPhase(m::Phase::PRECOMBAT_MAIN);
            
            WHEN("all players pass") {
                game.pass(active);
                game.pass(inactive);
                
                THEN("the beginning of combat step should start") {
                    CHECK( m::Phase::COMBAT             == game.phase() );
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
                    
                    THEN("the declare attackers step should start") {
                        CHECK( m::Phase::COMBAT           == game.phase() );
                        CHECK( m::Step::DECLARE_ATTACKERS == game.step() );

                    }
                }
            }
            GIVEN("current step is declare attackers") {
                game.skipStep(m::Step::DECLARE_ATTACKERS);
            
                WHEN("all players pass") {
                    game.pass(active);
                    game.pass(inactive);
                    
                    THEN("the declare blockers step should start") {
                        CHECK( m::Phase::COMBAT          == game.phase() );
                        CHECK( m::Step::DECLARE_BLOCKERS == game.step() );
                    }
                }
            }
            GIVEN("current step is declare blockers") {
                game.skipStep(m::Step::DECLARE_BLOCKERS);
            
                WHEN("all players pass") {
                    game.pass(active);
                    game.pass(inactive);
                    
                    THEN("the combat damage step should start") {
                        CHECK( m::Phase::COMBAT       == game.phase() );
                        CHECK( m::Step::COMBAT_DAMAGE == game.step() );
                    }
                }
            }
            GIVEN("current step is combat damage") {
                game.skipStep(m::Step::COMBAT_DAMAGE);
            
                WHEN("all players pass") {
                    game.pass(active);
                    game.pass(inactive);
                    
                    THEN("the end of combat step should start") {
                        CHECK( m::Phase::COMBAT       == game.phase() );
                        CHECK( m::Step::END_OF_COMBAT == game.step() );
                    }
                }
            }
            GIVEN("current step is end of combat") {
                game.skipStep(m::Step::END_OF_COMBAT);

                WHEN("all players pass") {
                    game.pass(active);
                    game.pass(inactive);

                    THEN("the postcombat main phase should start") {
                        CHECK( m::Phase::POSTCOMBAT_MAIN == game.phase() );
                        CHECK( m::Step::UNDEFINED        == game.step() );
                    }
                }
            }
        }
        GIVEN("current phase is postcombat main") {
            game.skipPhase(m::Phase::POSTCOMBAT_MAIN);

            WHEN("all players pass") {
                game.pass(active);
                game.pass(inactive);

                THEN("the end step should start") {
                    CHECK( m::Phase::ENDING == game.phase() );
                    CHECK( m::Step::END     == game.step() );
                }
            }
        }
        GIVEN("current phase is ending") {
            game.skipPhase(m::Phase::ENDING);

            WHEN("all players pass") {
                game.pass(active);
                game.pass(inactive);

                THEN("the cleanup step should start") {
                    CHECK( m::Phase::ENDING  == game.phase() );
                    CHECK( m::Step::CLEANUP  == game.step() );
                }
            }
        }
    }
    /*
      500.3. A step in which no players receive priority ends when all specified
      actions that take place during that step are completed. The only such
      steps are the untap step (see rule 502) and certain cleanup steps (see
      rule 514).

      500.4. When a step or phase ends, any unused mana left in a player's mana
      pool empties. This turn-based action doesn't use the stack.

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

SCENARIO("501. Beginning Phase","[501]"){
    /*
      501. Beginning Phase

      501.1. The beginning phase consists of three steps, in this order: untap,
      upkeep, and draw.
    */
    GIVEN("502. Untap Step") {
        m::GameState game;
        /*
          502. Untap Step

          502.1. First, all phased-in permanents with phasing that the active
          player controls phase out, and all phased-out permanents that the
          active player controlled when they phased out phase in. This all
          happens simultaneously. This turn-based action doesn't use the
          stack. See rule 702.25, "Phasing."

          502.2. Second, the active player determines which permanents he or she
          controls will untap. Then he or she untaps them all
          simultaneously. This turn-based action doesn't use the
          stack. Normally, all of a player's permanents untap, but effects can
          keep one or more of a player's permanents from untapping.

          502.3. No player receives priority during the untap step, so no spells
          can be cast or resolve and no abilities can be activated or
          resolve. Any ability that triggers during this step will be held until
          the next time a player would receive priority, which is usually during
          the upkeep step. (See rule 503, "Upkeep Step.")
        */
        WHEN("active player have tapped permanents") {
            AND_WHEN("there are NO effects preventing permanents from "
                     "untapping normally") {
                THEN("active player untaps all his or her tapped permanents") {
                }
            }
            AND_WHEN("there are effects preventings permanents from "
                     "untapping normally") {
                THEN("active player untaps only his or her permanents that "
                     "are not prevented from untapping") {
                }
            }
            THEN("active player untaps all permanents he or she controls") {
            }
        }
        WHEN("active player have NO tapped permanents") {
        }
        WHEN("active player tries to activate an ability") {
            THEN("the action has no effect") {
            }
        }
        WHEN("active player tries to cast a spell") {
            THEN("the action has no effect") {
            }
        }
        WHEN("an ability triggers") {
            THEN("it is deferred (to the upkeep phase)") {
            }
        }
        WHEN("there are phased-in permanents") {
            THEN("phase out the phased-in permanents") {
            }
        }
        WHEN("there are phased-out permanents") {
            THEN("phase in the phased-out permanents") {
            }
        }
    }

    GIVEN("a game in the upkeep phase") {
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
        WHEN("there are deferred abilities from the untap phase") {
            THEN("the deferred abilities go to the stack") {
                THEN("the active player gets priority") {
                }
            }
        }
        WHEN("there are no deferred abilities from the UNTAP phase") {
            THEN("the active player gets priority") {
            }
        }
    }
    GIVEN("a game that transitioned into the draw step") {
        /*
          504. Draw Step

          504.1. First, the active player draws a card. This turn-based action
          doesn't use the stack.

          504.2. Second, the active player gets priority. (See rule 116, Timing
          and Priority.)
        */
        WHEN("the game starts") {
            THEN("the active player draws a card from his or her library") {
                WHEN("there are abilities that trigger at the beginnig of "
                     "the [DRAW] step") {
                    THEN("the abilities go to the stack") {
                        THEN("the active player gets priority") {
                        }
                    }
                }
            }
        }
    }
}

SCENARIO("505. Main Phase",""){
    /*
      505. Main Phase

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

////////////////////////////////////////////////////////////////////////////////
SCENARIO("Beginning of combat step","[turn]"){
    /*
      All abilities that trigger at the beginning of combat.
     */
}

////////////////////////////////////////////////////////////////////////////////
SCENARIO("Declare attackers step","[turn]"){
    /*
     The active player declares attackers. This is a turn-based action
     and does not use the stack.

     Next all abilities that trigger on attackers being declared go
     on the stack, then the active player gets priority.
    */
}

////////////////////////////////////////////////////////////////////////////////
SCENARIO("Declare blockers step","[turn]"){
    /*
      The defending player declares blockers. This is a turn-based action
      and does not use the stack.

      Next all abilities that trigger on blockers being declared go on the
      stack, then the active player gets priority.
     */
}

////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
SCENARIO("End of combat step","[turn]"){
    /*
      All abilities that trigger at the beginning of the end step go on
      the stack, then the active player gets priority.
     */
}

////////////////////////////////////////////////////////////////////////////////
SCENARIO("End step","[turn]"){
    /*
      All abilities that trigger at the beginning of the end step go on
      the stack, then the active player gets priority.
     */
}

////////////////////////////////////////////////////////////////////////////////
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
