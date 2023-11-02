#include "menuScene.hpp"

#include <iostream>
#include <math.h>
#include <unordered_map>
#include <functional>

MenuScene::MenuScene(GUI& gui) 
    : 
    Scene(gui), 
    test_("res/test.png"), 
    t(0.f) {
}

MenuScene::~MenuScene() {
}

void MenuScene::update(float ts) {
    t += ts;

    // auto [x, y] = gui_.cursorPosition();
    // std::cout << "Cursor pos = " << x << " " << y << std::endl;

    // if(gui_.buttonState(sf::Mouse::Button::Left))
    //     std::cout << "LMB pressed" << std::endl;


    if(gui_.drawButton("test", 0.5f, 0.5f, 0.5f, 0.2f)){
        std::cout << "TEST BUTTON" << std::endl;
    }

    // gui_.drawSprite(0.5f * (sin(t) + 1.f), 0.2f, 0.2f, 0.2f, 0, test_);
    gui_.drawSprite(0.5f, 0.5f, 0.5f, 0.2f, 0, test_);
}
