#include <catch.hpp>
#include <player.hpp>
#include <context.hpp>

SCENARIO("PlayerHandle","[Player][PlayerHandle][unit]") {
    using Context = MTG::Context;
    
    GIVEN("a PlayerHandle created from a Context") {
        Context ctx;
        auto game = ctx.makeGame();
        auto player = ctx.makePlayer(game);

        WHEN("setting active to true") {
            player.setActive(true);
            THEN("isActive() should return true") {
                CHECK( player.isActive() );
            }
        }

        WHEN("setting active to false") {
            player.setActive(false);
            THEN("isActive() should return false") {
                CHECK( !player.isActive() );
            }
                                                    
        }

        WHEN("setting active without argument") {
            player.setActive();
            THEN("isActive() should return true") {
                CHECK( player.isActive() );
            }
        }

        WHEN("getting active for the first time") {
            auto active = player.isActive();
            THEN("isActive() should return false") {
                CHECK( !player.isActive() );
            }
        }

        WHEN("setting ready to true") {
            player.setReady(true);
            THEN("isReady() should return true") {
                CHECK( player.isReady() );
            }
        }

        WHEN("setting ready to false") {
            player.setReady(false);
            THEN("isReady() should return false") {
                CHECK( !player.isReady() );
            }
        }

        WHEN("setting ready without argument") {
            player.setReady();
            THEN("isReady() should return true") {
                CHECK( player.isReady() );
            }
        }

        WHEN("getting ready for the first time") {
            auto ready = player.isReady();
            THEN("false should be returned") {
                CHECK( !ready );
            }
        }        


        WHEN("setting starting hand size") {
            player.setStartingHandSize(99);
            THEN("getStartingHandSize() should return that value") {
                CHECK( 99 == player.getStartingHandSize() );
            }
        }

        WHEN("setting life") {
            player.setLife(88);
            THEN("getLife() should return that value") {
                CHECK( 88 == player.getLife() );
            }
        }

        WHEN("setting game") {
            auto game2 = ctx.makeGame();
            player.setGame(game2);
            THEN("getGame() should return that game") {
                CHECK( game2 == player.getGame() );
            }
        }

        WHEN("getting game the first time") {
            auto g = player.getGame();
            THEN("the game passed to makePlayer() should be returned") {
                CHECK( g == game );
            }
        }
                                             
                             
    }
}
