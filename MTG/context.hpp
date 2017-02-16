#pragma once
#include <gsl/gsl>
#include <entityx/quick.h>
#include <tuple>
#include <random>
#include <cstring>
#include <algorithm>

#include <game.hpp>
#include <player.hpp>
#include <card.hpp>
#include <random.hpp>
#include <systems.hpp>

namespace MTG {
    namespace ex = entityx;
    
    class Context : public ex::EntityX {
    public:

        Context(pRandomGenerator random) : m_random{std::move(random)} {
            
        }

        Context() : Context(std::make_unique<RandomGenerator>()) {}

        void advance() {
            systems.update_all(1.0);
        }

        auto makeGame() {
            auto game = GameHandle{entities.create()};
            game.assign<Game>();
            return game;
        }
        
        auto makePlayer(GameHandle& game) {
            auto player = PlayerHandle{entities.create()};
            player.assign<Player>();
            player->game             = game;
            player->flags            = 0;
            player->life             = 20;
            player->startingHandSize = 7;
            return player;
        }        

        auto makeCard(PlayerHandle player, std::string name) {
            auto entity = CardHandle{entities.create()};
            auto card   = entity.assign<Card>();
            card->player = player;
            std::strncpy(card->name, name.c_str(), Card::MAX_NAME_LENGTH);
            return entity;
        }

        bool isConnected(PlayerHandle player) {
            return player.component<Socket>().valid();
        }

        void connect(PlayerHandle player) {
            if(isConnected(player)) return;
            player.assign<Socket>();
        }

        void setDeck(PlayerHandle player, gsl::span<const char*> cards) {
            eachCard(player,[](auto card) { card.destroy(); });
            std::vector<const char*> mcards(cards.begin(), cards.end());
            m_random->shuffle(mcards);
            for(auto i = 0 ; i < mcards.size() ; ++i) {
                auto c = makeCard(player,mcards[i]);
                auto zone = (i < player->startingHandSize)
                    ? Card::HAND
                    : Card::LIBRARY;
                card::setZone(c, zone);
            }
        }

        CardVector getLibrary(PlayerHandle player) {
            MutableCardVector cards;
            cards.reserve(60);
            eachCard(player, [&](CardHandle card) {
                if(card::isInLibrary(card)) cards.push_back(card);
            });
            return (CardVector)cards;
        }

        CardVector getHand(PlayerHandle player) {
            MutableCardVector cards;
            cards.reserve(7);
            eachCard(player,[&](CardHandle card) {
                if(card::isInHand(card)) {
                    cards.push_back(card);
                }
            });
            return (CardVector)cards;
        }        

        auto getPlayers(GameHandle game) {
            MutablePlayerVector players;
            players.reserve(2);
            GameQuery{game}.eachPlayer(entities, [&](auto playerHandle) {
                players.push_back(playerHandle);
            });
            return (PlayerVector)players;            
        }
        
    protected:

        ////////////////////////////////////////////////////////////////////////
        // void randomizeActivePlayer(GameHandle game) {
        //     auto players = getPlayers(game);
        //     m_random->shuffle(players);
        //     player::setActive(*players.begin(), true);
        // }
        ////////////////////////////////////////////////////////////////////////

        template<typename TFunc>
        void eachCard(PlayerHandle player, TFunc&& func) {
            entities.each<Card>([&](CardHandle cardHandle, auto&) {
                if(cardHandle->player == player) func(cardHandle);
            });
        }
        
    private:
        pRandomGenerator m_random;
    };
}
