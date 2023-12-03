#pragma once

#include "entity.hpp"

class Enemy: public Entity {
public:
    Enemy(int healthPoints, double initRotation, double x, double y, int type):
    type_(type), Entity(true, true, healthPoints, initRotation, x, y) {}

    bodyType getType() const override {
        return bodyType::enemy;
    }

    bool contains(double x, double y) {
        return ( x < this->getX() + 0.5 && x > this->getX() - 0.5 && y < this->getY() + 0.5 && y > this->getY() - 0.5 );
    }

private:
    int type_;
};