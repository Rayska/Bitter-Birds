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
    t(0.f),
    menu_state_(MenuState::MainMenu),
    menu_scroll_y_(0.f) {
    levels_ = {
        { "World 1" },
        { "World 2" },
        { "World 3" },
        { "World 4" },
        { "World 5" }
    };
}

MenuScene::~MenuScene() {
}

void MenuScene::update(float ts) {
    t += ts;

    gui_.setViewport(0.5f, 0.5f, 1.f, 1.f);


    switch(menu_state_){
        case MenuState::MainMenu:
        {
            if(gui_.drawButton("Play", 0.5f, 0.6f, 0.5f, 0.2f)){
                menu_state_ = MenuState::LevelSelector;
                target_menu_scroll_y_ = 0.f;
            }

            if(gui_.drawButton("Exit", 0.5f, 0.3f, 0.5f, 0.2f)){
                gui_.close();
            }

            break;
        }
        case MenuState::LevelSelector:
        {
            target_menu_scroll_y_ -= gui_.scrollDelta() * 0.1f;
            std::cout << target_menu_scroll_y_ << std::endl;
            if(target_menu_scroll_y_ < 0.f) 
                target_menu_scroll_y_ = 0.f;
            if(target_menu_scroll_y_ > 1.f)
                target_menu_scroll_y_ = 1.f;
            menu_scroll_y_ += (target_menu_scroll_y_ - menu_scroll_y_) * 0.2;

            float y = 0.8f;
            for(auto& level : levels_){
                if(gui_.drawButton(level.name, 0.5f, y + menu_scroll_y_, 0.4f, 0.1f)){
                    Level lvl({
                        std::make_shared<Enemy>(100, 0.f, 10.f, 2.f, 0),
                    }, {}, {}, {}, {}, "Level 1");
                    gui_.setScene<PlayScene>(lvl);
                }
                y -= 0.2f;
            }

            if(gui_.drawButton("Back", 0.1f, 0.9f, 0.05f, 0.05f)){
                menu_state_ = MenuState::MainMenu;
            }
            break;
        }
    }
}
