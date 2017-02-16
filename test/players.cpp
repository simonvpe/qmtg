#include "catch.hpp"
#include "common.hpp"

namespace m = MTG;
SCENARIO("102. Players","[102.2]") {
    /*
      102.1. A player is one of the people in the game. The active
      player is the player whose turn it is. The other players are
      nonactive players.

      102.2. In a two-player game, a player's opponent is the other
      player.
    */
    GIVEN("a newly started game of two players") {
        m::Context ctx;
        m::PlayerHandle p1, p2;
        std::tie(p1,p2) = ctx.startGame();
        WHEN("the game have just started (all players connected)") {
            ctx.connect(p1);
            ctx.connect(p2);
            THEN("player ones opponent is player two") {
                CHECK( p1->other == p2 );
            }
            THEN("player twos opponent is player one") {
                CHECK( p2->other == p1 );
            }
        }
    }
}
