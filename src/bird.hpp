#pragma once

#include "image.hpp"

enum struct birdType {
    normal,
    special1,
    special2
};

class Bird {
public:
    Bird();

    ~Bird();

    virtual birdType get_bird_type() const = 0;

    void resetTime();

    sf::Time getTime() const;

protected:
    sf::Clock spawnTime;
};

class NormalBird: public Bird {
public:
    NormalBird();

    birdType get_bird_type() const override;
private:
};

class SpecialBird1: public Bird {
public:
    SpecialBird1();

    birdType get_bird_type() const override;
};

class SpecialBird2: public Bird {
public:
    SpecialBird2();

    birdType get_bird_type() const override;
};