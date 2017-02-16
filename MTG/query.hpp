#pragma once
#include <player.hpp>
#include <game.hpp>

namespace MTG {

namespace game {    
    bool isStarted(const GameHandle game) {
        return game->flags[Game::STARTED];
    }
    void setStarted(GameHandle game, bool value = true) {
        game->flags[Game::STARTED] = value;
    }

    template<typename TFunc>
    void eachPlayer(EntityManager& entities, GameHandle game, TFunc&& func) {
        entities.each<Player>([&](PlayerHandle playerHandle, auto&) {
            if(playerHandle->game == game) func(playerHandle);
        });
    }
}

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

namespace player {
    inline bool isActive(const PlayerHandle& player) {
        return player->flags[Player::ACTIVE];
    }

    inline void setActive(PlayerHandle& player, bool value = true) {
        player->flags[Player::ACTIVE] = value;
    }

    inline bool isReady(const PlayerHandle& player) {
        return player->flags[Player::READY];
    }
        
    inline void setReady(PlayerHandle& player, bool value = true) {
        player->flags[Player::READY] = value;
    }
}
    
}
