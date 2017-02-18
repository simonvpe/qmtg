#include <catch.hpp>
#include <player.hpp>
#include <context.hpp>

SCENARIO("CardHandle","[Card][CardHandle][unit]") {
    using Context = MTG::Context;

    GIVEN("a CardHandle created from a context") {
        Context ctx;
        auto game = ctx.makeGame();
        auto player = ctx.makePlayer(game);
        auto card = ctx.makeCard(player, "card");
    }
}
