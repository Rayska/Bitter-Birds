#pragma once

#include "entity.hpp"
/**
 * @brief 
 * 
 */
class Enemy: public Entity {
public:
    /**
     * @brief Construct a new Enemy object
     * 
     * @param healthPoints  Initial HP of this object
     * @param initRotation  Initial rotation of this object
     * @param x             x-component of this object's location
     * @param y             y-component of this object's location
     */
    Enemy(int healthPoints, double initRotation, double x, double y);

    /**
     * @brief Get the type object, in this case bodyType::Enemy 
     * @return bodyType 
     */
    bodyType get_type() const override;

    /**
     * @brief 
     * 
     * @param x 
     * @param y 
     * @return bool
     */
    bool contains(double x, double y);
};