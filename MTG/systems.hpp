#pragma once
#include <entityx/entityx.h>
#include <query.hpp>
#include <game.hpp>
#include <player.hpp>

namespace MTG {
using namespace entityx;

class PregameSystem : public System<PregameSystem> {

    void update(EntityManager &es, EventManager &events, TimeDelta dt) override {
        
        // When all players are ready, start the game
        es.each<Game>([&](GameHandle game, auto&) {
            if(!GameQuery{game}.isStarted()) {
                auto start = true;
                es.each<Player>([&](PlayerHandle player, auto&) {
                    start = start && player.isReady();
                });
                GameQuery{game}.setStarted(start);
            }
        });
        
    };
};
    
}
              
