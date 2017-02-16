#pragma once
#include <entityx/entityx.h>
#include <bitset>

namespace MTG {
using namespace entityx;
using namespace std;
    
struct Game {
    enum Flags { STARTED = 0 };
    bitset<1> flags;
};

struct GameHandle : public Entity {
    GameHandle(Entity e = {}) : Entity{e} {}
    auto operator->() { return component<Game>(); }
    const auto operator->() const { return component<const Game>(); }
};

namespace game {
    bool isStarted(const GameHandle game) {
        return game->flags[Game::STARTED];
    }
    void setStarted(GameHandle game, bool value = true) {
        game->flags[Game::STARTED] = value;
    }
}
    
}
