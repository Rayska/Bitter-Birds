#include "structure.hpp"

Structure::Structure(int healthPoints, double initRotation, double x, double y, double height, double width)
        :
        Entity(true, true, healthPoints, initRotation, x, y),
        width_(width), 
        height_(height)
    {}

bodyType Structure::get_type() const {
    return bodyType::structure;
}

double Structure::get_width() const {
        return width_;
}

double Structure::get_height() const {
    return height_;
}

bool Structure::contains(double x, double y) {
    return ( 
        x <= this->get_x() + this->get_width() * 0.5
        && x >= this->get_x() - this->get_width() * 0.5
        && y <= this->get_y() + this->get_height() * 0.5
        && y >= this->get_y() - this->get_height() * 0.5
    );
}