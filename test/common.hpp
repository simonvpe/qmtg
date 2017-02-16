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
        enum Flags { STARTED = 0 };
        std::bitset<1> flags;
    };
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    struct GameHandle : public ex::Entity {
        GameHandle(ex::Entity e = {}) : ex::Entity{e} {}
        auto operator->() { return component<Game>(); }
        const auto operator->() const { return component<const Game>(); }
    };
    namespace game {
        bool isStarted(const GameHandle game) {
            return game->flags[Game::STARTED];
        }
        void setStarted(GameHandle game, bool value = true) {
            game->flags[Game::STARTED] = value;
        }
    }
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    struct Socket {
    };
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    struct Player {
        enum Flags { ACTIVE = 0, READY = 1, MULLIGAN = 2 };
        GameHandle     game;
        ex::Entity     other;
        std::bitset<2> flags;
        int            life;
        int            startingHandSize;
    };
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    struct PlayerHandle : public ex::Entity {
        PlayerHandle(ex::Entity e = {})
        : ex::Entity{e}
        {}
        auto operator->() { return component<Player>(); }
        const auto operator->() const { return component<const Player>(); }
    };
    using PlayerVector = const std::vector<PlayerHandle>;
    using MutablePlayerVector = std::vector<PlayerHandle>;
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    namespace player {
        inline bool isActive(const PlayerHandle& player) {
            return player->flags[Player::ACTIVE];
        }

        inline void setActive(PlayerHandle& player, bool value = true) {
            player->flags[Player::ACTIVE] = value;
        }

        inline bool isReady(const PlayerHandle& player) {
            return player->flags[Player::READY];
        }
        
        inline void setReady(PlayerHandle& player, bool value = true) {
            player->flags[Player::READY] = value;
        }
    }
    ////////////////////////////////////////////////////////////////////////////
    
    ////////////////////////////////////////////////////////////////////////////
    struct Card {
        enum { MAX_NAME_LENGTH = 20};
        enum Zone { LIBRARY, HAND };
        char         name[MAX_NAME_LENGTH];
        Zone         zone;
        PlayerHandle player;
    };
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    struct CardHandle : public ex::Entity {
        CardHandle(ex::Entity e = {})
        : ex::Entity{e}
        {}
        auto operator->() { return component<Card>(); }
        const auto operator->() const { return component<const Card>(); } 
        
        bool operator==(const char *rhs) const
        { return 0 == std::strcmp(rhs,(*this)->name); }
    };
    using CardVector = const std::vector<CardHandle>;
    using MutableCardVector = std::vector<CardHandle>;
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    namespace card {
        inline void setZone(CardHandle card, Card::Zone zone) {
            card->zone = zone;
        }

        inline Card::Zone getZone(const CardHandle& card) {
            return card->zone;
        }

        inline bool isInHand(const CardHandle &card) {
            return card->zone == Card::HAND;
        }

        inline bool isInLibrary(const CardHandle &card) {
            return card->zone == Card::LIBRARY;
        }
    }
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    class RandomGenerator {
    public:
        ////////////////////////////////////////////////////////////////////////
        RandomGenerator() : m_gen{m_rd()} {}
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        virtual void shuffle(gsl::span<const char*> container) {
            std::shuffle(container.begin(), container.end(), m_gen);   
        }
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        virtual void shuffle(gsl::span<PlayerHandle> container) {
            std::shuffle(container.begin(), container.end(), m_gen);   
        }
        ////////////////////////////////////////////////////////////////////////

        
    private:
        std::random_device m_rd;
        std::mt19937       m_gen;
    };
    using pRandomGenerator = std::unique_ptr<RandomGenerator>;
    ////////////////////////////////////////////////////////////////////////////
    
    ////////////////////////////////////////////////////////////////////////////
    
    class Context : public ex::EntityX {
    public:
        ////////////////////////////////////////////////////////////////////////
        Context(pRandomGenerator random) : m_random{std::move(random)} {
            
        }
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        Context() : Context(std::make_unique<RandomGenerator>()) {}
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        void advance() {
            systems.update_all(1.0);
        }
        ////////////////////////////////////////////////////////////////////////
        
        ////////////////////////////////////////////////////////////////////////
        auto makeGame() {
            auto game = GameHandle{entities.create()};
            game.assign<Game>();
            return game;
        }
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // auto start(GameHandle game) {
        // }
        ////////////////////////////////////////////////////////////////////////
        
        ////////////////////////////////////////////////////////////////////////
        auto makePlayer(GameHandle& game) {
            auto player = PlayerHandle{entities.create()};
            player.assign<Player>();
            player->game             = game;
            player->flags            = 0;
            player->life             = 20;
            player->startingHandSize = 7;
            return player;
        }        
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        auto makeCard(PlayerHandle player, std::string name) {
            auto entity = CardHandle{entities.create()};
            auto card   = entity.assign<Card>();
            card->player = player;
            std::strncpy(card->name, name.c_str(), Card::MAX_NAME_LENGTH);
            return entity;
        }
        ////////////////////////////////////////////////////////////////////////
        
        ////////////////////////////////////////////////////////////////////////
        bool isConnected(PlayerHandle player) {
            return player.component<Socket>().valid();
        }
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        void connect(PlayerHandle player) {
            if(isConnected(player)) return;
            player.assign<Socket>();
        }
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
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
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        CardVector getLibrary(PlayerHandle player) {
            MutableCardVector cards;
            cards.reserve(60);
            eachCard(player, [&](CardHandle card) {
                if(card::isInLibrary(card)) cards.push_back(card);
            });
            return (CardVector)cards;
        }
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
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
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        auto getPlayers(GameHandle game) {
            MutablePlayerVector players;
            players.reserve(2);
            eachPlayer(game,[&](auto playerHandle) {
                players.push_back(playerHandle);
            });
            return (PlayerVector)players;            
        }
        ////////////////////////////////////////////////////////////////////////    
        
    protected:

        ////////////////////////////////////////////////////////////////////////
        // void randomizeActivePlayer(GameHandle game) {
        //     auto players = getPlayers(game);
        //     m_random->shuffle(players);
        //     player::setActive(*players.begin(), true);
        // }
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        template<typename TFunc>
        void eachPlayer(GameHandle game, TFunc&& func) {
            entities.each<Player>([&](PlayerHandle playerHandle, auto&) {
                if(playerHandle->game == game) func(playerHandle);
            });
        }
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        template<typename TFunc>
        void eachCard(PlayerHandle player, TFunc&& func) {
            entities.each<Card>([&](CardHandle cardHandle, auto&) {
                if(cardHandle->player == player) func(cardHandle);
            });
        }
        ////////////////////////////////////////////////////////////////////////
        
    private:
        pRandomGenerator m_random;
    };
    ////////////////////////////////////////////////////////////////////////////
}
