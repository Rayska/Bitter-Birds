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
    Image test_;
    float t;
};