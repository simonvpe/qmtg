#pragma once
#include <entityx/entityx.h>
#include <bitset>

namespace MTG {
using namespace entityx;
using namespace std;
    
class Game {
    friend class GameHandle;
private:
    enum Flags { STARTED = 0 };
    enum Phase { PREGAME };
    bitset<1> flags;
    Phase     phase;
};

class GameHandle : public Entity {
    friend class Context;
private:
    auto game() { return component<Game>(); }
    auto game() const { return component<const Game>(); }

public:
    GameHandle(Entity e = {})
    : Entity{e}
    {}

    inline void setStarted(bool value = true) {
        game()->flags[Game::STARTED] = value;
    }
    
    inline bool isStarted() {
        return game()->flags[Game::STARTED];
    }

    inline void setPregamePhase() {
        game()->phase = Game::Phase::PREGAME;
    }
    
    inline bool isInPregamePhase() const {
        return game()->phase == Game::Phase::PREGAME;
    }
};

}
