#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <array>

#include "image.hpp"

class Scene;

struct Color {
    float r, g, b;
};

/**
 * @brief 
 * GUI Class to create window and run the game
 */
class GUI {
public:
    /**
     * @brief Construct a new GUI object
     */
	GUI();
    /**
     * @brief Destroy the GUI object
     */
	~GUI();

    /**
     * @brief Start the application
     */
	void run();

    void close();

    /**
     * @brief Set the current Scene object to be replaced at start of next frame
     * @tparam SceneType scene class type
     * @tparam Params argument types of SceneType constructor
     * @param params arguments for constructing new class of SceneType
     */
    template<typename SceneType, typename...Params>
    void setScene(Params...params){
        new_scene_ = new SceneType(*this, params...);
    }

    /**
     * @brief Get current cursor position
     * @return std::pair<float, float> position in [0, 1] range
     */
    std::pair<float, float> cursorPosition() const;

    /**
     * @brief Return state of keyboard key
     * @param key 
     * @return true Key is pressed
     * @return false Key is released
     */
    bool keyState(sf::Keyboard::Key key) const; 
    /**
     * @brief Return state of mouse button
     * @param btn Button id
     * @return true Button is pressed
     * @return false Button is released
     */
    bool buttonState(sf::Mouse::Button btn) const;
    bool buttonReleased(sf::Mouse::Button btn) const;
    int scrollDelta() const;

    void setViewport(float x, float y, float w, float h);

    /**
     * @brief Draw a rectangular sprite on screen
     * @param x
     * @param y 
     * @param w 
     * @param h 
     * @param angle 
     * @param img 
     */
    void drawSprite(float x, float y, float w, float h, float angle, const Image& img);

    void drawRect(float x, float y, float w, float h, float angle, Color color);

    void drawText(float x, float y, float h, const std::string& text);

    /**
     * @brief Draw a button on screen. When mouse is hovered and pressed over button returns true
     * @param text 
     * @param x 
     * @param y 
     * @param w 
     * @param h 
     * @param button_image 
     * @return true Button is pressed
     * @return false 
     */
    bool drawButton(const std::string& text, float x, float y, float w, float h);
private:
    void update(float ts);
private:
    sf::RenderWindow window_;
    Scene* current_scene_;
    Scene* new_scene_;
    sf::Font font_;
    std::array<bool, (int)sf::Mouse::Button::ButtonCount> buttons_, prev_buttons_;
    int scroll_y_, prev_scroll_y_;
};
