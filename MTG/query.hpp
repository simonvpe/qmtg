#pragma once
#include <player.hpp>
#include <game.hpp>
#include <card.hpp>

namespace MTG {

class GameQuery {
public:
    GameQuery(GameHandle handle) : game{handle} {}
        
    bool isStarted() {
        return game->flags[Game::STARTED];
    }
    void setStarted(bool value = true) {
        game->flags[Game::STARTED] = value;
    }

    template<typename TFunc>
    void eachPlayer(EntityManager& entities, TFunc&& func) {
        entities.each<Player>([&](PlayerHandle playerHandle, auto&) {
            if(playerHandle.getGame() == game) func(playerHandle);
        });
    }
private:
    GameHandle game;
};

}
