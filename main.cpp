#include "entityx/entityx.h"

#include "SFML/Window.hpp"

#include "MTG/cards/cards.hpp"
#include "MTG/system/system.hpp"

#include <iostream>

int main() {

    sf::Window window(sf::VideoMode(800,600), "Magic The Gathering");

    while(window.isOpen()) {
        sf::Event evt;
        while(window.pollEvent(evt)) {
            if(evt.type == sf::Event::Closed) {
                window.close();
            }
        }
    }
    
    using namespace MTG;
    using namespace entityx;
    using MTG::Component::CreatureComponent;
    using MTG::Component::EnchantCreatureComponent;
    
    EntityX ex;

    auto e = MTG::cards::makeAeronautAdmiral(ex);
    
    ex.entities.each<CreatureComponent>([](auto e, auto& creature) {
            std::cout << "T: " << (int)creature.toughness << '\n';
    });
    
    e.destroy();
    
    
    std::cout << "EntityX " << sizeof(e) << '\n';    
}
