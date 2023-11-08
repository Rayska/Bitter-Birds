#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "image.hpp"

class Scene;

class GUI {
public:
	GUI();
	~GUI();

	void run();

    template<typename SceneType, typename...Params>
    void setScene(Params...params){
        new_scene_ = new SceneType(*this, params...);
    }

    // Input state
    std::pair<float, float> cursorPosition() const;
    bool keyState(sf::Keyboard::Key key) const; 
    bool buttonState(sf::Mouse::Button btn) const;

    // Rendering
    void drawSprite(float x, float y, float w, float h, float angle, const Image& img);
    bool drawButton(const std::string& text, float x, float y, float w, float h, const Image& button_image);
private:
    void update(float ts);
private:
    sf::RenderWindow window_;
    Scene* current_scene_;
    Scene* new_scene_;
};
