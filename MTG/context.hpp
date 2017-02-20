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

    template<typename T>
    using span = gsl::span<T>;
    
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

        void createDeck(PlayerHandle player, span<const char*> cards) {
            PlayerQuery pQuery{entities, player};
            pQuery.eachCard([](auto card) {
                throw std::overflow_error("Player already have deck assigned");
            });

            std::vector<const char*> mcards(cards.begin(), cards.end());
            m_random->shuffle(mcards);
            
            for(auto i = 0 ; i < mcards.size() ; ++i) {
                auto card = makeCard(player,mcards[i]);
                if(i < player.getStartingHandSize())
                    card.moveToHand();
                else
                    card.moveToLibrary();
            }

            player.setStartingHandSize(player.getStartingHandSize() - 1);
        }

        CardVector getLibrary(PlayerHandle player) {
            MutableCardVector cards;
            cards.reserve(60);
            PlayerQuery pQuery{entities, player};
            pQuery.eachCard([&](CardHandle card) {
                if(card.isInLibrary()) cards.push_back(card);
            });
            return (CardVector)cards;
        }

        CardVector getHand(PlayerHandle player) {
            MutableCardVector cards;
            cards.reserve(7);
            PlayerQuery pQuery{entities, player};
            pQuery.eachCard([&](CardHandle card) {
                if(card.isInHand()) {
                    cards.push_back(card);
                }
            });
            return (CardVector)cards;
        }        

        auto getPlayers(GameHandle game) {
            MutablePlayerVector players;
            players.reserve(2);
            GameQuery gQuery{entities, game};
            gQuery.eachPlayer([&](auto playerHandle) {
                players.push_back(playerHandle);
            });
            return (PlayerVector)players;            
        }
        
    private:
        pRandomGenerator m_random;
    };
}
