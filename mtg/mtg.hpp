#include "ecst.hpp"
#include <iostream>
#include <functional>
#include <array>
#include <algorithm>
#include <cstdint>
#include <cstring>

namespace MTG {

/*******************************************************************************
 namespace Constants
******************************************************************************/
namespace Constants {
static constexpr auto MaxAbilities = 5U;

enum class Color : uint8_t { 
    GENERIC, COLORLESS, BLACK, BLUE, GREEN, RED, WHITE 
};

enum class Zone : uint8_t { 
    HAND, GRAVEYARD, LIBRARY, BATTLEFIELD, STACK, ALL, OUTSIDE 
};

enum class ZoneDetail : uint8_t {
    NONE, TOP, BOTTOM, CHOOSE 
};

enum class TurnPhase : uint8_t {
    BEGINNNG, PRECOMBAT_MAIN, COMBAT, POSTCOMBAT_MAIN, END 
};

enum class PhaseStep : uint8_t {
    UNTAP, UPKEEP, DRAW, PRECOMBAT_MAIN, BEGIN_COMBAT, DECLARE_ATTACKERS,
    DECLARE_BLOCKERS, FIRST_COMBAT_DAMAGE, COMBAT_DAMAGE, END_COMBAT,
    POSTCOMBAT_MAIN, END_TURN, CLEANUP
};

enum class TimingRule : uint8_t{
    INSTANT, SORCERY 
};

enum class TableState : uint8_t { 
    WAITING, READY_TO_START, STARTING, DRAFTING, CONSTRUCTING, 
    DUELING, SIDEBOARDING, FINISHED 
};

enum class UserNotify {
    NOT_ENOUGH_MANA
};

}

/*******************************************************************************
 namespace Mana
******************************************************************************/
namespace Mana {
using CColor = Constants::Color;

#pragma pack(push,1)
// Base class for mana. Occupies one byte where the first three bits represents
// the mana color and the last five bits represents the amount
class Base { 
    static constexpr uint8_t m = 1U << 4U;

