#include "menuScene.hpp"

#include <iostream>
#include <math.h>
#include <unordered_map>
#include <functional>
#include <SFML/Audio.hpp>

#include "levelEditorScene.hpp"
#include "playScene.hpp"
#include "level.hpp"
#include "enemy.hpp"

MenuScene::MenuScene(GUI& gui, std::string current_player) 
    : 
    Scene(gui), 
    bird_image_("res/normal_bird.png"), 
    menu_state_(MenuState::MainMenu),
    current_player_(current_player),
    menu_scroll_y_(0.f)
{
    levels_ = reader_writer_.get_levels();
}

MenuScene::~MenuScene() {}

void MenuScene::update(float ts) 
{}

void MenuScene::render() {
    gui_.set_viewport(0.5f, 0.5f, 1.f, 1.f);

    switch(menu_state_){
        case MenuState::MainMenu:
        {
            gui_.draw_text(0.5f, 0.8f, 0.25f, "Bitter Birds");

            if(gui_.draw_button("Play", 0.5f, 0.6f, 0.5f, 0.05f)){
                gui_.play_sound("res/sounds/click_sound.wav");
                menu_state_ = MenuState::PlayLevelSelector;
                target_menu_scroll_y_ = 0.f;
            }

            if(gui_.draw_button("Level Editor", 0.5f, 0.5f, 0.5f, 0.05f)){
                gui_.play_sound("res/sounds/click_sound.wav");
                menu_state_ = MenuState::EditorLevelSelector;
                target_menu_scroll_y_ = 0.f;
            }

            if(gui_.draw_button("Help", 0.5f, 0.4f, 0.5f, 0.05f)){
                gui_.play_sound("res/sounds/click_sound.wav");
                menu_state_ = MenuState::Help;
            }    

            if(gui_.draw_button("Exit", 0.5f, 0.3f, 0.5f, 0.05f)){
                gui_.play_sound("res/sounds/click_sound.wav");
                gui_.close();
            }

            break;
        }
        case MenuState::PlayLevelSelector:
        {
            target_menu_scroll_y_ -= gui_.scroll_delta() * 0.1f;
            if(target_menu_scroll_y_ < 0.f) 
                target_menu_scroll_y_ = 0.f;
            if(target_menu_scroll_y_ > 1.f)
                target_menu_scroll_y_ = 1.f;
            menu_scroll_y_ += (target_menu_scroll_y_ - menu_scroll_y_) * 0.2;

            float y = 0.8f;
            for(auto& level : levels_){
                if(gui_.draw_button(level.name, 0.5f, y + menu_scroll_y_, 0.4f, 0.1f)){
                    gui_.play_sound("res/sounds/click_sound.wav");
                    auto loaded_level = reader_writer_.read_file(level.path);
                    if(loaded_level){
                        gui_.set_scene<PlayScene>(*loaded_level, current_player_);
                    }
                    else{
                        std::cout << "Unable to load level." << std::endl;
                    }
                }
                y -= 0.2f;
            }

            if(gui_.draw_button("Back", 0.1f, 0.9f, 0.05f, 0.05f)){
                gui_.play_sound("res/sounds/click_sound.wav");
                menu_state_ = MenuState::MainMenu;
            }

            break;
        }
        case MenuState::Help:
        {
            std::vector<std::string> help_text{
                "To play the game select a level from the level selection screen. ",
                "In the game, drag from the bird in the slingshot to",
                "launch it towards the enemy structures.",
                "You gain points from destroying structures and enemies.",
                "Camera can be moved by using WASD",
                "Disable camera following by using left click.",
                "",
                "Created by",
                "Luukas Karihtala",
                "Ray Sointula",
                "Onni Komulainen",
                "Daniel Granstrom"
            };

            for(int i = 0; i < help_text.size(); i++){
                gui_.draw_text(0.5f, 0.8f - i * 0.06f, 0.06f, help_text[i]);
            }
            // ö dots for name
            gui_.draw_rect(0.574f, 0.146f, 0.002f, 0.002f, 0.f, {1.f, 1.f, 1.f});
            gui_.draw_rect(0.578f, 0.146f, 0.002f, 0.002f, 0.f, {1.f, 1.f, 1.f});

            if(gui_.draw_button("Back", 0.1f, 0.9f, 0.05f, 0.05f)){
                gui_.play_sound("res/sounds/click_sound.wav");
                menu_state_ = MenuState::MainMenu;
            }

            break;
        }
        case MenuState::EditorLevelSelector:
        {
            target_menu_scroll_y_ -= gui_.scroll_delta() * 0.1f;
            if(target_menu_scroll_y_ < 0.f) 
                target_menu_scroll_y_ = 0.f;
            if(target_menu_scroll_y_ > 1.f)
                target_menu_scroll_y_ = 1.f;
            menu_scroll_y_ += (target_menu_scroll_y_ - menu_scroll_y_) * 0.2;

            float y = 0.8f;
            for(auto& level : levels_){
                if(gui_.draw_button(level.name, 0.5f, y + menu_scroll_y_, 0.4f, 0.1f)){
                    gui_.play_sound("res/sounds/click_sound.wav");
                    auto loaded_level = reader_writer_.read_file(level.path);
                    if(loaded_level){
                        gui_.set_scene<LevelEditorScene>(*loaded_level, current_player_);
                    }
                    else{
                        std::cout << "Unable to load level." << std::endl;
                    }
                }
                y -= 0.2f;
            }

            if(gui_.draw_button("Back", 0.1f, 0.9f, 0.05f, 0.05f)){
                gui_.play_sound("res/sounds/click_sound.wav");
                menu_state_ = MenuState::MainMenu;
            }
            break;
        }
    }
}
