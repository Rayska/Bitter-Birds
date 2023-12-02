#include "nameEntryScene.hpp"

#include "menuScene.hpp"

NameEntryScene::NameEntryScene(GUI& gui)
    :
    Scene(gui)
{}

NameEntryScene::~NameEntryScene()
{}

void NameEntryScene::update(float ts) {
    // Key presses
    if(gui_.keyState(sf::Keyboard::Key::A)){
    }

    // Rendering
    gui_.setViewport(0.5f, 0.5f, 1.f, 1.f);

    gui_.drawText(0.5f, 0.7f, 0.2f, "Enter your name");
    gui_.drawText(0.5f, 0.5f, 0.2f, current_player_);
    if(gui_.drawButton("Ok", 0.5f, 0.4f, 0.1f, 0.05f)){
        gui_.setScene<MenuScene>(current_player_);
    }
}

void NameEntryScene::on_input(char c) {
    if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        current_player_ += c;
    else if(c == 0x08){
        if(current_player_.size() > 0)
            current_player_.pop_back();
    }
}