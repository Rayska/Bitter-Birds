#pragma once

#include <string>
#include <SFML/Graphics.hpp>

class Image {
    friend class GUI;
public:
    /**
     * @brief Construct a new Image object
     * 
     * @param path Path to image
     */
    Image(const std::string& path);
    
private:
    sf::Texture image_;
};