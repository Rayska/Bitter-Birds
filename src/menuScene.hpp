#pragma once

#include "GUI.hpp"
#include "scene.hpp"
#include "image.hpp"

enum struct MenuState {
    MainMenu,
    LevelSelector
};

struct LevelInfo {
    std::string name;
};

class MenuScene : public Scene {
public:
    MenuScene(GUI& gui);
    ~MenuScene();

    void update(float ts) override;
private:
    MenuState menu_state_;
    Image button_image_, bird_image_;
    float t;
    std::vector<LevelInfo> levels_;
    float target_menu_scroll_y_, menu_scroll_y_;
};