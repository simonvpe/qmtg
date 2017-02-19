#pragma once
#include <player.hpp>
#include <game.hpp>
#include <card.hpp>

namespace MTG {

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
    
}
