#pragma once
#include <entityx/entityx.h>
#include <game.hpp>

namespace MTG {
using namespace entityx;

struct Socket {
};

class Player {
    enum Flags { ACTIVE = 0, READY = 1, MULLIGAN = 2 };
    GameHandle game;
    Entity     other;
    bitset<2>  flags;
    int        life;
    int        startingHandSize;
    friend class PlayerQuery;
};

struct PlayerHandle : public Entity {
    PlayerHandle(Entity e = {})
    : Entity{e}
    {}
    auto operator->() { return component<Player>(); }
    const auto operator->() const { return component<const Player>(); }
};
using PlayerVector = const std::vector<PlayerHandle>;
using MutablePlayerVector = std::vector<PlayerHandle>;

}
