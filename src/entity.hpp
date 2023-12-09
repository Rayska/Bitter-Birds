#pragma once

/**
 * @brief Type of entity
 */
enum struct bodyType {
    structure,
    enemy,
    ground,
    bird
};

/**
 * @brief Entity class to handle entity data loaded from levels
 */
class Entity {
public:
    /**
     * @brief Construct a new Entity object
     * 
     * @param movable 
     * @param destructible 
     * @param healthPoints 
     * @param initRotation 
     * @param x 
     * @param y 
     */
    Entity(bool movable, bool destructible, int healthPoints,
    double initRotation, double x, double y) : 
    movable_(movable), destructible_(destructible), healthPoints_(healthPoints),
    initRotation_(initRotation), x_(x), y_(y) {}

    /**
     * @brief Get the type object
     * @return bodyType 
     */
    virtual bodyType get_type() const = 0;

    /**
     * @brief Is entity movable
     * 
     * @return true 
     * @return false 
     */
    bool is_movable() const {
        return movable_;
    }

    /**
     * @brief Is entity destructible
     * 
     * @return true 
     * @return false 
     */
    bool is_destructible() const {
        return destructible_;
    }

    /**
     * @brief Get the initial health points
     * 
     * @return int 
     */
    int get_health_points() const {
        return healthPoints_;
    }

    /**
     * @brief Get the initial rotation
     * 
     * @return double 
     */
    double get_rotation() const {
        return initRotation_;
    }

    /**
     * @brief Get the initial x pos
     * 
     * @return double 
     */
    double get_x() const {
        return x_;
    }

    /**
     * @brief Get the initial y pos
     * 
     * @return double 
     */
    double get_y() const {
        return y_;
    }

    /**
     * @brief Is a point contained inside the entity
     * 
     * @param x 
     * @param y 
     * @return true Is contained
     * @return false Not contained
     */
    virtual bool contains(double x, double y) = 0;

private:
    bool movable_;
    bool destructible_;
    int healthPoints_;
    double initRotation_;
    double x_;
    double y_;
};