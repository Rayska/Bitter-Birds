#include "menuScene.hpp"

#include <iostream>
#include <math.h>
#include <unordered_map>
#include <functional>

#include "playScene.hpp"
#include "level.hpp"
#include "ground.hpp"
#include "enemy.hpp"

MenuScene::MenuScene(GUI& gui) 
    : 
    Scene(gui), 
    button_image_("res/button.png"), 
    bird_image_("res/test_bird.png"), 
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

    gui_.setViewport(0.5f, 0.5f, 1.f, 1.f);

    if(gui_.drawButton("test", 0.5f, 0.5f, 0.5f, 0.2f, button_image_)){
        std::cout << "TEST BUTTON" << std::endl;
        Level lvl({
            std::make_shared<Enemy>(100, 0.f, 10.f, 2.f, 0),
        }, {}, {}, {}, {}, "Level 1");
        gui_.setScene<PlayScene>(lvl);
    }

    gui_.drawSprite(0.5f * (sin(t) + 1.f), 0.2f, 0.2f, 0.2f, 0, bird_image_);
}
