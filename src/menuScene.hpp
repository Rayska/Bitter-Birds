#pragma once

#include "GUI.hpp"
#include "scene.hpp"
#include "image.hpp"
#include "readerWriter.hpp"

enum struct MenuState {
    MainMenu,
    PlayLevelSelector,
    EditorLevelSelector,
    LevelSelector,
    Help
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
    Image bird_image_;
    float t_;
    float target_menu_scroll_y_, menu_scroll_y_;
    std::string current_player_;
};