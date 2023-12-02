#pragma once

#include "GUI.hpp"

class Scene {
public:
    Scene(GUI& gui);
    virtual ~Scene();

    virtual void update(float ts) = 0;
    inline virtual void on_input(char c) {}
protected:
    GUI& gui_;
};