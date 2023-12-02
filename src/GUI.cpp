#include "GUI.hpp"

#include "scene.hpp"

#include <iostream>

GUI::GUI() 
	:
	window_(sf::VideoMode(1280, 720), "Bitter Birds!"),
	current_scene_(nullptr),
	new_scene_(nullptr),
	prev_scroll_y_(0) {

	std::cout << "Created window." << std::endl;

	if(!font_.loadFromFile("res/DancingScript-Regular.ttf")){
		std::cout << "Failed to load font" << std::endl;
	}

	scroll_y_ = 0;

	buttons_.fill(false);
	prev_buttons_.fill(false);
}

GUI::~GUI() {
	if(current_scene_)
		delete current_scene_;
	if(new_scene_)
		delete new_scene_;
	for (auto i = 0; i < buffers_.size(); i++) {
		delete buffers_[i];
	}
	for (auto& [k, v] : sounds_) {
		delete v;
	}
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
				scroll_y_ += (int)event.mouseWheelScroll.delta;
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
    
float GUI::getAspectRatio() const {
	auto wsize = window_.getSize();
	return wsize.x / wsize.y;
}

void GUI::setViewport(float x, float y, float w, float h) {
	auto wsize = window_.getSize();
	window_.setView(sf::View({x, y}, {w, h}));
}

void GUI::drawSprite(float x, float y, float w, float h, float angle, const Image &img) {
	sf::Sprite sp;
	sp.setTexture(img.image_);
	auto size = window_.getSize();
	auto imgSize = img.image_.getSize();
	sp.setRotation(angle);
	sp.setScale(w / imgSize.x, h / imgSize.y);
	sp.setPosition(x, (1.f - y ));
	sf::FloatRect rc = sp.getLocalBounds();
	sp.setOrigin(rc.width / 2, rc.height / 2);
	window_.draw(sp);
}

void GUI::drawRect(float x, float y, float w, float h, float angle, Color color) {
	sf::RectangleShape sp;
	sp.setFillColor({(sf::Uint8)(color.r * 255), (sf::Uint8)(color.g * 255), (sf::Uint8)(color.b * 255)});

	auto size = window_.getSize();
	sp.setPosition((x - w * 0.5f), (1.f - y - h * 0.5f));
	sp.setSize({w, h});

	window_.draw(sp);
}

void GUI::drawText(float x, float y, float h, const std::string& text, Alignment align, sf::Color color) {
	sf::Text txt(sf::String(text.c_str()), font_);
	// auto size = window_.getSize();
	txt.setPosition({x, 1.f - y});
	txt.setScale({h * 0.01f, h * 0.01f});
	switch(align){
		case Alignment::Center:
		{
			sf::FloatRect rc = txt.getLocalBounds();
			txt.setOrigin(rc.width / 2, rc.height / 2);
			break;
		}
		case Alignment::RightCenter:
		{
			sf::FloatRect rc = txt.getLocalBounds();
			txt.setOrigin(rc.width, rc.height / 2);
			break;
		}
		case Alignment::LeftCenter:
		{
			sf::FloatRect rc = txt.getLocalBounds();
			txt.setOrigin(0, rc.height / 2);
			break;
		}
	}
	sf::Color colour(color.r, color.g, color.b);
	txt.setFillColor(colour);
	window_.draw(txt);
}

bool GUI::drawButton(const std::string& text, float x, float y, float w, float h) {
	auto[cx, cy] = cursorPosition();

	Color defaultColor{0.1f, 0.1f, 0.1f}, hoverColor{0.2f, 0.2f, 0.2f};

	bool isHovered = 
		((cx > x - w * 0.5f) && (cx < x + w * 0.5f)) && 
		((cy > y - h * 0.5f) && (cy < y + h * 0.5f));

	drawRect(x, y, w * 0.95, h * 0.95, 0.f, isHovered ? hoverColor : defaultColor);
	drawText(x, y, h, text);

	return buttonReleased(sf::Mouse::Button::Left) && isHovered;
}

void GUI::update(float ts)
{
    if(new_scene_ != nullptr){
		std::cout << "Switching scenes now." << std::endl;
		delete current_scene_;
		current_scene_ = new_scene_;
		new_scene_ = nullptr;
	}

	if(current_scene_)
		current_scene_->update(ts);
}

void GUI::playSound(std::string path){
      if(sounds_.find(path) == sounds_.end()) {
        buffers_.push_back(new sf::SoundBuffer());
        buffers_[buffers_.size() - 1] -> loadFromFile(path);
        sounds_[path] = new sf::Sound(*buffers_[buffers_.size() - 1]);
      }
    sounds_[path] -> play();
}