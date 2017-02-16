#pragma once
#include <entityx/entityx.h>
#include <query.hpp>

namespace MTG {
using namespace entityx;

class PregameSystem : public System<PregameSystem> {

    void update(EntityManager &es, EventManager &events, TimeDelta dt) override {
        
    };
};
    
}
              
