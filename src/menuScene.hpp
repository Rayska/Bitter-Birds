#pragma once

#include "GUI.hpp"
#include "scene.hpp"
#include "image.hpp"
#include "readerWriter.hpp"

/**
 * @brief The different states a user can be in the main menu
 * 
 */
enum struct MenuState {
    MainMenu,
    PlayLevelSelector,
    EditorLevelSelector,
    LevelSelector,
    Help
};

/**
 * @brief Scene to handle the main menu
 */
class MenuScene : public Scene {
public:
    /**
     * @brief Construct a new Menu Scene
     * 
     * @param gui Reference to GUI
     * @param current_player Name of current player
     */
    MenuScene(GUI& gui, std::string current_player);
    
    /**
     * @brief Destroy the Menu Scene object
     */
    ~MenuScene();

    /**
     * @brief Update everything required in the main menu
     * 
     * @param ts Timestep
     */
    void update(float ts) override;
private:
    ReaderWriter reader_writer_;
    std::vector<LevelInfo> levels_;
    MenuState menu_state_;
    Image bird_image_;
    float t_;
    float target_menu_scroll_y_, menu_scroll_y_;
    std::string current_player_;
};