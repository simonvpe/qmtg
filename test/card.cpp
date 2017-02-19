#include <catch.hpp>
#include <player.hpp>
#include <context.hpp>

SCENARIO("CardHandle","[Card][CardHandle][unit]") {
    using Context = MTG::Context;

    GIVEN("a CardHandle created from a game and a context") {
        Context ctx;
        auto game   = ctx.makeGame();
        auto player = ctx.makePlayer(game);
        auto card   = ctx.makeCard(player, "card");

        WHEN("moving the card to the hand") {
            card.moveToLibrary(); // Reset position
            card.moveToHand();
            THEN("isInHand() should return true") {
                CHECK( card.isInHand() );
            }
        }

        WHEN("moving the card to the library") {
            card.moveToHand(); // Reset position
            card.moveToLibrary();
            THEN("isInLibrary() should return true") {
                CHECK( card.isInLibrary() );
            }
        }

        WHEN("initially checking the position of the card") {
            auto isUndefined = card.zoneIsUndefined();
            THEN("it should be undefined") {
                CHECK( isUndefined );
            }
        }

        WHEN("setting player") {
            auto p = ctx.makePlayer(game);
            card.setPlayer(p);
            THEN("getPlayer() should return that player") {
                CHECK( p == card.getPlayer() );
            }
        }

        WHEN("getting player for the first time") {
            auto p = card.getPlayer();
            THEN("the player passed to makeCard() should be returned") {
                CHECK( p == player );
            }
        }
    }
}
