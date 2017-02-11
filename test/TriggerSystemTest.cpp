#include "gtest/gtest.h"
#include "entityx/quick.h"
#include "gamestate.hpp"

struct TriggerSystemTest : ::testing::Test {
    virtual void SetUp() {
        game = new MTG::GameState{};
    }

    virtual void TearDown() {
        delete game;
    }

    MTG::GameState* game;
};

TEST_F(TriggerSystemTest, triggers_phase_change) {
    
}
