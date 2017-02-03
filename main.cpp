#include "entityx/entityx.h"
#include "SFML/Graphics.hpp"
#include "MTG/cards/cards.hpp"
#include "MTG/system/system.hpp"
#include "tinyfsm.hpp"
#include <iostream>

/*******************************************************************************
 * FSM
 ******************************************************************************/
namespace MTG {
using tinyfsm::Fsm;
using entityx::EntityX;
    
struct Event : tinyfsm::Event {
    Event(EntityX &e) : ex(e) {}
    EntityX &ex;
};
    
struct GameFsm : public Fsm<GameFsm> {
    // Unhandled events
    virtual void react(const Event &) {
        std::cout << "Unhandled event received!\n";
    }

    virtual void entry() {
        std::cout << "Entry GameFsm\n";
    }

    virtual void exit() {
        std::cout << "Exit GameFsm\n";
    }
};

struct Initial : public GameFsm {
    virtual void react(const Event &evt) {
        std::cout << "Event received S0!\n";
    }
};

FSM_INITIAL_STATE(MTG::GameFsm, MTG::Initial);

namespace GUI {
/*******************************************************************************
 * GUI Events
 ******************************************************************************/
struct MouseClickEvent {
    struct Position { int x,y; } position;
};
    
/*******************************************************************************
 * GUI Components
 ******************************************************************************/
struct SpriteComponent {
    struct Position { int x,y,z; } position;
    struct Size     { int w,h;   } size;
};

struct MouseClickComponent {
    bool clicked;
};

/*******************************************************************************
 * GUI Systems
 ******************************************************************************/
using entityx::EntityManager;
using entityx::EventManager;
using entityx::TimeDelta;
using entityx::Receiver;
using entityx::System;
using entityx::EntityCreatedEvent;
using entityx::EntityDestroyedEvent;
using entityx::ComponentAddedEvent;
using entityx::ComponentRemovedEvent;
using sf::RenderWindow;
    
class RenderSystem
    : public System<RenderSystem>
    , public Receiver<RenderSystem> {
public:
    RenderSystem(RenderWindow& window)
        : m_window(window)
        , m_requiresUpdate{true} {
    }
    
    void configure(EventManager &evm) {
        evm.subscribe<EntityCreatedEvent>(*this);
        evm.subscribe<EntityDestroyedEvent>(*this);
        evm.subscribe<ComponentAddedEvent<SpriteComponent>>(*this);
        evm.subscribe<ComponentRemovedEvent<SpriteComponent>>(*this);
    }

    void update(EntityManager &enm, EventManager &evm, TimeDelta dt) {
        if(!m_requiresUpdate) return;
        m_window.clear(sf::Color::Black);

        enm.each<SpriteComponent>([&](auto e, auto& sprite) {
            sf::RectangleShape r(sf::Vector2f(sprite.position.x, sprite.position.y));
            r.setSize(sf::Vector2f(sprite.size.w, sprite.size.h));
            m_window.draw(r);
        });
        
        m_window.display();
        m_requiresUpdate = false;
    }
    
    void receive(const EntityCreatedEvent& evt) {
        std::cout << "Update required\n";
        m_requiresUpdate = true;        
    }

    void receive(const EntityDestroyedEvent& evt) {
        std::cout << "Update required\n";
        m_requiresUpdate = true;
    }

    void receive(const ComponentAddedEvent<SpriteComponent>& evt) {
        std::cout << "Update required\n";
        m_requiresUpdate = true;
    }

    void receive(const ComponentRemovedEvent<SpriteComponent>& evt) {
        std::cout << "Update required\n";
        m_requiresUpdate = true;
    }
private:
    RenderWindow& m_window;
    bool          m_requiresUpdate;
};
    
class MouseClickSystem
    : public System<MouseClickSystem>
    , public Receiver<MouseClickSystem> {
public:
    void configure(EventManager &evm) {
        evm.subscribe<MouseClickEvent>(*this);
    }
    
    void update(EntityManager &enm, EventManager &evm, TimeDelta dt) {
    }

    void receive(const MouseClickEvent &evt) {
        std::cout << "Mouse click event received ("
                  << evt.position.x << "," << evt.position.y << ")!\n";
    }
};
} // namespace GUI
} // namespace MTG

int main() {
    using namespace MTG::cards;
    using namespace entityx;
    using MTG::GameFsm;
    using MTG::Component::CreatureComponent;
    using MTG::Component::EnchantCreatureComponent;
    using MTG::GUI::MouseClickEvent;
    using MTG::GUI::MouseClickSystem;
    using MTG::GUI::RenderSystem;

    EntityX ex;
    GameFsm fsm;
    
    sf::RenderWindow window(sf::VideoMode(800,600), "Magic The Gathering");
    ex.systems.add<MouseClickSystem>();
    ex.systems.add<RenderSystem>(window);
    ex.systems.configure();

    auto e = makeAeronautAdmiral(ex);
    auto sprite = e.assign<MTG::GUI::SpriteComponent>();
    sprite->position.x = 0;
    sprite->position.y = 0;
    sprite->size.w = 100;
    sprite->size.h = 100;
    
    while(window.isOpen()) {
        sf::Event evt;
        while(window.pollEvent(evt)) {
            switch(evt.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                ex.events.emit<MouseClickEvent>({{
                    evt.mouseButton.x,
                    evt.mouseButton.y
                }});
                break;
            default:
                break;
            }
            ex.systems.update<MouseClickSystem>(TimeDelta(1.0));
            ex.systems.update<RenderSystem>(TimeDelta(1.0));
        }
    }
    
    e.destroy();    
    // fsm.dispatch(MTG::Event{ex});
    
    
    
    // ex.entities.each<CreatureComponent>([](auto e, auto& creature) {
    //         std::cout << "T: " << (int)creature.toughness << '\n';
    // });
}
