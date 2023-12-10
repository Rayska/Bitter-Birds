#include "enemy.hpp"

Enemy::Enemy(int healthPoints, double initRotation, double x, double y):
        Entity(true, true, healthPoints, initRotation, x, y) {}

bodyType Enemy::get_type() const {
    return bodyType::enemy;
}

bool Enemy::contains(double x, double y) {
    return ( x < this->get_x() + 0.5 && x > this->get_x() - 0.5 && y < this->get_y() + 0.5 && y > this->get_y() - 0.5 );
}