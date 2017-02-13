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
        m_player1 = player1;
        m_player2 = player2;
    }

    void pass(Entity player) {
        if(m_player1 == player) { m_player1Passed = true; }
        if(m_player2 == player) { m_player2Passed = true; }
    }

    void skipPhase(Phase phase) {
        m_phase = phase;
    }

    void skipStep(Step step) {
        m_step = step;
    }

    void setActivePlayer(Entity player) {
        m_activePlayer = player;
    }

    const std::vector<Entity> stack() {
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
    
    void nextStep() {
        switch(m_phase) {
            
            // Beginning phase
        case Phase::BEGINNING:
            switch(m_step) {
            case Step::UNTAP:
                m_step = Step::UPKEEP;
                break;
            case Step::UPKEEP:
                m_step = Step::DRAW;
                break;
            case Step::DRAW:
                m_phase = Phase::PRECOMBAT_MAIN;
                m_step  = Step::UNDEFINED;
                break;
            }
            break;
            // Precombat main phase
        case Phase::PRECOMBAT_MAIN:
            m_phase = Phase::COMBAT;
            m_step  = Step::BEGINNING_OF_COMBAT;
            break;
            
            // Combat phase
        case Phase::COMBAT:
            switch(m_step) {
            case Step::BEGINNING_OF_COMBAT:
                m_step = Step::DECLARE_ATTACKERS;
                break;
            case Step::DECLARE_ATTACKERS:
                m_step = Step::DECLARE_BLOCKERS;
                break;
            case Step::DECLARE_BLOCKERS:
                m_step = Step::COMBAT_DAMAGE;
                break;
            case Step::FIRST_STRIKE_COMBAT_DAMAGE:
                m_step = Step::COMBAT_DAMAGE;
                break;
            case Step::COMBAT_DAMAGE:
                m_step = Step::END_OF_COMBAT;
                break;
            case Step::END_OF_COMBAT:
                m_phase = Phase::POSTCOMBAT_MAIN;
                m_step  = Step::UNDEFINED;
                break;
            }
            break;
            
            // Postcombat main phase
        case Phase::POSTCOMBAT_MAIN:
            m_phase = Phase::ENDING;
            m_step =  Step::END;
            break;

        case Phase::ENDING:
            m_step = Step::CLEANUP;
            break;
        }
    }
    
    Phase  m_phase;
    Step   m_step;
    Entity m_player1,m_player2;
    bool   m_player1Passed, m_player2Passed;
    Entity m_activePlayer;
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
