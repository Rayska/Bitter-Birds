#include "nameEntryScene.hpp"

#include "menuScene.hpp"

NameEntryScene::NameEntryScene(GUI& gui)
    :
    Scene(gui)
{}

NameEntryScene::~NameEntryScene()
{}

void NameEntryScene::update(float ts) 
{}

void NameEntryScene::render() {
    // Rendering
    gui_.set_viewport(0.5f, 0.5f, 1.f, 1.f);

    gui_.draw_text(0.5f, 0.7f, 0.2f, "Enter your name");
    gui_.draw_text(0.5f, 0.5f, 0.2f, current_player_);

    if(current_player_.size() != 0 && (gui_.draw_button("Ok", 0.5f, 0.4f, 0.1f, 0.05f) || gui_.key_state(sf::Keyboard::Enter))){
        gui_.set_scene<MenuScene>(current_player_);
    }
}

void NameEntryScene::on_input(char c) {
    bool is_alpha = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    bool is_backspace = c == 0x08;
    if(is_alpha)
        current_player_ += c;
    else if(is_backspace && current_player_.size() > 0)
        current_player_.pop_back();
}