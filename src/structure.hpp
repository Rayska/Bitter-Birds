#pragma once

#include "entity.hpp"

class Structure: public Entity {
public:
    Structure(int healthPoints, double initRotation, double x, double y, int height, int width):
    width_(width), height_(height), Entity(true, true, healthPoints, initRotation, x, y) {}

    double getWidth() const {
        return width_;
    }

    double getHeight() const {
        return width_;
    }

private:
    int height_;
    int width_;
};