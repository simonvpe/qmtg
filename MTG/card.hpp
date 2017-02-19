#pragma once
#include <entityx/entityx.h>
#include <cstring>
#include <player.hpp>

namespace MTG {
using namespace entityx;
    
class Card {
    friend class CardHandle;
private:
    enum      { MAX_NAME_LENGTH = 20};
    enum Zone { UNDEFINED, LIBRARY, HAND };
    char         name[MAX_NAME_LENGTH];
    Zone         zone;
    PlayerHandle player;
};

class CardHandle : public Entity {
    friend class Context;
private:
    auto card() { return component<Card>(); }
    auto card() const { return component<const Card>(); }
    
public:
    CardHandle(Entity e = {})
    : Entity{e}
    {}
    
    bool operator==(const char *rhs) const
    { return 0 == std::strcmp(rhs,card()->name); }

    void setName(const char *name) {
        strncpy(card()->name, name, Card::MAX_NAME_LENGTH);
    }

    const char *getName() const {
        return card()->name;
    }
    
    void setPlayer(PlayerHandle player) {
        card()->player = player;
    }

    PlayerHandle getPlayer() const {
        return card()->player;
    }

    void moveToHand() {
        card()->zone = Card::HAND;
    }

    void moveToLibrary() {
        card()->zone = Card::LIBRARY;
    }
    
    bool isInHand() {
        return card()->zone == Card::HAND;
    }

    bool isInLibrary() const {
        return card()->zone == Card::LIBRARY;
    }

    bool zoneIsUndefined() const {
        return card()->zone == Card::UNDEFINED;
    }
    
};
    
using CardVector = const std::vector<CardHandle>;
using MutableCardVector = std::vector<CardHandle>;
    
}
