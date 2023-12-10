#pragma once

#include <iostream>

#include "entity.hpp"

/**
 * @brief Structure type to represent walls in the game
 */
class Structure: public Entity {
public:
    /**
     * @brief Construct a new Structure
     * 
     * @param healthPoints HP
     * @param initRotation Rotation
     * @param x X position
     * @param y Y position
     * @param height Height of structure
     * @param width Width of structure
     */
    Structure(int healthPoints, double initRotation, double x, double y, double height, double width);

    /**
     * @brief Get the type object
     * 
     * @return bodyType::structure
     */
    bodyType get_type() const override;

    /**
     * @brief Get the width of structure
     * 
     * @return double Width
     */
    double get_width() const;

    /**
     * @brief Get the height the strucutre
     * 
     * @return double Height
     */
    double get_height() const;

    /**
     * @brief Returns true if contains the coordinate passed
     * 
     * @param x X-coord
     * @param y Y-coord
     * @return true Contained
     * @return false Not contained
     */
    bool contains(double x, double y);

private:
    double height_;
    double width_;
};