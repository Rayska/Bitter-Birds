#pragma once

#include <optional>
#include <string>
#include <vector>
#include <memory>
#include "level.hpp"

/*
Allows us to use switch case with headers
*/
enum struct Header {
    // Refer to BitterBirdsFileFormat https://docs.google.com/document/d/1Y1-SJsiiOjuyTeWvKD0GDAmaJLkKQ5z4CW_AUsgPUxY/edit?usp=sharing
    levelName, backgroundPath, soundtrackPath, soundFXStart, soundFXEnd, EntityStart, EntityEnd, BirdsStart, BirdsEnd, unknown
};

/*
A class for loading/reading and saving/writing levels in forms of Level objects and .txt files
*/
class ReaderWriter {
    public:
        /*
        Constructor defined
        */
        ReaderWriter() {}

        /*
        Given the file name as the parameter returns std::optional<Level>
        */
        std::optional<Level> readFile(std::string fileName) const;

        /*
        Saves the provided level into a .txt file named fileName
        */
        void writeFile(Level level, std::string fileName) const;

        /*
        Returns a vector of available savefiles' names
        TODO: NEED TO DEFINE A PATH FOR SAVEFILES SOMEWHERE
        */
        std::vector<std::string> fetchFiles() const;

    private:
        std::string versionNumber = "0";
        
        /*
        Takes an ifstream and returns the soundpaths in a vector. Stops reading when a line contains "SEN"
        */
        std::vector<std::string> readList(std::ifstream& is, Header h) const;

        /*
        Takes a single line of text and construct an instance of Entity
        */
        Entity formEntity(std::string line) const;

        /*
        Takes a single line of text and construct an instance of the according Bird type
        */
        Bird& formBird(std::string line) const;

        /*
        Create Entity objects from a std::vector<std::string>, where each element contains all the needed information for the constructor
        */
        std::vector<Entity> formEntities(std::vector<std::string> entityStrings) const;

        /*
        Create Bird objects from a std::vector<std::string>, where each element contains all the needed information for the constructor
        */
        std::vector<std::shared_ptr<Bird>> formBirds(std::vector<std::string> birdStrings) const;

         /*
        Forms a string that represents the Bird object's state
        */
        std::string toStringBird(Bird& bird) const;

        /*
        Forms a string that represents the Bird object's state
        */
        std::string toStringEntity(Entity& entity) const;

        /*
        Extracts the first 3 characters from line and returns matching Header
        */
        Header getHeader(std::string line) const;

        /*
        Drops the first 3 characters / header and returns the content without header
        */
        std::string getContent(std::string line) const;

};