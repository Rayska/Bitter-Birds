#pragma once

#include <string>
#include <SFML/Graphics.hpp>

class Image {
    friend class GUI;
public:
    Image(const std::string& path);
    
private:
    sf::Texture image_;
};