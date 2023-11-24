#pragma once

#include "entity.hpp"

class Enemy: public Entity {
public:
    Enemy(int healthPoints, double initRotation, double x, double y, int type):
    type_(type), Entity(true, true, healthPoints, initRotation, x, y) {}

    bodyType getType() const override {
        return bodyType::enemy;
    }

    double getDiameter() const {
        switch (type_) {
            case 0:
                return 10;
            default:
                return 0;
        }
    }

private:
    int type_;
};