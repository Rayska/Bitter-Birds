#pragma once

#include "GUI.hpp"
#include "scene.hpp"
#include "image.hpp"

class MenuScene : public Scene {
public:
    MenuScene(GUI& gui);
    ~MenuScene();

    void update(float ts) override;
private:
    Image button_image_, bird_image_;
    float t;
};