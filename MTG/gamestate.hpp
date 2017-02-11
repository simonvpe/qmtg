#pragma once
#include "entityx/entityx.h"
#include "constants.hpp"

namespace MTG {
using namespace entityx;
    
/**
 * The possible turn phases
 **/
enum class Phase {
    UNDEFINED = 0,
    BEGINNING = (1 << 0),
    MAIN_1    = (1 << 1),
    COMBAT    = (1 << 2),
    MAIN_2    = (1 << 3),
    ENDING    = (1 << 4)
};

/**    
 * The state of a game/simulation
 **/
struct GameState : public EntityX {
    Phase               phase;
    std::vector<Entity> stack;

    template<typename TTrigger>    
    void action(Entity trigger, Entity target = {});

    template<typename TTrigger>
    bool check(Entity trigger, Entity target = {});

    template<typename TTrigger>    
    void pay(Entity trigger, Entity target = {});
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
    const auto empty = state.stack.empty();
    const auto main1 = state.phase == Phase::MAIN_1;
    const auto main2 = state.phase == Phase::MAIN_2;
    return empty && (main1 || main2);
}

    
template<Phase P>
inline bool check(GameState& mgr) {
    return P == mgr.phase;
}

template<Timing T>
inline bool check(GameState& mgr);
    
template<>
inline bool check<Timing::SORCERY>(GameState& mgr) {
    return mgr.phase == Phase::MAIN_1
        || mgr.phase == Phase::MAIN_2;
}
    
} // namespace MTG
