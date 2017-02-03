#include "Component.hpp"

namespace MTG {
namespace cards {
    using entityx::EntityX;
    using Component::CardComponent;
    using Component::EnchantCreatureComponent;
    using Component::CreatureComponent;
    using Component::EvasionAbilityComponent;
    using Component::CreaturesYouControlHaveEvasion;
    
auto makeAeronautAdmiral(EntityX& ex) {
    auto e = ex.entities.create();

    auto cardComponent = e.assign<CardComponent>();
    cardComponent->type    = Constants::CardType::CREATURE;
    cardComponent->subType = Constants::CardSubtype::PILOT;
        
    auto creatureComponent = e.assign<CreatureComponent>();
    creatureComponent->power     = 3;
    creatureComponent->toughness = 1;
    creatureComponent->race      = Constants::Race::HUMAN;
        
    auto evasionComponent = e.assign<EvasionAbilityComponent>();
    evasionComponent->evasion = Constants::EvasionAbility::FLYING;

    auto activeEffect = e.assign<CreaturesYouControlHaveEvasion>();
    activeEffect->evasion = Constants::EvasionAbility::FLYING;

    return e;
}
        
} // namespace Card
} // namespace MTG
