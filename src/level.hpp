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
    /**
     * @brief Construct a new Level object
     * 
     * @param entities List of entities
     * @param birds List of birds
     * @param backgroundPath Path to background
     * @param name Name of level
     * @param scores List of scores reached in level
     * @param save_name Path to save file
     */
    Level(std::vector<std::shared_ptr<Entity>> entities, std::vector<std::shared_ptr<Bird>> birds, std::string backgroundPath,
        std::string name, std::vector<ScoreBoardEntry> scores, std::string save_name);

    /**
     * @brief Get the list of entities in level
     * 
     * @return std::vector<std::shared_ptr<Entity>> The list of entities
     */
    std::vector<std::shared_ptr<Entity>> get_entities() const;

    /**
     * @brief Get the list of bird objects
     * 
     * @return std::vector<std::shared_ptr<Bird>> 
     */
    std::vector<std::shared_ptr<Bird>> get_birds() const;

    /**
     * @brief Get path to the background file
     * 
     * @return std::string Path to file
     */
    std::string get_background() const;

    /**
     * @brief Get the name of the Level
     * 
     * @return std::string 
     */
    std::string get_name() const;

    /**
     * @brief Get the scores reached in the level
     * 
     * @return std::vector<ScoreBoardEntry> List of scores
     */
    std::vector<ScoreBoardEntry> get_scores() const;

    /**
     * @brief Register a new score to the level
     * 
     * @param name Name of player 
     * @param score Score reached
     */
    void add_score(std::string name, int score);

    /**
     * @brief Get the path to the level
     * 
     * @return std::string Path
     */
    std::string get_save_name() const;
private:
    std::vector<std::shared_ptr<Entity>> entities_; 
    std::vector<std::shared_ptr<Bird>> birds_; 
    std::string backgroundPath_;
    std::vector<ScoreBoardEntry> scores_;
    std::string save_name_, name_;
};