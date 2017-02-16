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
            if(playerHandle->game == game) func(playerHandle);
        });
    }
private:
    GameHandle game;
};

class CardQuery {
public:
    CardQuery(CardHandle handle) : card{handle} {}
    
    void setZone(Card::Zone zone) {
        card->zone = zone;
    }

    Card::Zone getZone() const {
        return card->zone;
    }

    bool isInHand() {
        return card->zone == Card::HAND;
    }

    bool isInLibrary() const {
        return card->zone == Card::LIBRARY;
    }
private:
    CardHandle card;
};

class PlayerQuery {
public:
    PlayerQuery(PlayerHandle handle) : player{handle} {}
    
    inline bool isActive() const {
        return player->flags[Player::ACTIVE];
    }

    inline void setActive(bool value = true) {
        player->flags[Player::ACTIVE] = value;
    }

    inline bool isReady() const {
        return player->flags[Player::READY];
    }
        
    inline void setReady(bool value = true) {
        player->flags[Player::READY] = value;
    }
private:
    PlayerHandle player;
};
    
}
