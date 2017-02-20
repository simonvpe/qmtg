#include <catch.hpp>
#include <context.hpp>

////////////////////////////////////////////////////////////////////////////////
namespace m = MTG;

bool operator==(const m::CardHandle& lhs, const std::string& rhs) {
    return lhs.getName() == rhs;
}
////////////////////////////////////////////////////////////////////////////////

SCENARIO("103. Starting the Game","[103.1][103.2][103.3][103.4]") {
    GIVEN("a game with two not-yet-ready players, and a list of dummy cards") {
        m::Context ctx;
        auto game    = ctx.makeGame();
        auto player1 = ctx.makePlayer(game);
        auto player2 = ctx.makePlayer(game);
        auto deck    = std::vector<const char*>(60, "dummy");

        WHEN("all players are not ready") {
            player1.setReady();
            ctx.advance();
            THEN("the game should not start") {
                CHECK( false == game.isStarted() );
            }
        }
        WHEN("all players are ready") {
            for(auto player : { player1, player2 }) {
                player.setReady();
            }
            THEN("the game should start") {
                ctx.advance();
                CHECK( game.isStarted() );
                AND_THEN("the ready field of the players should reset to false") {
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
                            p.setReady();
                        }
                    }
                    ctx.advance();
                    THEN("all the games should be started") {
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
        WHEN("the game have just started") {
            for(auto player : { player1, player2 }) {
                ctx.createDeck(player,deck);
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
            THEN("each player has a starting hand of 7 cards if no effects "
                     "have modified their staring hand size") {
                for(auto player : { player1, player2 }) {
                    auto hand = ctx.getHand(player);
                    CHECK( 7 == hand.size() );
                }
                AND_THEN("the players starting hand sizes should have decreased "
                         "to 6") {
                    for(auto player : {player1, player2 }) {
                        CHECK( 6 == player.getStartingHandSize() );
                    }
                    
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
            ctx.createDeck(player1, deck1);
            THEN("should fail with an exception") {
                auto deck2 = std::vector<const char*>(60,"two");
                CHECK_THROWS( ctx.createDeck(player1, deck2) );
            }
        }
        WHEN("the game has started") {
            for(auto player : {player1,player2}) {
                ctx.createDeck(player, deck);
                player.setReady();
            }
            ctx.advance();
            AND_WHEN("a player is marked for mulligan and both players are marked ready") {
                player1.setMulligan();
                for(auto player : {player1,player2}) player.setReady();
                ctx.advance();
                THEN("the mulligan player should be dealt one less card than his or her "
                     "current hand size") {
                    CHECK( 6 == ctx.getHand(player1).size() );
                    AND_THEN("the mulligan player should not be marked for mulligan anymore") {
                        CHECK( !player1.wantsMulligan() );
                        AND_THEN("none of the players should be marked ready") {
                            CHECK( !player1.isReady() );
                            CHECK( !player2.isReady() );
                        }
                    }
                }
            }
            AND_WHEN("both players are marked for mulligan and both players are marked ready") {
                for(auto player : {player1,player2}) {
                    player.setReady();
                    player.setMulligan();
                }
                ctx.advance();
                THEN("both players should be dealt one less card than his or her current hand size") {
                    for(auto player : {player1,player2})
                        CHECK( 6 == ctx.getHand(player).size() );
                    AND_THEN("both players should not be marked for mulligan anymore") {
                        for(auto player : {player1,player2})
                            CHECK( !player.wantsMulligan() );
                        AND_THEN("none of the players should be marked ready") {
                            for(auto player : {player1,player2})
                                CHECK( !player.isReady() );
                        }
                    }
                }
            }
            
        }
    }
}
