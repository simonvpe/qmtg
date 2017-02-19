#pragma once
#include <entityx/entityx.h>
#include <game.hpp>
#include <player.hpp>

namespace MTG {
using namespace entityx;

class GameQuery {
public:
    GameQuery(EntityManager& mgr, GameHandle game)
    : m_entities{mgr}
    , m_game{game}
    {}

    template<typename TFun>
    void eachPlayer(TFun&& fun) {
        m_entities.each<Player>([&](PlayerHandle player, auto&){
            if(m_game == player.getGame()) fun(player);
        });
    }
    
private:
    EntityManager& m_entities;
    GameHandle     m_game;
};

class PlayerQuery {
public:
    PlayerQuery(EntityManager& mgr, PlayerHandle player)
    : m_entities{mgr}
    , m_player{player}
    {}

    template<typename TFun>
    void eachCard(TFun&& fun) {
        m_entities.each<Card>([&](CardHandle card, auto&) {
            if(m_player == card.getPlayer()) fun(card);
        });
    }

    template<typename TFun>
    void eachCardInHand(TFun&& fun) {
        eachCard([&](CardHandle card) {
            if(card.isInHand()) fun(card);
        });
    }

    template<typename TFun>
    void eachCardInLibrary(TFun&& fun) {
        eachCard([&](CardHandle card) {
            if(card.isInLibrary()) fun(card);
        });
    }

    PlayerHandle player() const {
        return m_player;
    }
    
private:
    EntityManager& m_entities;
    PlayerHandle   m_player;
};

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
              
