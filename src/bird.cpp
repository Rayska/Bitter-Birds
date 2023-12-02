#include "bird.hpp"


Bird::Bird(Image* image)
    : image_(image) 
{}

Bird::~Bird() {
    delete image_;
}

Image* Bird::getImage() const {
    return image_;
}

void Bird::resetTime() {
    spawnTime.restart();
}

sf::Time Bird::getTime() const {
    return spawnTime.getElapsedTime();
}

NormalBird::NormalBird()
    : 
    Bird(new Image("res/test_bird.png")) 
{}

birdType NormalBird::getBirdType() const {
    return birdType::normal;
}

double NormalBird::getDiameter() const {
    return 10;
}

SpecialBird1::SpecialBird1()
    : 
    Bird(new Image("res/test_specialbird1.png")) 
{}

birdType SpecialBird1::getBirdType() const {
    return birdType::special1;
}

double SpecialBird1::getDiameter() const {
    return 10;
}

SpecialBird2::SpecialBird2()
    : 
    Bird(new Image("res/test_specialbird2.png")) 
{}

birdType SpecialBird2::getBirdType() const {
    return birdType::special2;
}

double SpecialBird2::getDiameter() const {
    return 10;
}