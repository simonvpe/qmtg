#pragma once
#include <entityx/entityx.h>
#include <game.hpp>

namespace MTG {
using namespace entityx;

struct Socket {
};

class Player {
private:
    enum Flags { ACTIVE = 0, READY = 1, MULLIGAN = 2 };
    GameHandle game;
    Entity     other;
    bitset<2>  flags;
    int        life;
    int        startingHandSize;
    friend class PlayerHandle;
};

class PlayerHandle : public Entity {
    friend class Context;
private:
    auto player() { return component<Player>(); }
    auto player() const { return component<const Player>(); }
    
public:
    PlayerHandle(Entity e = {})
    : Entity{e}
    {}    
    
    inline bool isActive() const {
        return player()->flags[Player::ACTIVE];
    }

    inline int getStartingHandSize() const {
        return player()->startingHandSize;
    }

    inline int getLife() const {
        return player()->life;
    }

    inline auto getGame() const {
        return player()->game;
    }

    inline void setActive(bool value = true) {
        player()->flags[Player::ACTIVE] = value;
    }

    inline bool isReady() const {
        return player()->flags[Player::READY];
    }
        
    inline void setReady(bool value = true) {
        player()->flags[Player::READY] = value;
    }

    inline void setGame(GameHandle value) {
        player()->game = value;
    }

    inline void setLife(int value) {
        player()->life = value;
    }

    inline void setStartingHandSize(int value) {
        player()->startingHandSize = value;
    }
    
};
using PlayerVector = const std::vector<PlayerHandle>;
using MutablePlayerVector = std::vector<PlayerHandle>;

}
