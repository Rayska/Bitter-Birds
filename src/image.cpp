#include "image.hpp"

#include <iostream>

Image::Image(const std::string &path) {
    if(!image_.loadFromFile(path.c_str())){
        std::cout << "Failed to load image '" << path << "'" << std::endl;
    }
}
