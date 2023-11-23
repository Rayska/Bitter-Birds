#include "GUI.hpp"

#include "scene.hpp"

#include <iostream>

GUI::GUI() 
	:
	window_(sf::VideoMode(1280, 720), "Bitter Birds!"),
	current_scene_(nullptr),
	new_scene_(nullptr) {
	font_.loadFromFile("res/DancingScript-Regular.ttf");
	scroll_y_ = 0;

	buttons_.fill(false);
	prev_buttons_.fill(false);
}

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
			if(event.type == sf::Event::MouseWheelScrolled){
				scroll_y_ += event.mouseWheelScroll.delta;
			}
            if (event.type == sf::Event::Closed)
                window_.close();
        }

		// Get delta time
		float ts = timer.getElapsedTime().asSeconds();
		timer.restart();

		prev_buttons_ = buttons_;
		// Update button states
		for(int i = 0; i < buttons_.size(); i++){
			buttons_[i] = sf::Mouse::isButtonPressed((sf::Mouse::Button)i);
		}

		// Update
        window_.clear();
		update(ts);
        window_.display();
		
		prev_scroll_y_ = scroll_y_;
    }
}

void GUI::close() {
	window_.close();
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
    return buttons_[(int)btn];
}
    
bool GUI::buttonReleased(sf::Mouse::Button btn) const {
	return !buttons_[(int)btn] && prev_buttons_[(int)btn];
}

int GUI::scrollDelta() const {
	return scroll_y_ - prev_scroll_y_;
}

void GUI::setViewport(float x, float y, float w, float h) {
	auto wsize = window_.getSize();
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

void GUI::drawRect(float x, float y, float w, float h, float angle, Color color) {
	sf::RectangleShape sp;
	sp.setFillColor({(int)(color.r * 255), (int)(color.g * 255), (int)(color.b * 255)});

	auto size = window_.getSize();
	sp.setPosition((x - w * 0.5f) * size.x, (1.f - y - h * 0.5f) * size.y);
	sp.setSize({w * size.x, h * size.y});

	window_.draw(sp);
}

void GUI::drawText(float x, float y, float h, const std::string& text) {
	sf::Text txt(sf::String(text.c_str()), font_);
	auto size = window_.getSize();
	txt.setPosition({x * size.x, (1.f - y) * size.y});
	txt.setScale({h, h});
	sf::FloatRect rc = txt.getLocalBounds();
	txt.setOrigin(rc.width / 2, rc.height / 2);
	window_.draw(txt);
}

bool GUI::drawButton(const std::string& text, float x, float y, float w, float h) {
	auto[cx, cy] = cursorPosition();

	Color defaultColor{0.1f, 0.1f, 0.1f}, hoverColor{0.2f, 0.2f, 0.2f};

	bool isHovered = 
		((cx > x - w * 0.5f) && (cx < x + w * 0.5f)) && 
		((cy > y - h * 0.5f) && (cy < y + h * 0.5f));

	drawRect(x, y, w * 0.95, h * 0.95, 0.f, isHovered ? hoverColor : defaultColor);
	drawText(x, y, h * 5.f, text);

	return buttonReleased(sf::Mouse::Button::Left) && isHovered;
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