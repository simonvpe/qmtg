#pragma once
#include "common.hpp"
namespace MTG {
using namespace entityx;    
    
void move(Entity e, Zone z) {
    auto component = zone(e);
    DBG("Moving entity {} from {} to {}", e,(int)component->current, (int)z);
    component->previous = component->current;
    component->current = z;
}

auto makeTrigger(EntityManager&     mgr,
                 TriggerTiming   tt,
                 TriggerLifetime tl,
                 Entity          effect,
                 Entity          parent = Entity{}) {
    auto entity  =  mgr.create();
    auto trigger = entity.assign<TriggerComponent>();
    trigger->timing   = tt;
    trigger->lifetime = tl;
    trigger->effect   = effect;
    if(parent.valid())
        entity.assign<ParentComponent>()->entity = parent;
    DBG("Created trigger {} (timing {},lifetime {},effect {},parent {}",
        entity,(int)tt,(int)tl,effect,parent);
    return trigger;
}

auto makeEffect(EntityManager& mgr) {
    auto entity = mgr.create();
    auto zone = entity.assign<ZoneComponent>();
    zone->current =  Zone::UNDEFINED;
    zone->previous = Zone::UNDEFINED;
    DBG("Created effect {}", entity);
    return entity;
}

auto makeCreature(EntityManager& mgr, Power power,
                  Toughness toughness, CreatureType type,
                  CreatureClass classs) {
    auto entity = mgr.create();
    auto zone = entity.assign<ZoneComponent>();
    zone->current  = Zone::UNDEFINED;
    zone->previous = Zone::UNDEFINED;
    auto creature = entity.assign<CreatureComponent>();
    creature->power     = power;
    creature->toughness = toughness;
    creature->type      = type;
    creature->classs    = classs;
    DBG("Created creature {} (power {},toughness {},type {},class {})",
        entity,power,toughness,(int)type,(int)classs);
    return entity;
}
    
}