    public:
    constexpr Base() : c(0), a(0) {}
    constexpr Base(CColor color, int8_t amount) : c{(uint8_t)color}, a(amount & 0x1f) {}
    constexpr CColor color()  const { return (CColor)c; }
    constexpr int8_t amount() const { return a; }
    constexpr Base& operator=(const Base &) = default;
    private:
    unsigned c:3;
    int      a:5;
};
#pragma pack(pop)

// Shortcuts for creating mana of different color
struct Black : Base { 
    constexpr Black(int8_t a) : Base{CColor::BLACK, a} {} 
    constexpr Black()                         = default;
    constexpr Black& operator=(const Black &) = default;   
};
struct Blue : Base { 
    constexpr Blue(int8_t a) : Base{CColor::BLUE, a} {} 
    constexpr Blue()                        = default;
    constexpr Blue& operator=(const Blue &) = default;
};
struct Colorless : Base { 
    constexpr Colorless(int8_t a) : Base{CColor::COLORLESS, a} {} 
    constexpr Colorless()                             = default;
    constexpr Colorless& operator=(const Colorless &) = default;
};
struct Generic : Base { 
    constexpr Generic(int8_t a) : Base{CColor::GENERIC, a} {} 
    constexpr Generic()                           = default;
    constexpr Generic& operator=(const Generic &) = default;
};
struct Green : Base { 
    constexpr Green(int8_t a) : Base{CColor::GREEN, a} {} 
    constexpr Green()                         = default;
    constexpr Green& operator=(const Green &) = default;
};
struct Red : Base { 
    constexpr Red(int8_t a) : Base{CColor::RED, a} {} 
    constexpr Red()                       = default;
    constexpr Red& operator=(const Red &) = default;
};
struct White : Base { 
    constexpr White(int8_t a) : Base{CColor::WHITE, a} {} 
    constexpr White()                         = default;
    constexpr White& operator=(const White &) = default;
};

// Cost class, in alphabetical order
struct Collection {
    constexpr Collection() = default;
    constexpr Collection(const Collection &) = default;
    Black     black;
    Blue      blue;
    Colorless colorless;
    Generic   generic;
    Green     green;
    Red       red;
    White     white;
};
}

/*******************************************************************************
 namespace Energy
******************************************************************************/
namespace Energy {
using Base = int8_t;
}

/*******************************************************************************
 namespace Player
******************************************************************************/
namespace Property {
using Hitpoints = int8_t;
using Name      = std::array<char,20>;
using Loyalty   = int8_t;
using Power     = int8_t;
using Toughness = int8_t;
}

/*******************************************************************************
 namespace Component
******************************************************************************/
namespace Component {
struct Ability          { std::function<void(void)> implementation; };
struct Color            { Constants::Color value; };
struct Effect           { std::function<void(void)> implementation; };
struct EnergyPool       { Energy::Base value; };
struct Hitpoints        { Property::Hitpoints value; };
struct Loyalty          { Property::Loyalty value; };
struct ManaPool         { Mana::Collection value; };
struct Name             { Property::Name value; };
struct PhaseStep        { Constants::PhaseStep value; };
struct Power            { Property::Power value; };
struct TableState       { Constants::TableState value; };
struct TimingRule       { Constants::TimingRule value; };
struct TurnPhase        { Constants::TurnPhase value; };
struct Toughness        { Property::Toughness value; };
struct UserNotify { Constants::UserNotify value; };
struct Zone             { Constants::Zone value; };
struct ZoneDetail       { Constants::ZoneDetail value; };
}

/*******************************************************************************
 namespace ComponentTag
******************************************************************************/
namespace ComponentTag {

namespace sc = ecst::signature::component;
namespace tc = ecst::tag::component;

constexpr auto Ability          = tc::v<Component::Ability>;
constexpr auto Color            = tc::v<Component::Color>;
constexpr auto Effect           = tc::v<Component::Effect>;
constexpr auto EnergyCost       = tc::v<Component::EnergyPool>;
constexpr auto EnergyPool       = tc::v<Component::EnergyPool>;
constexpr auto Hitpoints        = tc::v<Component::Hitpoints>;
constexpr auto Loyalty          = tc::v<Component::Loyalty>;
constexpr auto ManaCost         = tc::v<Component::ManaPool>;
constexpr auto ManaPool         = tc::v<Component::ManaPool>;
constexpr auto Name             = tc::v<Component::Name>;
constexpr auto PhaseStep        = tc::v<Component::PhaseStep>;
constexpr auto Power            = tc::v<Component::Power>;
constexpr auto TableState       = tc::v<Component::TableState>;
constexpr auto TimingRule       = tc::v<Component::TimingRule>;
constexpr auto TurnPhase        = tc::v<Component::TurnPhase>;
constexpr auto Toughness        = tc::v<Component::Toughness>;
constexpr auto UserNotify       = tc::v<Component::UserNotify>;
constexpr auto Zone             = tc::v<Component::Zone>;
constexpr auto ZoneDetail       = tc::v<Component::ZoneDetail>;
}

/*******************************************************************************
 namespace System
******************************************************************************/
namespace System {
struct ToughnessPrinter {
    template<typename TData>
    void process (TData& data) {
        data.for_entities([&](auto eid) {
            auto &t = data.get(ComponentTag::Toughness, eid);
            std::cout << eid << "->" << (int)t.value << '\n';
            t.value++;
        });
    }
};
struct PlayerPrinter {
    template<typename TData>
    void process (TData& data) {
        data.for_entities([&](auto eid) {
            auto& name = data.get(ComponentTag::Name, eid);
            auto& hp   = data.get(ComponentTag::Hitpoints, eid);
            std::cout << eid << " -> ";
            for(auto &c : name.value) std::cout << c;
            std::cout << " -> " << (int)hp.value << '\n';
        });
    }
};
}

/*******************************************************************************
 namespace SystemTag
******************************************************************************/
namespace SystemTag {
constexpr auto ToughnessPrinter = ecst::tag::system::v<System::ToughnessPrinter>;
constexpr auto PlayerPrinter = ecst::tag::system::v<System::PlayerPrinter>;
}

/*******************************************************************************
 namespace Setup
******************************************************************************/
namespace Setup {
constexpr auto makeComponentSignatureList() {
    namespace sc  = ecst::signature::component;
    namespace slc = ecst::signature_list::component;
    
    return slc::make(
        sc::make(ComponentTag::Ability).contiguous_buffer(),
        sc::make(ComponentTag::Color).contiguous_buffer(),
        sc::make(ComponentTag::Effect).contiguous_buffer(),
        sc::make(ComponentTag::EnergyCost).contiguous_buffer(),
        sc::make(ComponentTag::EnergyPool).contiguous_buffer(),
        sc::make(ComponentTag::Hitpoints).contiguous_buffer(),
        sc::make(ComponentTag::Loyalty).contiguous_buffer(),
        sc::make(ComponentTag::ManaCost).contiguous_buffer(),
        sc::make(ComponentTag::ManaPool).contiguous_buffer(),
        sc::make(ComponentTag::Name).contiguous_buffer(),
        sc::make(ComponentTag::PhaseStep).contiguous_buffer(),
        sc::make(ComponentTag::Power).contiguous_buffer(),
        sc::make(ComponentTag::TableState).contiguous_buffer(),
        sc::make(ComponentTag::TimingRule).contiguous_buffer(),
        sc::make(ComponentTag::Toughness).contiguous_buffer(),
        sc::make(ComponentTag::TurnPhase).contiguous_buffer(),
        sc::make(ComponentTag::UserNotify).contiguous_buffer(),
        sc::make(ComponentTag::Zone).contiguous_buffer(),
        sc::make(ComponentTag::ZoneDetail).contiguous_buffer()
    );
}

constexpr auto makeSystemSignatureList() {
    namespace ss  = ecst::signature::system;
    namespace sls = ecst::signature_list::system;
      
    namespace ips = ecst::inner_parallelism::strategy;
    namespace ipc = ecst::inner_parallelism::composer;

    constexpr auto none = ips::none::v();

    constexpr auto ssigToughness = ss::make(SystemTag::ToughnessPrinter)
        .parallelism(none)
        .write(ComponentTag::Toughness);

    constexpr auto ssigPlayer = ss::make(SystemTag::PlayerPrinter)
        .parallelism(none)
        .read(ComponentTag::Name, ComponentTag::Hitpoints);

    return sls::make(ssigToughness, ssigPlayer);
}
}

/*******************************************************************************
 namespace Factory
******************************************************************************/
namespace Factory {

template<typename TProxy>
auto makePlayer(
    TProxy&             proxy, 
    const char*         name,
    Property::Hitpoints hp,
    Mana::Collection    manaPool,
    Energy::Base        energyPool
) {
    auto eid = proxy.create_entity();
    { // Name
        auto& c = proxy.add_component(ComponentTag::Name, eid);
        std::copy(name, name + std::strlen(name), c.value.begin());
    }
    { // HP
        auto& c = proxy.add_component(ComponentTag::Hitpoints, eid);
        c.value = hp;
    }
    { // Mana Pool
        auto& c = proxy.add_component(ComponentTag::ManaPool, eid);
        c.value = manaPool;
    }
    { // Energy Pool
        auto& c = proxy.add_component(ComponentTag::EnergyPool, eid);
        c.value = energyPool;
    }
    return eid;
}

template<typename TProxy>
auto makeCreature(
    TProxy&                  proxy, 
    MTG::Property::Toughness toughness, 
    MTG::Property::Power     power
) {
    auto eid = proxy.create_entity();
    { // Add toughness component
        auto& c = proxy.add_component(ComponentTag::Toughness, eid);
        c.value = toughness;
    }
    { // Add power component
        auto& c = proxy.add_component(ComponentTag::Power, eid);
        c.value = power;
    }
    {
        auto& c = proxy.add_component(ComponentTag::Loyalty, eid);
        c.value = 12;
    }
    return eid;
}
}

using namespace Mana;
void loop() {
    // Aliases
    using namespace MTG::Setup;
    namespace cs  = ecst::settings;
    namespace ss  = ecst::scheduler;
    namespace sea = ecst::system_execution_adapter;

    // Instantiate settings
    constexpr auto s = 
        cs::make()
        .fixed_entity_limit(ecst::sz_v<1000>)
        .component_signatures(makeComponentSignatureList())
        .system_signatures(makeSystemSignatureList())
        .scheduler(cs::scheduler<ss::s_atomic_counter>);
    auto ctx = ecst::context::make_uptr(s);

    // Create some dummy data
    ctx->step([&](auto& proxy) {
        // for(auto i = 0 ; i < 100 ; ++i) {
        //     MTG::Factory::makeCreature(proxy, 1, 2);
        // }
        // MTG::Factory::makePlayer(proxy,
        //     "Simon", 
        //     12,
        //     {
        //         Black{1}, 
        //         Blue{2}, 
        //         Colorless{0},
        //         Generic{-1},
        //         Green{2},
        //         Red{2},
        //         White{5}
        //     },
        //     55);
    });

    // while(true) {
    //     ctx->step([](auto& proxy) {
    //         proxy.execute_systems_from(MTG::SystemTag::ToughnessPrinter)(
    //             sea::t(MTG::SystemTag::ToughnessPrinter)
    //             .for_subtasks([](auto& s, auto& data) {
    //                 s.process(data);
    //             })
    //         );
    //         proxy.execute_systems_from(MTG::SystemTag::PlayerPrinter)(
    //             sea::t(MTG::SystemTag::PlayerPrinter)
    //             .for_subtasks([](auto& s, auto& data) {
    //                 s.process(data);
    //             })
    //         );
    //     });
    // }
}

} // namespace MTG

