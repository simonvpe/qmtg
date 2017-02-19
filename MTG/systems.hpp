#pragma once
#include <entityx/entityx.h>
#include <game.hpp>
#include <player.hpp>
#include <query.hpp>

namespace MTG {
using namespace entityx;

inline void moveHandToLibrary(PlayerQuery query) {
    query.eachCardInHand([](CardHandle card) {
        card.moveToLibrary();
    });
}

inline void draw(PlayerQuery query, int numberOfCards) {
    int k = 0;
    query.eachCardInLibrary([&](CardHandle card) {
        if(k++ < numberOfCards) {
            card.moveToHand();
        }
    });
}
    
class PregameSubsystem {
public:
    PregameSubsystem(EntityManager &mgr)
    : m_entities{mgr}
    {}

    void update(GameHandle game) {
        GameQuery query{m_entities, game};
        
        if(!game.isStarted()) {
            auto startGame = allPlayersReady(query);
            if(startGame) {
                resetPlayersReady(query);
                game.setStarted();
            }
        }
        
        else if(game.isInPregamePhase()) {
            query.eachPlayer([&](PlayerHandle player) {
                if(player.wantsMulligan()) {
                    PlayerQuery pQuery{m_entities, player};
                    moveHandToLibrary(pQuery);
                    draw(pQuery, player.getStartingHandSize());
                    player.setStartingHandSize(player.getStartingHandSize() - 1);
                    player.setMulligan(false);
                }
            });
        }
    }

protected:
    bool allPlayersReady(GameQuery query) const {
        auto allReady = true;
        query.eachPlayer([&](PlayerHandle player) {
            allReady = allReady && player.isReady();
        });
        return allReady;
    }

    void resetPlayersReady(GameQuery query) const {
        query.eachPlayer([&](PlayerHandle player) {
            player.setReady(false);
        });
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
              
