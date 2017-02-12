#pragma once
#include "entityx/entityx.h"
#include "constants.hpp"
#include "component.hpp"

namespace MTG {
using namespace entityx;

/**    
 * The state of a game/simulation
 **/
struct GameState : public EntityX {
    
    template<typename TTrigger>    
    void action(Entity trigger, Entity target = {});

    template<typename TTrigger>
    bool check(Entity trigger, Entity target = {});

    template<typename TTrigger>    
    void pay(Entity trigger, Entity target = {});

    void start(Entity player1, Entity player2) {
        m_players.reserve(2);
        m_players.push_back(player1);
        m_players.push_back(player2);
    }

    void pass(Entity player) {
        
    }
    
    void skipPhase(Phase phase) {
        m_phase = phase;
        m_step  = Step::UNDEFINED;
    }

    void skipStep(Step step) {
        m_step = step;
    }

    void setActivePlayer(Entity player) {
        m_activePlayer = player;
    }

    std::vector<Entity> stack() {
        std::vector<Entity> s;
        ComponentHandle<ZoneComponent> zone;
        for(Entity e : entities.entities_with_components(zone)) {
            if(Zone::STACK != zone->current) continue;
            s.push_back(e);
        }
        return s;
    }

    Phase phase() const { return m_phase; }
    Step  step()  const { return m_step; }
    
private:
    Phase               m_phase;
    Step                m_step;
    std::vector<Entity> m_players;
    Entity              m_activePlayer;
};

/**
 * Perform a trigger action
 * 
 * @tparam TTrigger The trigger component type
 * @param  trigger  The trigger entity
 * @param  target   The target  entity
 *
 **/
template<typename TTrigger>    
inline void GameState::action(Entity trigger, Entity target) {
    auto c = trigger.component<TTrigger>();
    c->action(*this, trigger, target);
}

/**
 * Check whether the trigger can be activated
 * 
 * @tparam TTrigger The trigger component type
 * @param  trigger  The trigger entity
 * @param  target   The target  entity
 *
 * @returns true if the trigger can be activated
 **/
template<typename TTrigger>
inline bool GameState::check(Entity trigger, Entity target) {
    auto c = trigger.component<TTrigger>();
    return c->check(*this,trigger,target);
}

/**
 * Perform the payment action for the trigger
 * 
 * @tparam TTrigger The trigger component type
 * @param  trigger  The trigger entity
 * @param  target   The target  entity
 **/
template<typename TTrigger>    
inline void GameState::pay(Entity trigger, Entity target) {
    auto c = trigger.component<TTrigger>();
    return c->pay(*this,trigger,target);
}

/**
 * Checks if the current game state allow plaing a sorcery
 * 
 * @state The current game state
 *
 * @returns true if it is allowed to play a sorcery
 **/
inline bool canPlaySorcery(GameState &state) {
    const auto empty = state.stack().empty();
    const auto main1 = state.phase() == Phase::PRECOMBAT_MAIN;
    const auto main2 = state.phase() == Phase::POSTCOMBAT_MAIN;
    return empty && (main1 || main2);
}

    
template<Phase P>
inline bool check(GameState& mgr) {
    return P == mgr.phase();
}

template<Timing T>
inline bool check(GameState& mgr);
    
template<>
inline bool check<Timing::SORCERY>(GameState& mgr) {
    return mgr.phase() == Phase::PRECOMBAT_MAIN
        || mgr.phase() == Phase::POSTCOMBAT_MAIN;
}
    
} // namespace MTG
