#pragma once

#include <optional>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include "level.hpp"
#include "enemy.hpp"
#include "structure.hpp"

/**
* @brief Different types of Headers, Allows us to use switch case with headers
*/
enum struct Header {
    levelName, 
    backgroundPath, 
    soundtrackPath, 
    soundFXStart, 
    soundFXEnd, 
    EntityStart, 
    EntityEnd, 
    BirdsStart, 
    BirdsEnd, 
    ScoresStart,
    ScoresEnd,
    unknown
};

/**
 * @brief Information about levels
 */
struct LevelInfo {
    std::string name;
    std::string path;
};

/**
* @brief A class for loading/reading and saving/writing levels in forms of Level objects and .txt files
*/
class ReaderWriter {
    public:
        /**
        * @brief Constructor 
        */
        ReaderWriter() {}

        /**
        * @brief Given the file name as the parameter loads the level
        * 
        * @return std::optional<Level> Loaded level 
        */
        std::optional<Level> read_file(std::string fileName) const;

        /**
        * @brief Saves the provided level into a file named fileName
        */
        void write_file(Level level, std::string fileName) const;

        /**
        * @brief Returns a list of levels that can be loaded
        */
        std::vector<LevelInfo> get_levels() const;

        /**
         * @brief Get the name of the next level in sequence
         * 
         * @return std::optional<std::string> Name of level if there is a next level
         */
        std::optional<std::string> get_next_level(std::string) const;
    private:
        std::string versionNumber_ = "0";
        
        /**
        * @brief Takes an ifstream and returns the elements of a vector.
        * 
        * Stops reading when a line contains the string associated with Header h, returned by getHeader
        */
        std::vector<std::string> read_list(std::ifstream& is, Header h) const;

        /**
        * @brief Takes a single line of text and construct an instance of Entity
        * 
        * @todo Implementation, requires implementation of Entity class, it's derived classes, and a format design
        */
        std::shared_ptr<Entity> form_entity(std::string line) const;

        /**
        * @brief Takes a single line of text and construct an instance of the matching Bird type
        * 
        * @todo Implementation, requires implementation of Bird class, it's derived classes, and a format design
        */
        std::shared_ptr<Bird> form_bird(std::string line) const;

        /**
        * @brief Create Entity objects from a std::vector<std::string>, where each element contains all the needed information for the constructor
        *
        */
        std::vector<std::shared_ptr<Entity>> form_entities(std::vector<std::string> entityStrings) const;

        /**
        * @brief Create Bird objects from a std::vector<std::string>, where each element contains all the needed information for the constructor
        *
        */
        std::vector<std::shared_ptr<Bird>> form_birds(std::vector<std::string> birdStrings) const;

        /**
        * @brief Forms a string that represents the Bird object's state
        * 
        * @todo Implementation, requires implementation of Bird class, it's derived classes, and a format design
        */
        std::string to_string_bird(std::shared_ptr<Bird> bird) const;

        /**
        * @brief Forms a string that represents the Bird object's state
        * 
        * @todo Implementation, requires implementation of Entity class, it's derived classes, and a format design
        */
        std::string to_string_entity(std::shared_ptr<Entity> e) const;

        std::vector<ScoreBoardEntry> form_scores(std::vector<std::string> scoreStrings) const;
        ScoreBoardEntry form_score(std::string line) const;

        /**
        * @brief Extracts the first 3 characters from line and returns matching Header
        *
        */
        Header get_header(std::string line) const;

        /**
        * @brief Drops the first 3 characters / header and returns the content without header
        *
        */
        std::string get_content(std::string line) const;

        /**
         * @brief Compare level names. Return true if first < second
        */
        static bool compare_lvl_name(LevelInfo first, LevelInfo second);

        /**
         * @brief Compare preset level names. Return true if first < second
        */
        static bool compare_preset_lvl_name(LevelInfo first, LevelInfo second);
};