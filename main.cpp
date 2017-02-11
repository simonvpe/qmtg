#include "entityx/entityx.h"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <deque>
#include <cmath>

// namespace ex = entityx;

// /*******************************************************************************
//  * FSM
//  ******************************************************************************/
// namespace MTG {

// struct EntityActivationEvent {
//     EntityActivationEvent(ex::Entity& e) : entity(e) {}
//     ex::Entity entity;
// };

// // Receives events from entityx and dispatches to
// // game FSM. These events must be tinyfsm::Event's
// class GameFsm : public ex::Receiver<GameFsm> {
// public:

//     GameFsm(ex::EntityManager& em, ex::EventManager& evm)
//         : m_entities{em}
//         , m_events{evm} {
//         evm.subscribe<EntityActivationEvent>(*this);
//     }

//     template<typename T>
//     void receive(const T& evt) {
//     }
    
// private:
//     ex::EntityManager&    m_entities;
//     ex::EventManager&     m_events;
// };

// namespace GUI {
// /*******************************************************************************
//  * GUI Events
//  ******************************************************************************/
// struct MouseClickEvent {
//     struct Position { int x,y; } position;
// };
    
// /*******************************************************************************
//  * GUI Components
//  ******************************************************************************/
// struct SpriteComponent {
//     SpriteComponent(const char* img) {
//         texture.loadFromFile(img);
//         sprite.setTexture(texture);
//     }
//     sf::Texture texture;
//     sf::Sprite  sprite;
// };

// /*******************************************************************************
//  * GUI Systems
//  ******************************************************************************/
// using ex::EntityManager;
// using ex::EventManager;
// using ex::TimeDelta;
// using ex::EntityCreatedEvent;
// using ex::EntityDestroyedEvent;
// using ex::ComponentAddedEvent;
// using ex::ComponentRemovedEvent;
// using sf::RenderWindow;
    
// class RenderSystem
//     : public ex::System<RenderSystem>
//     , public ex::Receiver<RenderSystem> {
// public:
//     RenderSystem(RenderWindow& window)
//         : m_window(window)
//         , m_requiresUpdate{true} {
//     }
    
//     void configure(EventManager &evm) {
//         evm.subscribe<EntityCreatedEvent>(*this);
//         evm.subscribe<EntityDestroyedEvent>(*this);
//         evm.subscribe<ComponentAddedEvent<SpriteComponent>>(*this);
//         evm.subscribe<ComponentRemovedEvent<SpriteComponent>>(*this);
//     }

//     void update(EntityManager &enm, EventManager &evm, TimeDelta dt) {
//         //if(!m_requiresUpdate) return;
//         m_window.clear(sf::Color::Black);
//         renderHand(enm,evm,dt);
//         m_window.display();
//         //m_requiresUpdate = false;
//     }

//     template<typename T>
//     void receive(const T& evt) { m_requiresUpdate = true; }
// protected:
    
//     void renderHand(EntityManager& enm, EventManager& evm, TimeDelta dt) {
//         // Hand is rendered on a circle with a specified width. The cards
//         // are overlapped to fit within a certain section of the circle.
//         auto   winSize   = sf::Vector2f{1024.0f,768.0f};
//         auto   origin    = sf::Vector2f{ winSize.x/2.0f, (float)winSize.y };

//         auto makePoints = [&] (int npoints) {
//             auto a = winSize.y;
//             auto b = winSize.x;
//             auto spread  = M_PI/6.0f;
            
//             auto origin = std::make_tuple(winSize.x/2.0f, (float)winSize.y);
//             auto start  = spread / 2.0f;
//             auto diff   = spread / (float)(npoints - 1);
            
