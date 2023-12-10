#include "entity.hpp"

Entity::Entity(bool movable, bool destructible, int healthPoints,
    double initRotation, double x, double y) : 
    movable_(movable), destructible_(destructible), healthPoints_(healthPoints),
    initRotation_(initRotation), x_(x), y_(y) {}

bool Entity::is_movable() const {
        return movable_;
}

bool Entity::is_destructible() const {
    return destructible_;
}

int Entity::get_health_points() const {
    return healthPoints_;
}

double Entity::get_rotation() const {
    return initRotation_;
}

double Entity::get_x() const {
        return x_;
}

double Entity::get_y() const {
        return y_;
}