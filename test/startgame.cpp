#include <catch.hpp>
#include <context.hpp>

////////////////////////////////////////////////////////////////////////////////
namespace m = MTG;
namespace p = MTG::player;
namespace c = MTG::card;

bool operator==(const m::CardHandle& lhs, const std::string& rhs) {
    return lhs->name == rhs;
}
////////////////////////////////////////////////////////////////////////////////

SCENARIO("103. Starting the Game","[103.1][103.2][103.3]") {
    GIVEN("a newly started match of two players, each with a deck of cards") {
        m::Context ctx;
        auto gameHandle = ctx.makeGame();
        auto player1 = ctx.makePlayer(gameHandle);
        auto player2 = ctx.makePlayer(gameHandle);
        auto game = m::GameQuery{gameHandle};
        auto deck = std::vector<const char*>(60, "dummy");
        
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
                p::setReady(player);
            }
            ctx.advance();
            THEN("the game should start") {
                CHECK( true == game.isStarted() );
            }
        }
        WHEN("the game have just started (all players connected and ready)") {
            for(auto player : { player1, player2 }) {
                ctx.connect(player);
                ctx.setDeck(player,deck);
                p::setReady(player);
            }
            game.setStarted();
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
                    CHECK( 7 == player->startingHandSize );
                    CHECK( 7 == hand.size() );
                }
            }
            THEN("each player begins the game with a starting life total "
                     "of 20.") {
                for(auto player : { player1, player2 }) {
                    CHECK( 20 == player->life );
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
                p::setReady(player);
            }
            //ctx.setMulligan(player1, true);
            //ctx.setMulligan(player2, false);
            THEN("players should be dealt one less card than his or her "
                 "current hand size") {
                //CHECK( 6 == ctx.hand(player1).size() );
            }
        }
    }
}
