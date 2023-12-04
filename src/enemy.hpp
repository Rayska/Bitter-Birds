#pragma once

#include "entity.hpp"

class Enemy: public Entity {
public:
    Enemy(int healthPoints, double initRotation, double x, double y, int type):
    type_(type), Entity(true, true, healthPoints, initRotation, x, y) {}

    bodyType get_type() const override {
        return bodyType::enemy;
    }

    bool contains(double x, double y) {
        return ( x < this->get_x() + 0.5 && x > this->get_x() - 0.5 && y < this->get_y() + 0.5 && y > this->get_y() - 0.5 );
    }

private:
    int type_;
};