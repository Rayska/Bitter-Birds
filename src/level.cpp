#include "level.hpp"

Level::Level(std::vector<std::shared_ptr<Entity>> entities, std::vector<std::shared_ptr<Bird>> birds, std::string backgroundPath,
         std::string name, std::vector<ScoreBoardEntry> scores, std::string save_name)
        : 
        entities_(entities), birds_(birds), backgroundPath_(backgroundPath),
        scores_(scores), name_(name), save_name_(save_name)
    {}

std::vector<std::shared_ptr<Entity>> Level::get_entities() const {
    return entities_;
}

std::vector<std::shared_ptr<Bird>> Level::get_birds() const {
    return birds_;
}

std::string Level::get_background() const {
    return backgroundPath_;
}

std::string Level::get_name() const {
    return name_;
}

std::vector<ScoreBoardEntry> Level::get_scores() const {
    return scores_;
}
void Level::add_score(std::string name, int score){
    scores_.push_back({name, score});
    std::sort(scores_.begin(), scores_.end(), [](const auto& lhs, const auto& rhs){
        return lhs.score > rhs.score;
    });
}
std::string Level::get_save_name() const {
    return save_name_;
}
