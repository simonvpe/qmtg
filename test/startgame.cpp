#include <catch.hpp>
#include <context.hpp>

////////////////////////////////////////////////////////////////////////////////
namespace m = MTG;

bool operator==(const m::CardHandle& lhs, const std::string& rhs) {
    return lhs.getName() == rhs;
}
////////////////////////////////////////////////////////////////////////////////

SCENARIO("103. Starting the Game","[103.1][103.2][103.3]") {
    GIVEN("a newly started match of two players, each with a deck of cards") {
        m::Context ctx;
        auto game    = ctx.makeGame();
        auto player1 = ctx.makePlayer(game);
        auto player2 = ctx.makePlayer(game);
        auto deck    = std::vector<const char*>(60, "dummy");

        WHEN("not all players are connected") {
            ctx.connect(player1);
            ctx.advance();
            THEN("the game should not to start") {
                CHECK( false == game.isStarted() );
            }
        }
        WHEN("all players are connected but not ready") {
            for(auto player : { player1, player2 }) {
                ctx.connect(player);
            }
            ctx.advance();
            THEN("the game should not start") {
                CHECK( false == game.isStarted() );
            }
        }
        WHEN("all players are connected and ready") {
            for(auto player : { player1, player2 }) {
                ctx.connect(player);
                player.setReady();
            }
            THEN("the game should start") {
                ctx.advance();
                CHECK( game.isStarted() );
                AND_THEN("the reset field of the players should reset to false") {
                    CHECK( !player1.isReady() );
                    CHECK( !player2.isReady() );
                }
            }
            WHEN("there are several games running") {
                auto game2 = ctx.makeGame();
                auto game3 = ctx.makeGame();
                AND_WHEN("the other games are about to start") {
                    for(auto g : { game2, game3 }) {
                        for(auto p : { ctx.makePlayer(g), ctx.makePlayer(g) }) {
                            ctx.connect(p);
                            p.setReady();
                        }
                    }

                    ctx.advance();
                    THEN("the other games should also be able to start") {
                        CHECK( game.isStarted() );
                        CHECK( game2.isStarted() );
                        CHECK( game3.isStarted() );
                    }
                }
                AND_WHEN("the other games are not about to start") {
                    for(auto g : { game2, game3 }) {
                        ctx.makePlayer(g);
                        ctx.makePlayer(g);
                    }
                    ctx.advance();
                    THEN("the other games should not start") {
                        CHECK( game.isStarted() );
                        CHECK( !game2.isStarted() );
                        CHECK( !game3.isStarted() );
                    }
                }
            }
        }
        WHEN("the game have just started (all players connected and ready)") {
            for(auto player : { player1, player2 }) {
                ctx.connect(player);
                ctx.setDeck(player,deck);
                player.setReady();
            }
            ctx.advance();
            
            THEN("each players decks have been shuffled and become "
                 "their libraries") {
                for(auto player : { player1, player2 }) {
                    auto lib   = ctx.getLibrary(player);
                    CHECK( 53 == lib.size() );
                }
            }
            THEN("each player has a starting hand size of 7 if no effects "
                     "have modified their staring hand size") {
                for(auto player : { player1, player2 }) {
                    auto hand = ctx.getHand(player);
                    CHECK( 7 == player.getStartingHandSize() );
                    CHECK( 7 == hand.size() );
                }
            }
            THEN("each player begins the game with a starting life total "
                     "of 20.") {
                for(auto player : { player1, player2 }) {
                    CHECK( 20 == player.getLife() );
                }
            }
        }
        WHEN("setting a players deck several times") {
            auto deck1 = std::vector<const char*>(60,"one");
            auto deck2 = std::vector<const char*>(60,"two");
            for(auto deck : {deck1, deck2}) {
                ctx.setDeck(player1,deck);
            }
            THEN("the old cards should have been destroyed") {
                CHECK( 53 == ctx.getLibrary(player1).size() );
                CHECK( 7 ==  ctx.getHand(player1).size() );
                CHECK( *ctx.getLibrary(player1).begin() == *deck2.begin() );
                CHECK( *ctx.getHand(player1).begin() == *deck2.begin() );
            }
        }
        WHEN("a player chooses to mulligan") {
            for(auto player : {player1,player2}) {
                ctx.connect(player);
                ctx.setDeck(player, deck);
                player.setReady();
            }
            ctx.advance();
            //ctx.setMulligan(player1, true);
            //ctx.setMulligan(player2, false);
            THEN("players should be dealt one less card than his or her "
                 "current hand size") {
                //CHECK( 6 == ctx.hand(player1).size() );
            }
        }
    }
}
