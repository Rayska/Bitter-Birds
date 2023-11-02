#pragma once

#include <vector>
#include <string>
#include "entity.hpp"
#include "bird.hpp"


class Level {
public:
    //Create Level
    Level(std::vector<Entity> entities, std::vector<Bird> birds, std::string backgroundPath
        , std::string soundtrackPath, std::vector<std::string> soundFX, std::string name)
        : entities_(entities), birds_(birds), backgroundPath_(backgroundPath), soundtrackPath_(soundtrackPath)
        , soundFX_(soundFX), name_(name) {}

    //get the list of entities at the start of the level
    std::vector<Entity> getEntities() const {
        return entities_;
    }

    //get the list of usable birds at the start of the level
    std::vector<Bird> getBirds() const {
        return birds_;
    }

    //get background path for the level
    std::string getBackground() const {
        return backgroundPath_;
    }

    //get soundtrack path for the level
    std::string getSoundtrack() const {
        return soundtrackPath_;
    }

    //get soundFX paths to the game
    std::vector<std::string> getSoundFX() const {
        return soundFX_;
    }

    //get name of the level
    std::string getName() const {
        return name_;
    }

private:
    std::vector<Entity> entities_; 
    std::vector<Bird> birds_; 
    std::string backgroundPath_;
    std::string soundtrackPath_;
    std::vector<std::string> soundFX_;
    std::string name_;
};