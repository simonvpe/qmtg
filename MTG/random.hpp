#pragma once
#include <random>

namespace MTG {

class RandomGenerator {
public:
    RandomGenerator() : m_gen{m_rd()} {}

    virtual void shuffle(gsl::span<const char*> container) {
        std::shuffle(container.begin(), container.end(), m_gen);   
    }

    virtual void shuffle(gsl::span<PlayerHandle> container) {
        std::shuffle(container.begin(), container.end(), m_gen);   
    }
    
private:
    std::random_device m_rd;
    std::mt19937       m_gen;
};
    
using pRandomGenerator = std::unique_ptr<RandomGenerator>;
    
}
