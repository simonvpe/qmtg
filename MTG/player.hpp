#pragma once
#include <entityx/entityx.h>
#include <game.hpp>

namespace MTG {
using namespace entityx;

class Player {
    friend class PlayerHandle;
private:
    enum Flags { ACTIVE = 0, READY = 1, MULLIGAN = 2, CAN_MULLIGAN = 3 };
    GameHandle game;
    Entity     other;
    bitset<8>  flags;
    int        life;
    int        startingHandSize;
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
    
    inline void setActive(bool value = true) {
        player()->flags[Player::ACTIVE] = value;
    }

    inline bool isActive() const {
        return player()->flags[Player::ACTIVE];
    }

    inline void setStartingHandSize(int value) {
        player()->startingHandSize = value;
    }

    inline int getStartingHandSize() const {
        return player()->startingHandSize;
    }

    inline void setLife(int value) {
        player()->life = value;
    }

    inline int getLife() const {
        return player()->life;
    }

    inline void setGame(GameHandle value) {
        player()->game = value;
    }

    inline auto getGame() const {
        return player()->game;
    }

    inline void setMulligan(bool value = true) {
        player()->flags[Player::MULLIGAN] = value;
    }

    inline void setCanMulligan(bool value = true) {
        player()->flags[Player::CAN_MULLIGAN] = value;
    }
    
    inline bool getCanMulligan() const {
        return player()->flags[Player::CAN_MULLIGAN];
    }
    
    inline auto getMulligan() const {
        return player()->flags[Player::MULLIGAN];
    }

    inline void setReady(bool value = true) {
        player()->flags[Player::READY] = value;
    }
    
    inline bool isReady() const {
        return player()->flags[Player::READY];
    }

};
using PlayerVector = const std::vector<PlayerHandle>;
using MutablePlayerVector = std::vector<PlayerHandle>;

}