//             auto i      = 0;
//             auto points = std::vector<std::tuple<float,float,float>>(npoints);
//             std::generate(points.begin(), points.end(), [&] {
//                 auto theta = i*diff - start;
//                 auto cos   = std::cos(theta);
//                 auto sin   = std::sin(theta);
//                 auto r     = a * b / std::sqrt( pow(a*sin,2) + pow(b*cos,2) );
//                 auto x = std::get<0>(origin) + r * sin;
//                 auto y = std::get<1>(origin) - r * cos;
//                 ++i;
//                 return std::make_tuple(x,y,theta/M_PI*180.0f);
//             });
//             auto adj = (float)winSize.y - std::get<1>(points[0]) - 5.0f;
//             std::transform(points.begin(), points.end(), points.begin(),
//                            [&](auto v) {
//                 return std::make_tuple(
//                     std::get<0>(v),
//                     std::get<1>(v) + adj,
//                     std::get<2>(v)
//                 );
//             });
//             return points;
//         };
//         auto points = makePoints(5);
//         for( auto& p : points ) {
//             auto c  = sf::CircleShape(5.0f,20);
//             c.setPosition({std::get<0>(p), std::get<1>(p)});
//             m_window.draw(c);
//         }
        
//         auto handIndex = 0;
//         auto f = [&](auto e, auto& sprite, const auto& zone){
//             const auto x     = std::get<0>(points[handIndex]);
//             const auto y     = std::get<1>(points[handIndex]);
//             const auto theta = std::get<2>(points[handIndex]);

//             auto& s = sprite.sprite;
//             const auto bounds = s.getLocalBounds();
//             s.setOrigin(bounds.width/2.0f, bounds.height/2.0f);
//             s.setPosition(x,y);
//             s.setRotation(theta);
//             m_window.draw(s);
//             ++handIndex;
//         };
//         //enm.each<SpriteComponent, ZoneComponent>(f);
//     }
    
// private:
//     RenderWindow& m_window;
//     bool          m_requiresUpdate;
// };

// // - Emits SpriteClickEvent's
// class MouseClickSystem
//     : public ex::System<MouseClickSystem>
//     , public ex::Receiver<MouseClickSystem> {
// public:
//     void configure(EventManager &evm) {
//         evm.subscribe<MouseClickEvent>(*this);
//     }
    
//     void update(EntityManager &enm, EventManager &evm, TimeDelta dt) {
//         while(!m_clicks.empty()) {
//             auto evt = m_clicks.front();

//             // Fire an EntityActivationEvent for the entity that was
//             // clicked, this event is routed to the FSM for game logic
//             // crunching.
//             enm.each<SpriteComponent>([&](auto e, auto& sprite) {
//                 auto bounds = sprite.sprite.getTextureRect();
//                 if(bounds.contains(evt.position.x, evt.position.y)) {
//                     evm.emit<EntityActivationEvent>(e);
//                 }
//             });
//             m_clicks.pop_front();
//         }
//     }

//     void receive(const MouseClickEvent &evt) {
//         m_clicks.push_back(evt);
//     }
// private:
//     std::deque<MouseClickEvent> m_clicks;
// };
// } // namespace GUI
// } // namespace MTG

int main() {
    // using MTG::GameFsm;
    // using MTG::GUI::MouseClickEvent;
    // using MTG::GUI::MouseClickSystem;
    // using MTG::GUI::RenderSystem;

    // entityx::EntityX  ex;
    // GameFsm  fsm{ex.entities, ex.events};
    
    // sf::RenderWindow window(sf::VideoMode(1024,768), "Magic The Gathering");
    // ex.systems.add<MouseClickSystem>();
    // ex.systems.add<RenderSystem>(window);
    // ex.systems.configure();

    // for(auto i = 0 ; i < 5 ; ++i ) {
    //     //auto e = makeAeronautAdmiral(ex);    
    //     //e.assign<MTG::GUI::SpriteComponent>("AeronautAdmiral.jpg");
    //     //e.assign<ZoneComponent>()->zone = MTG::Constants::Zone::HAND;
    // }
    
    // while(window.isOpen()) {
    //     sf::Event evt;
    //     while(window.pollEvent(evt)) {
    //         switch(evt.type) {
    //         case sf::Event::Closed:
    //             window.close();
    //             break;
    //         case sf::Event::MouseButtonPressed:
    //             ex.events.emit<MouseClickEvent>({{
    //                 evt.mouseButton.x,
    //                 evt.mouseButton.y
    //             }});
    //             break;
    //         default:
    //             break;
    //         }
    //     }
    //     ex.systems.update_all(1.0);        
    // }
    
    // // fsm.dispatch(MTG::Event{ex});
    
    
    
    // // ex.entities.each<CreatureComponent>([](auto e, auto& creature) {
    // //         std::cout << "T: " << (int)creature.toughness << '\n';
    // // });
}
