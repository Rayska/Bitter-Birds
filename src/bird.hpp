#pragma once

#include "image.hpp"

enum struct birdType {
    normal,
    special1,
    special2
};

class Bird {
public:
    Bird(Image* image);

    ~Bird();

    virtual birdType getBirdType() const = 0;

    Image* getImage() const;

    void resetTime();

    sf::Time getTime() const;

protected:
    Image*  image_;
    sf::Clock spawnTime;
};

class NormalBird: public Bird {
public:
    NormalBird();

    birdType getBirdType() const override;
private:
};

class SpecialBird1: public Bird {
public:
    SpecialBird1();

    birdType getBirdType() const override;
};

class SpecialBird2: public Bird {
public:
    SpecialBird2();

    birdType getBirdType() const override;
};