#pragma once
#include "Common.hpp"

namespace MTG {
namespace System {
    using entityx::EntityX;
    using entityx::Entity;
    using entityx::EntityManager;
    using entityx::EventManager;
    using entityx::TimeDelta;
    
    using Component::CardComponent;
    using Component::EnchantCreatureComponent;
    using Component::CreatureComponent;
    using Component::EvasionAbilityComponent;
    using Component::CreaturesYouControlHaveEvasion;
    

    SYSTEM(CreaturesYouControlHaveEvasionSystem) {
        void update(EntityManager& es, EventManager &events, TimeDelta) override {
            
        }
    };

} // namespace System
} // namespace MTG
