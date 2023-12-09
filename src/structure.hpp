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
    inline Structure(int healthPoints, double initRotation, double x, double y, double height, double width)
        :
        Entity(true, true, healthPoints, initRotation, x, y),
        width_(width), 
        height_(height)
    {}

    /**
     * @brief Get the type object
     * 
     * @return bodyType::structure
     */
    inline bodyType get_type() const override {
        return bodyType::structure;
    }

    /**
     * @brief Get the width of structure
     * 
     * @return double Width
     */
    inline double get_width() const {
        return width_;
    }

    /**
     * @brief Get the height the strucutre
     * 
     * @return double Height
     */
    inline double get_height() const {
        return height_;
    }

    /**
     * @brief Returns true if contains the coordinate passed
     * 
     * @param x X-coord
     * @param y Y-coord
     * @return true Contained
     * @return false Not contained
     */
    inline bool contains(double x, double y) {
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