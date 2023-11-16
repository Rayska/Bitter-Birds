#include "GUI.hpp"

#include "scene.hpp"

#include <iostream>

GUI::GUI() 
	:
	window_(sf::VideoMode(1280, 720), "Bitter Birds!"),
	current_scene_(nullptr),
	new_scene_(nullptr) {}

GUI::~GUI() {
	if(current_scene_)
		delete current_scene_;
	if(new_scene_)
		delete new_scene_;
}

void GUI::run() {
	sf::Clock timer;
	while (window_.isOpen())
    {
		// Poll for events and handle them
        sf::Event event;
        while (window_.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window_.close();
        }

		// Get delta time
		float ts = timer.getElapsedTime().asSeconds();
		timer.restart();

		// Update
        window_.clear();
		update(ts);
        window_.display();
    }
}

std::pair<float, float> GUI::cursorPosition() const {
	auto pos = sf::Mouse::getPosition(window_);
	auto size = window_.getSize();
    return {static_cast<float>(pos.x) / size.x, 1.f - static_cast<float>(pos.y) / size.y};
}

bool GUI::keyState(sf::Keyboard::Key key) const {
    return sf::Keyboard::isKeyPressed(key);
}

bool GUI::buttonState(sf::Mouse::Button btn) const {
    return sf::Mouse::isButtonPressed(btn);
}

void GUI::setViewport(float x, float y, float w, float h) {
	auto wsize = window_.getSize();
	// window_.setView(sf::View({x * wsize.x / 2, y * wsize.y / 2}, {w * wsize.x, h * wsize.y}));
	window_.setView(sf::View({x * wsize.x, y * wsize.y}, {w * wsize.x, h * wsize.y}));
}

void GUI::drawSprite(float x, float y, float w, float h, float angle, const Image &img) {
	sf::Sprite sp;
	sp.setTexture(img.image_);
	auto size = window_.getSize();
	sp.setPosition((x - w * 0.5f) * size.x, (1.f - y - h * 0.5f) * size.y);
	auto imgSize = img.image_.getSize();
	sp.setScale(w * size.x / imgSize.x, h * size.y / imgSize.y);
	sp.setRotation(angle);
	window_.draw(sp);
}

bool GUI::drawButton(const std::string& text, float x, float y, float w, float h, const Image& button_image) {
	auto[cx, cy] = cursorPosition();
	drawSprite(x, y, w, h, 0.f, button_image);
	return buttonState(sf::Mouse::Button::Left) && 
		((cx > x - w * 0.5f) && (cx < x + w * 0.5f)) && 
		((cy > y - h * 0.5f) && (cy < y + h * 0.5f)) ;
}

void GUI::update(float ts)
{
    if(new_scene_ != nullptr){
		delete current_scene_;
		current_scene_ = new_scene_;
		new_scene_ = nullptr;
	}

	if(current_scene_)
		current_scene_->update(ts);
}