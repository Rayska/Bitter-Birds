#pragma once

enum struct bodyType {
    structure,
    enemy,
    ground,
    bird
};

class Entity {
public:
    Entity(bool movable, bool destructible, int healthPoints,
    double initRotation, double x, double y) : 
    movable_(movable), destructible_(destructible), healthPoints_(healthPoints),
    initRotation_(initRotation), x_(x), y_(y) {}

    virtual bodyType get_type() const = 0;

    bool is_movable() const {
        return movable_;
    }

    bool is_destructible() const {
        return destructible_;
    }

    int get_health_points() const {
        return healthPoints_;
    }

    double get_rotation() const {
        return initRotation_;
    }

    double get_x() const {
        return x_;
    }

    double get_y() const {
        return y_;
    }

    virtual bool contains(double x, double y) = 0;

private:
    bool movable_;
    bool destructible_;
    int healthPoints_;
    double initRotation_;
    double x_;
    double y_;
};