#include "gtest/gtest.h"
#include "entityx/quick.h"
#include "gamestate.hpp"
#include "common.hpp"
#include "component.hpp"

namespace ex = entityx;

#include <functional>
namespace MTG {
        
    
auto makeGideonAllyOfZendikar(ex::EntityManager& mgr) {
    auto gideon = mgr.create();
    gideon.assign<ZoneComponent>()->zone                  = Zone::UNDEFINED;
    gideon.assign<PlaneswalkerComponent>()->loyalty       = {4};
    gideon.assign<MechanicComponent>()->summoningSickness = true;
    auto cost = gideon.assign<CardCostComponent>();
    cost->generic = {2};
    cost->white   = {2};
    
    // +1
    // ----------------------------------------------------------------------
    // Until end of turn, Gideon, Ally of Zendikar becomes a
    // 5/5 Human Soldier Ally creature with indestructible that's still a
    // planeswalker. Prevent all damage that would be dealt to him this turn.
    // ----------------------------------------------------------------------
    {
        auto ability = mgr.create();
        ability.assign<ParentComponent>()->entity = gideon;
        
        auto trigger = ability.assign<ManualTriggerComponent>();
        
        trigger->properties = ManualTriggerProperties::CARD_SLOT_0;
        
        trigger->check = [](GameState& state, ex::Entity self, auto target) {
            return check<Timing::SORCERY>(state)
            && check<Zone::BATTLEFIELD>(parent(self));
        };

        trigger->pay = [](GameState& state, ex::Entity self, auto target) {
            auto  gideon  = self.component<ParentComponent>()->entity;
            auto& loyalty = planeswalker(gideon)->loyalty;
            loyalty += 1;
        };

        trigger->action = [](GameState& state, ex::Entity self, auto target) {
            auto gideon = self.component<ParentComponent>()->entity;

            // Change into a creature component
            auto creature = gideon.assign<CreatureComponent>();
            creature->power     = {5};
            creature->toughness = {5};
            creature->type      = CreatureType::HUMAN;
            creature->classs    = CreatureClass::SOLDIER;

            // Remove component with an end of turn trigger
            auto endEntity = state.entities.create();
            auto endTrigger = endEntity.assign<EndOfTurnTriggerComponent>();
            endEntity.assign<ParentComponent>()->entity = gideon;
            endTrigger->action = [](GameState& state, ex::Entity self, auto) {
                auto gideon = self.component<ParentComponent>()->entity;
                gideon.remove<CreatureComponent>();
                self.destroy();
            };
        };
        
    }

    // 0
    // ----------------------------------------------------------------------
    // Create a 2/2 white Knight Ally creature token.
    // ----------------------------------------------------------------------
    {
        auto ability = mgr.create();
        ability.assign<ParentComponent>()->entity = gideon;

        auto trigger = ability.assign<ManualTriggerComponent>();
        trigger->properties = ManualTriggerProperties::CARD_SLOT_1;

        trigger->check = [](GameState& state, ex::Entity self, auto target) {
            return canPlaySorcery(state)
            && check<Zone::BATTLEFIELD>(parent(self));
        };

        trigger->pay = [](GameState& state, ex::Entity self, auto target) {
            return;
        };

        trigger->action = [](GameState& state, ex::Entity self, auto target) {
            auto card = state.entities.create();
            auto creature = card.assign<CreatureComponent>();
            creature->power     = {2};
            creature->toughness = {2};
            creature->type      = CreatureType::KNIGHT;
            creature->classs    = CreatureClass::ALLY | CreatureClass::TOKEN;
            card.assign<ZoneComponent>()->zone = Zone::BATTLEFIELD;
            card.assign<MechanicComponent>()->summoningSickness = true;
        };
    }        

    // -4
    // ----------------------------------------------------------------------
    // You get an emblem with "Creatures you control get +1/+1."
    // ----------------------------------------------------------------------
    {
        auto ability = mgr.create();
        ability.assign<ParentComponent>()->entity = gideon;

        auto trigger = ability.assign<ManualTriggerComponent>();
        trigger->properties = ManualTriggerProperties::CARD_SLOT_2;

        trigger->check = [](GameState& state, ex::Entity self, auto target) {
            // Enough loyalty points and if sorcery play is allowed
            auto gideon  = self.component<ParentComponent>()->entity;
            auto loyalty = planeswalker(gideon)->loyalty;
            return loyalty >= 4 && canPlaySorcery(state);
        };

        trigger->pay = [](GameState& state, ex::Entity self, auto target) {
            // Pay one loyalty, if loyalty reaches 0 move to graveyard
            auto  gideon  = self.component<ParentComponent>()->entity;
            auto& loyalty = planeswalker(gideon)->loyalty;
            loyalty -= 4;
            if(loyalty <= 0) {
                zone(gideon)->zone = Zone::GRAVEYARD;
            }
        };

        trigger->action = [](GameState& state, ex::Entity self, auto target) {
            auto card = state.entities.create();
            auto trigger = card.assign<EnterBattlefieldTriggerComponent>();
            trigger->action = [](GameState& state, ex::Entity self, auto target) {
                auto effect = state.entities.create();
                auto buff   = effect.assign<BuffComponent>();
                buff->power     = {1};
                buff->toughness = {1};
                buff->parent    = target;
            };
            card.assign<EmblemComponent>();
        };
    }
       
    

    return gideon;
}
} // namespace MTG

