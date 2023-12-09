#pragma once

#include "GUI.hpp"
#include "scene.hpp"

#include <string>

/**
 * @brief A scene for the user to enter their name from
 */
class NameEntryScene : public Scene {
public:
    /**
     * @brief Construct a new Name Entry Scene object
     * 
     * @param gui Reference to GUI
     */
    NameEntryScene(GUI& gui);
    /**
     * @brief Destroy the Name Entry Scene object
     */
    ~NameEntryScene();

    /**
     * @brief Function to update required
     * 
     * @param ts Timestep
     */
    void update(float ts) override;
    /**
     * @brief Handle input events 
     * 
     * @param c Character entered
     */
    void on_input(char c) override;
private:
    std::string current_player_;
};