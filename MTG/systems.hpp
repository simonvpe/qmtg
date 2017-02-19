#pragma once
#include <entityx/entityx.h>
#include <game.hpp>
#include <player.hpp>

namespace MTG {
using namespace entityx;

class PregameSubsystem {
public:
    PregameSubsystem(EntityManager &mgr)
    : m_entities{mgr}
    {}

    void update(GameHandle game) {
        if(!game.isStarted()) {
            auto start = true;
            m_entities.each<Player>([&](PlayerHandle player, auto&) {
                start = start && player.isReady();
            });
            game.setStarted(start);
            m_entities.each<Player>([&](PlayerHandle player, auto&) {
                player.setReady(false);
            });
        }
    }
    
private:
    EntityManager& m_entities;
};
    
class GameSystem : public System<GameSystem> {

    void update(EntityManager &es, EventManager &events, TimeDelta dt) override {
        PregameSubsystem pregame{es};

        
        // When all players are ready, start the game
        es.each<Game>([&](GameHandle game, auto&) {
                pregame.update(game);
            // if(!game.isStarted()) {
            //     auto start = true;
            //     es.each<Player>([&](PlayerHandle player, auto&) {
            //         start = start && player.isReady();
            //     });
            //     game.setStarted(start);
            //     es.each<Player>([&](PlayerHandle player, auto&) {
            //         player.setReady(false);
            //     });
            // }
        });
        
    };
    
};
    
}
              
