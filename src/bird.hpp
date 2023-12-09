#pragma once

#include "image.hpp"

/**
 * @brief Enumeration of all different bird types
 */
enum struct birdType {
    normal,
    special1,
    special2
};

/**
 * @brief Base class for Bird objects 
 */
class Bird {
public:
    /**
     * @brief Construct a new Bird object
     */
    Bird();
    /**
     * @brief Destroy the Bird object
     */
    ~Bird();

    /**
     * @brief Get the bird type object
     * @return birdType 
     */
    virtual birdType get_bird_type() const = 0;

    /**
     * @brief Reset spawn timer
     */
    void resetTime();

    /**
     * @brief Get the spawn time object
     * @return sf::Time 
     */
    sf::Time getTime() const;
protected:
    sf::Clock spawnTime;
};

/**
 * @brief Normal bird class
 * No special effects
 */
class NormalBird: public Bird {
public:
    /**
     * @brief Construct a new Normal Bird object
     */
    NormalBird();

    /**
     * @brief Get the bird type object
     * @return birdType::normal 
     */
    birdType get_bird_type() const override;
private:
};

/**
 * @brief Special Bird 1 class
 * Has speed up effect
 */
class SpecialBird1: public Bird {
public:
    /**
     * @brief Construct a new Special Bird 1 object
     */
    SpecialBird1();

    /**
     * @brief Get the bird type object
     * @return birdType::special1 
     */
    birdType get_bird_type() const override;
};

/**
 * @brief Special Bird 2 class
 * Has explosion effect
 */
class SpecialBird2: public Bird {
public:
    /**
     * @brief Construct a new Special Bird 2 object
     */
    SpecialBird2();

    /**
     * @brief Get the bird type object
     * @return birdType::special2 
     */
    birdType get_bird_type() const override;
};