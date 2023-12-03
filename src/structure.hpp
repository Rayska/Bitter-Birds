#pragma once

#include <iostream>

#include "entity.hpp"

class Structure: public Entity {
public:
    Structure(int healthPoints, double initRotation, double x, double y, double height, double width)
        :
        Entity(true, true, healthPoints, initRotation, x, y),
        width_(width), 
        height_(height)
    {}

    bodyType getType() const override {
        return bodyType::structure;
    }

    double getWidth() const {
        return width_;
    }

    double getHeight() const {
        return height_;
    }

    bool contains(double x, double y) {
        return ( 
            x <= this->getX() + this->getWidth() * 0.5
            && x >= this->getX() - this->getWidth() * 0.5
            && y <= this->getY() + this->getHeight() * 0.5
            && y >= this->getY() - this->getHeight() * 0.5
        );
    }

private:
    double height_;
    double width_;
};