// Helpers

template<typename TComponent, typename TFunction>
auto count(ex::EntityManager& mgr, TFunction&& f) {
    auto count = 0;
    mgr.each<TComponent>([&](auto e, auto& component) {
        count += f(component) ? 1 : 0;
    });
    return count;
}

template<typename TComponent, typename TFunction>
auto first(ex::EntityManager& mgr, TFunction&& f) {
    ex::Entity result;
    mgr.each<TComponent>([&](auto e, auto& component) {
        result = f(component) && (result == ex::Entity{}) ? e : result;
    });
    return result;
}

template<typename TComponent>
auto first(ex::EntityManager& mgr) {
    ex::Entity result;
    mgr.each<TComponent>([&](auto e, auto& component) {
        result = (result == ex::Entity{}) ? e : result;
    });
    return result;
}


// Tests

struct GideonAllyOfZendikarTest : public ::testing::Test {
    virtual void SetUp() {
        game   = new MTG::GameState{};
        gideon = MTG::makeGideonAllyOfZendikar(game->entities);
        ASSERT_TRUE(gideon.valid());
    }

    virtual void TearDown() {
        gideon.destroy();
        delete game;
    }

    MTG::GameState*    game;
    ex::Entity         gideon;
};

using namespace MTG;
using namespace ex;

TEST_F(GideonAllyOfZendikarTest, test_planeswalker_loyalty) {
    ASSERT_TRUE( planeswalker(gideon).valid() );
    EXPECT_EQ( Loyalty{4}, planeswalker(gideon)->loyalty );
}

TEST_F(GideonAllyOfZendikarTest, test_mechanic_summoning_sickness) {
    ASSERT_TRUE( mechanic(gideon).valid() );
    EXPECT_EQ( true, mechanic(gideon)->summoningSickness );
}

TEST_F(GideonAllyOfZendikarTest, test_ability_0_created) {
    auto& mgr = game->entities;
    ASSERT_EQ(1, count<ManualTriggerComponent>(mgr,[](auto& component) {
        return test(component.properties & ManualTriggerProperties::CARD_SLOT_0);
    }));
}

TEST_F(GideonAllyOfZendikarTest, test_ability_0_entity) {
    auto& mgr = game->entities;
    auto trigger = first<ManualTriggerComponent>(mgr, [](auto& component) {
        return test(component.properties & ManualTriggerProperties::CARD_SLOT_0);
    });
    ASSERT_EQ(gideon, parent(trigger));
}

