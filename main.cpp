#include "entityx/entityx.h"

#include "SFML/Window.hpp"

#include "MTG/cards/cards.hpp"
#include "MTG/system/system.hpp"

#include <iostream>

int main() {
    using namespace MTG::cards;
    using namespace entityx;
    using MTG::Component::CreatureComponent;
    using MTG::Component::EnchantCreatureComponent;

    sf::Window window(sf::VideoMode(800,600), "Magic The Gathering");

    while(window.isOpen()) {
        sf::Event evt;
        while(window.pollEvent(evt)) {
            if(evt.type == sf::Event::Closed) {
                window.close();
            }
        }
    }
    
    
    EntityX ex;

    auto e = makeAeronautAdmiral(ex);
    
    ex.entities.each<CreatureComponent>([](auto e, auto& creature) {
            std::cout << "T: " << (int)creature.toughness << '\n';
    });
    
    e.destroy();
    
    
    std::cout << "EntityX " << sizeof(e) << '\n';    
}
