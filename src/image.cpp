#include "image.hpp"

Image::Image(const std::string &path) {
    image_.loadFromFile(path.c_str());
}

Image::~Image() {
}
