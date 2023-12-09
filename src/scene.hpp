#pragma once

#include "GUI.hpp"

/**
 * @brief Base Scene class to represent different states the game can be in.
 * Ex. Play state can be implemented with PlayScene.
 * Menu can be implemented with MenuScene etc.
 */
class Scene {
public:
    /**
     * @brief Construct a new Scene 
     * 
     * @param gui Reference to GUI
     */
    Scene(GUI& gui);
    /**
     * @brief Destroy the Scene object
     */
    virtual ~Scene();

    /**
     * @brief Called on an interval to update state of scene 
     * 
     * @param ts Timestep since last update
     */
    virtual void update(float ts) = 0;
    /**
     * @brief Called when input has been entered and should be passed to scene
     * 
     * @param c Character
     */
    inline virtual void on_input(char c) {}
protected:
    GUI& gui_;
};