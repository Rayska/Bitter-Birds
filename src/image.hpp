#pragma once

#include <string>
#include <SFML/Graphics.hpp>

class Image {
    friend class GUI;
public:
    Image(const std::string& path);
    //~Image();
    
private:
    sf::Texture image_;
};