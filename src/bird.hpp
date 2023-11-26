#pragma once

#include "image.hpp"

class Bird {
public:
    Bird(Image* image): image_(image) {}

    virtual double getDiameter() const = 0;

    ~Bird() {
        delete image_;
    }

    Image* getImage() const {
        return image_;
    }

protected:
    Image*  image_;
};

class NormalBird: public Bird {
public:
    NormalBird(): Bird(new Image("res/test_bird.png")) {}

    double getDiameter() const {
        return 10;
    }

private:
};

class SpecialBird1: public Bird {
public:
    SpecialBird1(): Bird(new Image("res/test_specialbird1.png")) {}

    double getDiameter() const {
        return 10;
    }
};

class SpecialBird2: public Bird {
public:
    SpecialBird2(): Bird(new Image("res/test_specialbird2.png")) {}

    double getDiameter() const {
        return 10;
    }
};