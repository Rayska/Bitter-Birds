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

    bodyType get_type() const override {
        return bodyType::structure;
    }

    double get_width() const {
        return width_;
    }

    double get_height() const {
        return height_;
    }

    bool contains(double x, double y) {
        return ( 
            x <= this->get_x() + this->get_width() * 0.5
            && x >= this->get_x() - this->get_width() * 0.5
            && y <= this->get_y() + this->get_height() * 0.5
            && y >= this->get_y() - this->get_height() * 0.5
        );
    }

private:
    double height_;
    double width_;
};