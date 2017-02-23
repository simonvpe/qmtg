#pragma once
#include <entityx/entityx.h>
#include <game.hpp>
#include <player.hpp>
#include <query.hpp>

namespace MTG {
using namespace entityx;

inline void moveHandToLibrary(PlayerQuery pQuery) {
    pQuery.eachCardInHand([](CardHandle card) {
        card.moveToLibrary();
    });
}

inline void draw(PlayerQuery pQuery, int numberOfCards) {
    int k = 0;
    pQuery.eachCardInLibrary([&](CardHandle card) {
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
        GameQuery gQuery{m_entities, game};
        
        if(!game.isStarted()) {
            auto startGame = allPlayersReady(gQuery);
            if(startGame) {
                resetPlayersReady(gQuery);
                game.setStarted();
                gQuery.eachPlayer([](PlayerHandle player) {
                    player.setCanMulligan();
                });
            }
        }
        
        else if(game.isInPregamePhase()) {
            gQuery.eachPlayer([&](PlayerHandle player) {
                if(player.getMulligan()) {
                    if(!player.getCanMulligan())
                        throw std::logic_error("The player cannot mulligan");
                    mulligan(player);
                }
            });
        }
    }

protected:
    void mulligan(PlayerHandle player) {
        PlayerQuery pQuery{m_entities, player};
        moveHandToLibrary(pQuery);
        draw(pQuery, player.getStartingHandSize());
        player.setStartingHandSize(player.getStartingHandSize() - 1);
        player.setMulligan(false);
        player.setCanMulligan(false);
        resetPlayersReady({m_entities,player.getGame()});
    }
    
    bool allPlayersReady(GameQuery gQuery) const {
        auto allReady = true;
        gQuery.eachPlayer([&](PlayerHandle player) {
            allReady = allReady && player.isReady();
        });
        return allReady;
    }

    void resetPlayersReady(GameQuery gQuery) const {
        gQuery.eachPlayer([&](PlayerHandle player) {
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
              
