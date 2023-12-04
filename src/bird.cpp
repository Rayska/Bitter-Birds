#include "bird.hpp"


Bird::Bird()
{}

Bird::~Bird() {}


void Bird::resetTime() {
    spawnTime.restart();
}

sf::Time Bird::getTime() const {
    return spawnTime.getElapsedTime();
}

NormalBird::NormalBird()
    : 
    Bird() 
{}

birdType NormalBird::get_bird_type() const {
    return birdType::normal;
}


SpecialBird1::SpecialBird1()
    : 
    Bird() 
{}

birdType SpecialBird1::get_bird_type() const {
    return birdType::special1;
}


SpecialBird2::SpecialBird2()
    : 
    Bird() 
{}

birdType SpecialBird2::get_bird_type() const {
    return birdType::special2;
}
