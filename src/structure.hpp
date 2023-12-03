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

private:
    double height_;
    double width_;
};