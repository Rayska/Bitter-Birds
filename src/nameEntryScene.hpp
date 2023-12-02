#pragma once

#include "GUI.hpp"
#include "scene.hpp"

#include <string>

class NameEntryScene : public Scene {
public:
    NameEntryScene(GUI& gui);
    ~NameEntryScene();

    void update(float ts) override;
    void on_input(char c) override;
private:
    std::string current_player_;
};