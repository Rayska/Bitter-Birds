#pragma once

#include "GUI.hpp"
#include "scene.hpp"
#include "image.hpp"
#include "ReaderWriter.hpp"

enum struct MenuState {
    MainMenu,
    LevelSelector
};

class MenuScene : public Scene {
public:
    MenuScene(GUI& gui, std::string current_player);
    ~MenuScene();

    void update(float ts) override;
private:
    ReaderWriter reader_writer_;
    std::vector<LevelInfo> levels_;
    MenuState menu_state_;
    Image button_image_, bird_image_;
    float t;
    float target_menu_scroll_y_, menu_scroll_y_;
    std::string current_player_;
};