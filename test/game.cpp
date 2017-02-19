#include <catch.hpp>
#include <game.hpp>
#include <context.hpp>

SCENARIO("GameHandle", "[Game][GameHandle][unit]") {
    using Context = MTG::Context;

    GIVEN("a GameHandle created from a Context") {
        Context ctx;
        auto game = ctx.makeGame();

        WHEN("setting started to true") {
            game.setStarted(true);
            THEN("isStarted() should return true") {
                CHECK( game.isStarted() );
            }
        }

        WHEN("setting started to false") {
            game.setStarted(false);
            THEN("isStarted() should return false") {
                CHECK( !game.isStarted() );
            }
        }

        WHEN("getting started for the first time") {
            auto started = game.isStarted();
            THEN("isStarted() should return false") {
                CHECK( !started );
            }
        }

        WHEN("calling setStarted() without argument") {
            game.setStarted();
            THEN("the argument should default to true") {
                CHECK( game.isStarted() );
            }
        }
    }
}
