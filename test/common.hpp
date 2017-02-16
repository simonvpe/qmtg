#pragma once
#include <gsl/gsl>
#include <entityx/quick.h>
#include <tuple>
#include <random>
#include <cstring>
#include <algorithm>

namespace MTG {
    namespace ex = entityx;

    ////////////////////////////////////////////////////////////////////////////
    struct Game {
        bool started;
    };
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    struct GameHandle : public ex::Entity {
        GameHandle(ex::Entity e = {})
        : ex::Entity{e}
        {}
        auto operator->() { return component<Game>(); }
    };
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    struct Socket {
    };
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    struct Player {
        enum Flags { ACTIVE = 1, READY = 2, MULLIGAN = 3 };
        GameHandle     game;
        ex::Entity     other;
        std::bitset<2> flags;
        int            life;
        int            startingHandSize;
        bool active() const { return flags[ACTIVE]; }
        void setActive()    { flags[ACTIVE] = true; }
        bool ready()  const { return flags[READY];  }
        void setReady()     { flags[READY]  = true; }
    };
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    struct PlayerHandle : public ex::Entity {
        PlayerHandle(ex::Entity e = {})
        : ex::Entity{e}
        {}
        auto operator->() { return component<Player>(); }
    };
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    struct Card {
        enum { MAX_NAME_LENGTH = 20};
        enum Zone { LIBRARY, HAND };
        char         name[MAX_NAME_LENGTH];
        Zone         zone;
        PlayerHandle player;
        bool isInLibrary() const { return LIBRARY == zone; }
        bool isInHand()    const { return HAND    == zone; }
    };
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    struct CardHandle : public ex::Entity {
        CardHandle(ex::Entity e = {})
        : ex::Entity{e}
        {}
        auto operator->()
        { return component<Card>(); }
        
        bool operator==(const char *rhs)
        { return 0 == std::strcmp(rhs,(*this)->name); }
    };
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    namespace card {
        inline auto make(
        ex::EntityManager& mgr,
        PlayerHandle       player,
        std::string        name) {
            auto e = CardHandle{mgr.create()};
            auto card = e.assign<Card>();
            card->player = player;
            std::strncpy(card->name, name.c_str(), Card::MAX_NAME_LENGTH);
            return e;
        }

        inline void move(
        CardHandle card,
        Card::Zone zone) {
            card->zone = zone;
        }
        
    }
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    class RandomGenerator {
    public:
        ////////////////////////////////////////////////////////////////////////
        RandomGenerator()
        : m_gen{m_rd()}
        {}
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        virtual void
        shuffle(
        gsl::span<const char*> container) {
            std::shuffle(container.begin(), container.end(), m_gen);   
        }
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        virtual void
        shuffle(
        gsl::span<PlayerHandle> container) {
            std::shuffle(container.begin(), container.end(), m_gen);   
        }
        ////////////////////////////////////////////////////////////////////////

        
    private:
        std::random_device m_rd;
        std::mt19937       m_gen;
    };
    ////////////////////////////////////////////////////////////////////////////
    
    ////////////////////////////////////////////////////////////////////////////
    class Context : public ex::EntityX {
    public:
        ////////////////////////////////////////////////////////////////////////
        Context(std::unique_ptr<RandomGenerator> random)
        : m_random{std::move(random)}
        {}
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        Context()
        : Context(std::make_unique<RandomGenerator>())
        {}
        ////////////////////////////////////////////////////////////////////////
        
        ////////////////////////////////////////////////////////////////////////
        auto startGame() {
            auto game = GameHandle{entities.create()};
            game.assign<Game>();

            auto player1 = PlayerHandle{entities.create()};
            player1.assign<Player>();
            player1->game            = game;
            player1->flags           = 0;
            player1->life             = 20;
            player1->startingHandSize = 7;
            
            auto player2 = PlayerHandle{entities.create()};
            player2.assign<Player>();
            player2->game             = game;            
            player2->flags            = 0;
            player2->life             = 20;
            player2->startingHandSize = 7;

            player1->other = player2;
            player2->other = player1;
            
            return std::make_tuple(player1, player2);
        }
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        bool
        connected(
        PlayerHandle player) {
            return player.component<Socket>().valid();
        }
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        void
        connect(
        PlayerHandle player) {
            if(connected(player)) return;
            player.assign<Socket>();
        }
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        void
        setDeck(
        PlayerHandle player,
        gsl::span<const char*> cards) {
            eachCard(player,[](auto card) { card.destroy(); });
            std::vector<const char*> mcards(cards.begin(), cards.end());
            m_random->shuffle(mcards);
            for(auto i = 0 ; i < mcards.size() ; ++i) {
                auto c = card::make(entities,player,mcards[i]);
                auto zone = (i < player->startingHandSize)
                    ? Card::HAND
                    : Card::LIBRARY;
                card::move(c, zone);
            }
        }
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        std::vector<CardHandle>
        library(
        PlayerHandle player) {
            std::vector<CardHandle> cards;
            cards.reserve(60);
            eachCard(player, [&](CardHandle card) {
                if(card->isInLibrary()) cards.push_back(card);
            });
            return cards;
        }
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        std::vector<CardHandle>
        hand(
        PlayerHandle player) {
            std::vector<CardHandle> cards;
            cards.reserve(7);
            eachCard(player,[&](CardHandle card) {
                    if(card->isInHand()) {
                        cards.push_back(card);
                    }
            });
            return cards;
        }        
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        void
        setRandomGenerator(
        std::unique_ptr<RandomGenerator> generator) {
            m_random = std::move(generator);
        }
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        auto
        getPlayers(GameHandle game) {
            std::vector<PlayerHandle> players;
            players.reserve(2);
            eachPlayer(game,[&](auto playerHandle) {
                players.push_back(playerHandle);
            });
            return players;            
        }
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        auto
        setReady(PlayerHandle player) {
            auto game = player->game;
            player->setReady();
            bool allReady = true;
            eachPlayer(game,[&](auto playerHandle) {
                allReady = allReady && playerHandle->ready();
            });
            if( allReady ) initializeGame(game);
        }
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        void
        setMulligan(
        PlayerHandle player,
        bool selection) {
            if(selection)
        }
        ////////////////////////////////////////////////////////////////////////
        
        
    protected:
        ////////////////////////////////////////////////////////////////////////
        void
        initializeGame(GameHandle game) {
            randomizeActivePlayer(game);
            game->started = true;
        }
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        void
        randomizeActivePlayer(
        GameHandle game) {
            auto players = getPlayers(game);
            m_random->shuffle(players);
            (*players.begin())->setActive();
        }
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        void
        eachPlayer(
        GameHandle                          game,
        std::function<void(PlayerHandle)>&& func) {
            entities.each<Player>([&](PlayerHandle playerHandle, auto&) {
                if(playerHandle->game == game) func(playerHandle);
            });
        }
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        void
        eachCard(
        PlayerHandle                      player,
        std::function<void(CardHandle)>&& func) {
            entities.each<Card>([&](CardHandle cardHandle, auto&) {
                if(cardHandle->player == player) func(cardHandle);
            });
        }
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        CardHandle
        firstCard(
        PlayerHandle player,
        Card::Zone   zone) {
            entityx::ComponentHandle<Card> card;
            for(auto entity : entities.entities_with_components(card)) {
                if(card->player == player) return {entity};
            }
            return {};
        }
        ////////////////////////////////////////////////////////////////////////
        
    private:
        std::unique_ptr<RandomGenerator> m_random;
    };
    ////////////////////////////////////////////////////////////////////////////
}
