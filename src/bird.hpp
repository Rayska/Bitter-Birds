#pragma once

#include "image.hpp"

enum struct birdType {
    normal,
    special1,
    special2
};

class Bird {
public:
    Bird(Image* image): image_(image) {}

    virtual double getDiameter() const = 0;

    ~Bird() {
        delete image_;
    }

    virtual birdType getBirdType() const = 0;

    Image* getImage() const {
        return image_;
    }

    void resetTime() {
        spawnTime.restart();
    }

    sf::Time getTime() const {
        return spawnTime.getElapsedTime();
    }

protected:
    Image*  image_;
    sf::Clock spawnTime;
};

class NormalBird: public Bird {
public:
    NormalBird(): Bird(new Image("res/test_bird.png")) {}

    birdType getBirdType() const {
        return birdType::normal;
    }

    double getDiameter() const {
        return 10;
    }

private:
};

class SpecialBird1: public Bird {
public:
    SpecialBird1(): Bird(new Image("res/test_specialbird1.png")) {}

    birdType getBirdType() const {
        return birdType::special1;
    }

    double getDiameter() const {
        return 10;
    }
};

class SpecialBird2: public Bird {
public:
    SpecialBird2(): Bird(new Image("res/test_specialbird2.png")) {}

    birdType getBirdType() const {
        return birdType::special2;
    }

    double getDiameter() const {
        return 10;
    }
};