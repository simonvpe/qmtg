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
            systems.add<GameSystem>();
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
        
        const auto makePlayer(GameHandle game) {
            auto player = PlayerHandle{entities.create()};
            player.assign<Player>();
            player.setGame(game);
            player.setLife(20);
            player.setStartingHandSize(7);
            return player;
        }        

        auto makeCard(PlayerHandle player, std::string name) {
            auto card = CardHandle{entities.create()};
            card.assign<Card>();
            card.setPlayer(player);
            card.setName(name.c_str());
            return card;
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
                auto card = makeCard(player,mcards[i]);
                if(i < player.getStartingHandSize())
                    card.moveToHand();
                else
                    card.moveToLibrary();
            }
        }

        CardVector getLibrary(PlayerHandle player) {
            MutableCardVector cards;
            cards.reserve(60);
            eachCard(player, [&](CardHandle card) {
                if(card.isInLibrary()) cards.push_back(card);
            });
            return (CardVector)cards;
        }

        CardVector getHand(PlayerHandle player) {
            MutableCardVector cards;
            cards.reserve(7);
            eachCard(player,[&](CardHandle card) {
                if(card.isInHand()) {
                    cards.push_back(card);
                }
            });
            return (CardVector)cards;
        }        

        auto getPlayers(GameHandle game) {
            MutablePlayerVector players;
            players.reserve(2);
            eachPlayer(entities, game, [&](auto playerHandle) {
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
        void eachPlayer(EntityManager& entities, GameHandle game, TFunc&& func) {
            entities.each<Player>([&](PlayerHandle player, auto&) {
                if(player.getGame() == game) func(player);
            });
        }

        template<typename TFunc>
        void eachCard(PlayerHandle player, TFunc&& func) {
            entities.each<Card>([&](CardHandle cardHandle, auto&) {
                if(cardHandle.getPlayer() == player) {
                    func(cardHandle);
                }
            });
        }
        
    private:
        pRandomGenerator m_random;
    };
}
