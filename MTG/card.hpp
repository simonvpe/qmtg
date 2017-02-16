#pragma once
#include <entityx/entityx.h>
#include <cstring>
#include <player.hpp>

namespace MTG {
using namespace entityx;
    
struct Card {
    enum { MAX_NAME_LENGTH = 20};
    enum Zone { LIBRARY, HAND };
    char         name[MAX_NAME_LENGTH];
    Zone         zone;
    PlayerHandle player;
};

struct CardHandle : public Entity {
    CardHandle(Entity e = {})
    : Entity{e}
    {}
    auto operator->() { return component<Card>(); }
    const auto operator->() const { return component<const Card>(); } 
        
    bool operator==(const char *rhs) const
    { return 0 == std::strcmp(rhs,(*this)->name); }
};

using CardVector = const std::vector<CardHandle>;
using MutableCardVector = std::vector<CardHandle>;
    
namespace card {
    inline void setZone(CardHandle card, Card::Zone zone) {
        card->zone = zone;
    }

    inline Card::Zone getZone(const CardHandle& card) {
        return card->zone;
    }

    inline bool isInHand(const CardHandle &card) {
        return card->zone == Card::HAND;
    }

    inline bool isInLibrary(const CardHandle &card) {
        return card->zone == Card::LIBRARY;
    }
}
    
}
