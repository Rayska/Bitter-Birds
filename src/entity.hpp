#pragma once

enum struct entityType {
    structure,
    enemy,
    ground,
    bedrock,
    bird
};

class Entity {
public:
    Entity(bool movable, bool destructible, int healthPoints,
    double initRotation, double x, double y) : 
    movable_(movable), destructible_(destructible), healthPoints_(healthPoints),
    initRotation_(initRotation), x_(x), y_(y) {}

    virtual entityType getType() const = 0;

    bool isMovable() const {
        return movable_;
    }

    bool isDestructible() const {
        return destructible_;
    }

    int getHealthPoints() const {
        return healthPoints_;
    }

    double getRotation() const {
        return initRotation_;
    }

    double getX() const {
        return x_;
    }

    double getY() const {
        return y_;
    }

private:
    bool movable_;
    bool destructible_;
    int healthPoints_;
    double initRotation_;
    double x_;
    double y_;
};