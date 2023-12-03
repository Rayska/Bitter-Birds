#include "menuScene.hpp"

#include <iostream>
#include <math.h>
#include <unordered_map>
#include <functional>
#include <SFML/Audio.hpp>

#include "playScene.hpp"
#include "level.hpp"
#include "ground.hpp"
#include "enemy.hpp"



MenuScene::MenuScene(GUI& gui, std::string current_player) 
    : 
    Scene(gui), 
    button_image_("res/button.png"), 
    bird_image_("res/test_bird.png"), 
    t(0.f),
    menu_state_(MenuState::MainMenu),
    current_player_(current_player),
    menu_scroll_y_(0.f)
{
    levels_ = reader_writer_.getLevels();
}

MenuScene::~MenuScene() {}

void MenuScene::update(float ts) {
    t += ts;
    gui_.setViewport(0.5f, 0.5f, 1.f, 1.f);


    switch(menu_state_){
        case MenuState::MainMenu:
        {
            if(gui_.drawButton("Play", 0.5f, 0.6f, 0.5f, 0.2f)){
                gui_.playSound("res/sounds/click_sound.wav");
                menu_state_ = MenuState::LevelSelector;
                target_menu_scroll_y_ = 0.f;
            }

            if(gui_.drawButton("Exit", 0.5f, 0.3f, 0.5f, 0.2f)){
                gui_.playSound("res/sounds/click_sound.wav");
                gui_.close();
            }

            break;
        }
        case MenuState::LevelSelector:
        {
            target_menu_scroll_y_ -= gui_.scrollDelta() * 0.1f;
            if(target_menu_scroll_y_ < 0.f) 
                target_menu_scroll_y_ = 0.f;
            if(target_menu_scroll_y_ > 1.f)
                target_menu_scroll_y_ = 1.f;
            menu_scroll_y_ += (target_menu_scroll_y_ - menu_scroll_y_) * 0.2;

            float y = 0.8f;
            for(auto& level : levels_){
                if(gui_.drawButton(level.name, 0.5f, y + menu_scroll_y_, 0.4f, 0.1f)){
                    gui_.playSound("res/sounds/click_sound.wav");
                    auto loaded_level = reader_writer_.readFile(level.path);
                    if(loaded_level){
                        gui_.setScene<PlayScene>(*loaded_level, current_player_);
                    }
                    else{
                        std::cout << "Unable to load level." << std::endl;
                    }
                }
                y -= 0.2f;
            }

            if(gui_.drawButton("Back", 0.1f, 0.9f, 0.05f, 0.05f)){
                gui_.playSound("res/sounds/click_sound.wav");
                menu_state_ = MenuState::MainMenu;
            }
            break;
        }
    }
}