TEST_F(GideonAllyOfZendikarTest, test_ability0_check) {
    auto& mgr = game->entities;
    auto triggerEntity = first<ManualTriggerComponent>(mgr, [](auto& component) {
        return test(component.properties & ManualTriggerProperties::CARD_SLOT_0);
    });

    // Phase good
    game->phase = Phase::MAIN_1;
    zone(gideon)->zone = Zone::BATTLEFIELD;
    EXPECT_TRUE(parent(triggerEntity).valid());
    EXPECT_TRUE(check<Timing::SORCERY>(*game));
    EXPECT_TRUE(check<Zone::BATTLEFIELD>(parent(triggerEntity)));
    EXPECT_TRUE(game->check<ManualTriggerComponent>(triggerEntity, ex::Entity{}));
    
    game->phase = Phase::MAIN_2;
    zone(gideon)->zone = Zone::BATTLEFIELD;
    EXPECT_TRUE(game->check<ManualTriggerComponent>(triggerEntity, ex::Entity{}));

    
    // Phase bad
    game->phase = Phase::BEGINNING;
    zone(gideon)->zone = Zone::BATTLEFIELD;
    planeswalker(gideon)->loyalty = {0};
    EXPECT_FALSE(game->check<ManualTriggerComponent>(triggerEntity, ex::Entity{}));
}

TEST_F(GideonAllyOfZendikarTest, test_ability_0_pay) {
    auto& mgr = game->entities;
    auto triggerEntity = first<ManualTriggerComponent>(mgr, [](auto& component) {
        return test(component.properties & ManualTriggerProperties::CARD_SLOT_0);
    });

    // Survive and get loyalty
    zone(gideon)->zone            = Zone::BATTLEFIELD;
    planeswalker(gideon)->loyalty = {2};
    game->pay<ManualTriggerComponent>(triggerEntity, ex::Entity{});
    EXPECT_EQ( Zone::BATTLEFIELD, zone(gideon)->zone );
    EXPECT_EQ( Loyalty{3}, planeswalker(gideon)->loyalty );
}

TEST_F(GideonAllyOfZendikarTest, test_ability_0_action) {
    auto& mgr = game->entities;
    auto trigger = first<ManualTriggerComponent>(mgr, [](auto& component) {
        return test(component.properties & ManualTriggerProperties::CARD_SLOT_0);
    });

    EXPECT_FALSE(creature(gideon).valid());
    game->action<ManualTriggerComponent>(trigger, ex::Entity{});
    ASSERT_TRUE(creature(gideon).valid());
    
    EXPECT_EQ( MTG::Power{5},               creature(gideon)->power );
    EXPECT_EQ( MTG::Toughness{5},           creature(gideon)->toughness );
    EXPECT_EQ( MTG::CreatureType::HUMAN,    creature(gideon)->type );
    EXPECT_EQ( MTG::CreatureClass::SOLDIER, creature(gideon)->classs );

    // Trigger end of turn and make sure Gideons creature is removed
    auto endTrigger = first<EndOfTurnTriggerComponent>(mgr);
    ASSERT_TRUE( endTrigger.valid() );
    game->action<EndOfTurnTriggerComponent>(endTrigger, ex::Entity{} );
    EXPECT_FALSE( endTrigger.valid() );
    EXPECT_FALSE( creature(gideon).valid() );
}

TEST_F(GideonAllyOfZendikarTest, test_ability1_created) {
    auto& mgr = game->entities;
    ASSERT_EQ(1, count<ManualTriggerComponent>(mgr,[&](auto& component) {
        return (test(component.properties & ManualTriggerProperties::CARD_SLOT_1));
    }));
}

TEST_F(GideonAllyOfZendikarTest, test_ability1_check) {
    auto& mgr = game->entities;
    auto trigger = first<ManualTriggerComponent>(mgr, [](auto& component) {
        return test(component.properties & ManualTriggerProperties::CARD_SLOT_1);
    });
    ASSERT_TRUE(trigger.valid());
    ASSERT_TRUE(parent(trigger).valid());
    zone(gideon)->zone = Zone::BATTLEFIELD;
    
    // Phase good
    game->phase = Phase::MAIN_1;
    EXPECT_TRUE(game->check<ManualTriggerComponent>(trigger, Entity{}));

    // Phase good
    game->phase = Phase::MAIN_2;
    EXPECT_TRUE(game->check<ManualTriggerComponent>(trigger, Entity{}));
    
    // Phase bad
    game->phase = Phase::BEGINNING;
    ASSERT_FALSE(game->check<ManualTriggerComponent>(trigger, Entity{}));
}


