#pragma once
#include <entityx/entityx.h>
#include <game.hpp>
#include <player.hpp>

namespace MTG {
using namespace entityx;

class GameQuery {
public:
    GameQuery(EntityManager& mgr, GameHandle game)
    : entities{mgr}
    , game{game}
    {}

    template<typename TFun>
    void eachPlayer(TFun&& fun) {
        entities.each<Player>([&](PlayerHandle player, auto&){
            if(game == player.getGame()) fun(player);
        });
    }
    
private:
    EntityManager& entities;
    GameHandle     game;
};
    
class PregameSubsystem {
public:
    PregameSubsystem(EntityManager &mgr)
    : m_entities{mgr}
    {}

    void update(GameHandle game) {
        if(!game.isStarted()) {
            GameQuery query{m_entities, game};
            
            auto start = true;
            query.eachPlayer([&](PlayerHandle player) {
                start = start && player.isReady();
            });
            game.setStarted(start);
            query.eachPlayer([&](PlayerHandle player) {
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
        
        es.each<Game>([&](GameHandle game, auto&) {
                pregame.update(game);
        });
        
    };
    
};
    
}
              
