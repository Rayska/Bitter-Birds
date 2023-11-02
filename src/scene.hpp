#pragma once

#include "GUI.hpp"

class Scene {
public:
    Scene(GUI& gui);
    virtual ~Scene();

    virtual void update(float ts) = 0;
protected:
    GUI& gui_;
};