TEST_F(GideonAllyOfZendikarTest, test_ability_1_pay) {
    auto& mgr = game->entities;
    auto trigger = first<ManualTriggerComponent>(mgr, [](auto& component) {
        return test(component.properties & ManualTriggerProperties::CARD_SLOT_1);
    });
    
    const auto loyalty = planeswalker(gideon)->loyalty;
    game->pay<ManualTriggerComponent>(trigger, ex::Entity{});
    EXPECT_EQ( loyalty, planeswalker(gideon)->loyalty );
}

TEST_F(GideonAllyOfZendikarTest, test_ability_1_action) {
    auto& mgr = game->entities;
    auto trigger = first<ManualTriggerComponent>(mgr, [](auto& component) {
        return test(component.properties & ManualTriggerProperties::CARD_SLOT_1);
    });
    game->action<ManualTriggerComponent>(trigger, ex::Entity{});

    auto token = first<CreatureComponent>(mgr);
    ASSERT_TRUE( token.valid() );
    
    ASSERT_TRUE( creature(token).valid() );
    EXPECT_EQ( Power{2}, creature(token)->power );
    EXPECT_EQ( Toughness{2}, creature(token)->toughness );
    EXPECT_EQ( CreatureType::KNIGHT, creature(token)->type );
    EXPECT_EQ( CreatureClass::ALLY | CreatureClass::TOKEN, creature(token)->classs );
    ASSERT_TRUE( mechanic(token).valid() );
    EXPECT_TRUE( mechanic(token)->summoningSickness );
}

TEST_F(GideonAllyOfZendikarTest, test_ability_2_created) {
    auto& mgr = game->entities;
    ASSERT_EQ(1, count<ManualTriggerComponent>(mgr,[&](auto& component) {
        return (test(component.properties & ManualTriggerProperties::CARD_SLOT_2));
    }));
}

TEST_F(GideonAllyOfZendikarTest, test_ability2_check) {
    auto& mgr = game->entities;
    auto trigger = first<ManualTriggerComponent>(mgr, [](auto& component) {
        return test(component.properties & ManualTriggerProperties::CARD_SLOT_2);
    });
    zone(gideon)->zone = Zone::BATTLEFIELD;
    
    // Phase good
    game->phase = Phase::MAIN_1;
    EXPECT_TRUE(game->check<ManualTriggerComponent>(trigger, Entity{}));

    // Phase good
    game->phase = Phase::MAIN_2;
    EXPECT_TRUE(game->check<ManualTriggerComponent>(trigger, Entity{}));
    
    // Phase bad
    game->phase = Phase::BEGINNING;
    ASSERT_FALSE(game->check<ManualTriggerComponent>(trigger, Entity{}));
}

TEST_F(GideonAllyOfZendikarTest, test_ability_2_pay) {
    auto& mgr = game->entities;
    auto trigger = first<ManualTriggerComponent>(mgr, [](auto& component) {
        return test(component.properties & ManualTriggerProperties::CARD_SLOT_2);
    });
    
    const auto loyalty = planeswalker(gideon)->loyalty;
    game->pay<ManualTriggerComponent>(trigger, ex::Entity{});
    EXPECT_EQ( loyalty - 4, planeswalker(gideon)->loyalty );
}

TEST_F(GideonAllyOfZendikarTest, test_ability_2_action) {
    auto& mgr = game->entities;
    auto trigger = first<ManualTriggerComponent>(mgr, [](auto& component) {
        return test(component.properties & ManualTriggerProperties::CARD_SLOT_2);
    });

    EXPECT_FALSE(creature(gideon).valid());
    game->action<ManualTriggerComponent>(trigger, ex::Entity{});

    auto emblem = first<EnterBattlefieldTriggerComponent>(mgr);
    ASSERT_TRUE(emblem.valid());
    
        
    // Trigger end of turn and make sure Gideons creature is removed
    // auto endTrigger = first<EndOfTurnTriggerComponent>(mgr);
    // ASSERT_TRUE( endTrigger.valid() );
    // game->action<EndOfTurnTriggerComponent>(endTrigger, ex::Entity{} );
    // EXPECT_FALSE( endTrigger.valid() );
    // EXPECT_FALSE( creature(gideon).valid() );
}
