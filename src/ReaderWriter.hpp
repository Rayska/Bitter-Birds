#pragma once

#include <optional>
#include <string>
#include <vector>
#include <memory>
#include "level.hpp"

/**
* \brief Allows us to use switch case with headers
*/
enum struct Header {
    levelName, backgroundPath, soundtrackPath, soundFXStart, soundFXEnd, EntityStart, EntityEnd, BirdsStart, BirdsEnd, unknown
};

/**
* \brief A class for loading/reading and saving/writing levels in forms of Level objects and .txt files
*
* 
*/
class ReaderWriter {
    public:
        /**
        * \brief Constructor defined
        */
        ReaderWriter() {}

        /**
        * \brief Given the file name as the parameter returns std::optional<Level>
        */
        std::optional<Level> readFile(std::string fileName) const;

        /**
        * \brief Saves the provided level into a .txt file named fileName
        */
        void writeFile(Level level, std::string fileName) const;

        /**
        * \brief Returns a vector of available savefiles' names
        * 
        /// \todo: Implementation, define a path for savefiles
        */
        std::vector<std::string> fetchFiles() const;

    private:
        std::string versionNumber = "0";
        
        /**
        * \brief Takes an ifstream and returns the elements of a vector.
        * 
        * Stops reading when a line contains the string associated with Header h, returned by getHeader
        */
        std::vector<std::string> readList(std::ifstream& is, Header h) const;

        /**
        * \brief Takes a single line of text and construct an instance of Entity
        * 
        * \todo Implementation, requires implementation of Entity class, it's derived classes, and a format design
        */
        Entity formEntity(std::string line) const;

        /**
        * \brief Takes a single line of text and construct an instance of the matching Bird type
        * 
        * \todo Implementation, requires implementation of Bird class, it's derived classes, and a format design
        */
        Bird& formBird(std::string line) const;

        /**
        * \brief Create Entity objects from a std::vector<std::string>, where each element contains all the needed information for the constructor
        */
        std::vector<Entity> formEntities(std::vector<std::string> entityStrings) const;

        /**
        * \brief Create Bird objects from a std::vector<std::string>, where each element contains all the needed information for the constructor
        */
        std::vector<std::shared_ptr<Bird>> formBirds(std::vector<std::string> birdStrings) const;

        /**
        * \brief Forms a string that represents the Bird object's state
        * 
        * \todo Implementation, requires implementation of Bird class, it's derived classes, and a format design
        */
        std::string toStringBird(Bird& bird) const;

        /**
        * \brief Forms a string that represents the Bird object's state
        * 
        * \todo Implementation, requires implementation of Entity class, it's derived classes, and a format design
        */
        std::string toStringEntity(Entity& entity) const;

        /**
        * \brief Extracts the first 3 characters from line and returns matching Header
        */
        Header getHeader(std::string line) const;

        /**
        * \brief Drops the first 3 characters / header and returns the content without header
        */
        std::string getContent(std::string line) const;

};