#include "catch.hpp"
#include "common.hpp"
#include <random>
#include <set>

////////////////////////////////////////////////////////////////////////////////
namespace m = MTG;
bool operator==(m::CardHandle& lhs, const std::string& rhs) {
    return lhs->name == rhs;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
template<typename T>
auto cardNames(T library) {
    std::vector<std::string> lib;
    std::transform(
            library.begin(),
            library.end(),
            std::back_inserter(lib),
            [](auto& card) { return card->name; });
    return lib;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
template<typename TLeft, typename TRight>
auto isPermutation(TLeft left, TRight right) {
    auto match = std::is_permutation(
            left.begin(), left.end(), right.begin());
    return (left.size() > 0) && (right.size() > 0) && match;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
template<typename TLeft, typename TRight>
auto isEqual(TLeft left, TRight right) {
    return std::equal(left.begin(), left.end(), right.begin());
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
class FakeRandomGenerator : public m::RandomGenerator {
public:
    ////////////////////////////////////////////////////////////////////////////
    FakeRandomGenerator(bool reverse)
    : m::RandomGenerator()
    , m_reverse{reverse}
    {}
    ////////////////////////////////////////////////////////////////////////////
    
    ////////////////////////////////////////////////////////////////////////////
    virtual void
    shuffle(
    gsl::span<m::PlayerHandle> container) override {
        if(m_reverse) std::reverse(container.begin(), container.end());
    }
    ////////////////////////////////////////////////////////////////////////////
private:
    int m_reverse;
};

SCENARIO("103. Starting the Game","[103.1][103.2][103.3]") {
    GIVEN("a newly started match of two players, each with a deck of cards") {
        m::Context ctx;
        m::PlayerHandle player1, player2;
        std::tie(player1,player2) = ctx.startGame();
        auto deck = std::vector<const char*>(60, "dummy");
        
        WHEN("the game have just started (all players connected and ready)") {
            for(auto player : { player1, player2 }) {
                ctx.connect(player);
                ctx.setDeck(player,deck);
                ctx.setReady(player);
            }
            THEN("each players decks have been shuffled and become "
                 "their libraries") {
                for(auto player : { player1, player2 }) {
                    auto lib   = ctx.library(player);
                    CHECK( 53 == lib.size() );
                }
            }
            THEN("each player has a starting hand size of 7 if no effects "
                     "have modified their staring hand size") {
                for(auto player : { player1, player2 }) {
                    auto hand = ctx.hand(player);
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
                CHECK( 53 == ctx.library(player1).size() );
                CHECK( 7 ==  ctx.hand(player1).size() );
                CHECK( *ctx.library(player1).begin() == *deck2.begin() );
                CHECK( *ctx.hand(player1).begin() == *deck2.begin() );
            }
        }
        WHEN("a player chooses to mulligan") {
            for(auto player : {player1,player2}) {
                ctx.connect(player);
                ctx.setDeck(player, deck);
                ctx.setReady(player);
            }
            ctx.setMlligan(player1, true);
            ctx.setMulligan(player2, false);
            THEN("players should be dealt one less card than his or her "
                 "current hand size") {
                CHECK( 6 == ctx.hand(player1).size() );
            }
        }
    }
}
