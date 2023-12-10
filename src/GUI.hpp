#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <array>
#include <iostream>

#include "image.hpp"

class Scene;

/**
 * @brief Color object 
 * r is red in range 0 to 1
 * g is green in range 0 to 1
 * b is blue in range 0 to 1
 */
struct Color {
    float r, g, b;
};

/**
 * @brief Enumeration for alignment types 
 */
enum struct Alignment {
    LeftCenter,
    Center,
    RightCenter
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
    void set_scene(Params...params){
        std::cout << "Set new scene" << std::endl;
        new_scene_ = new SceneType(*this, params...);
    }

    /**
     * @brief Get current cursor position
     * @return std::pair<float, float> position in [0, 1] range
     */
    std::pair<float, float> cursor_position() const;

    /**
     * @brief Return state of keyboard key
     * @param key 
     * @return true Key is pressed
     * @return false Key is released
     */
    bool key_state(sf::Keyboard::Key key) const; 
    
    /**
     * @brief Return state of mouse button
     * @param btn Button id
     * @return true Button is pressed
     * @return false Button is released
     */
    bool button_state(sf::Mouse::Button btn) const;
    bool button_released(sf::Mouse::Button btn) const;
    int scroll_delta() const;

    float get_aspect_ratio() const;
    void set_viewport(float x, float y, float w, float h);

    /**
     * @brief Draw a rectangular sprite on screen
     * @param x
     * @param y 
     * @param w 
     * @param h 
     * @param angle 
     * @param img 
     */
    void draw_sprite(float x, float y, float w, float h, float angle, const Image& img);

    /**
     * @brief Draw a rectangle with a color
     * @param x 
     * @param y 
     * @param w 
     * @param h 
     * @param angle 
     * @param color 
     */
    void draw_rect(float x, float y, float w, float h, float angle, Color color);

    /**
     * @brief Draw text to screen with text centering
     * 
     * @param x xpos
     * @param y ypos
     * @param h text scale
     * @param text text string
     */
    void draw_text(float x, float y, float h, const std::string& text, Alignment align = Alignment::Center, sf::Color color = sf::Color::White);

    /**
     * @brief Draw a button on screen. When mouse is hovered and pressed over button returns true
     * @param text 
     * @param x 
     * @param y 
     * @param w 
     * @param h 
     * @param button_image 
     * @return true Button is released
     * @return false otherwise
     */
    bool draw_button(const std::string& text, float x, float y, float w, float h);

    /**
     * @brief Play sound from given path. Caches sound file inside class
     * 
     * @param path Path to file
     * @param vol Volume to play at
     */
    void play_sound(std::string path, int vol = 100);
private:
    void update(float ts);
private:
    sf::RenderWindow window_;
    Scene* current_scene_;
    Scene* new_scene_;
    sf::Font font_;
    std::array<bool, (int)sf::Mouse::Button::ButtonCount> buttons_, prev_buttons_;
    int scroll_y_, prev_scroll_y_;
    std::vector<sf::SoundBuffer*> buffers_;
    std::map<std::string, sf::Sound*> sounds_;
};
