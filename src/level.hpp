#pragma once

#include <vector>
#include <string>
#include <memory>
#include "entity.hpp"
#include "bird.hpp"

struct ScoreBoardEntry {
    std::string name;
    int score;
};

class Level {
public:
    //Create Level
    Level(std::vector<std::shared_ptr<Entity>> entities, std::vector<std::shared_ptr<Bird>> birds, std::string backgroundPath,
        std::string name, std::vector<ScoreBoardEntry> scores, std::string save_name);

    //get the list of entities at the start of the level
    std::vector<std::shared_ptr<Entity>> get_entities() const;

    //get the list of usable birds at the start of the level
    std::vector<std::shared_ptr<Bird>> get_birds() const;

    //get background path for the level
    std::string get_background() const;

    //get name of the level
    std::string get_name() const;

    std::vector<ScoreBoardEntry> get_scores() const;

    void add_score(std::string name, int score);

    std::string get_save_name() const;
private:
    std::vector<std::shared_ptr<Entity>> entities_; 
    std::vector<std::shared_ptr<Bird>> birds_; 
    std::string backgroundPath_;
    std::vector<ScoreBoardEntry> scores_;
    std::string save_name_, name_;
};