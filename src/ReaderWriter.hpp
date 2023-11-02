#pragma once

#include <optional>
#include <string>
#include <vector>
#include "level.hpp"

enum struct Header {
    // Refer to BitterBirdsFileFormat https://docs.google.com/document/d/1Y1-SJsiiOjuyTeWvKD0GDAmaJLkKQ5z4CW_AUsgPUxY/edit?usp=sharing
    nme, bgr, std, sst, sen, est, een, bst, ben, unknown
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
        /*
        Takes a single line of text and construct an instance of Entity
        */
        Entity readEntity(std::string line) const;

        /*
        Takes a single line of text and construct an instance of the according Bird type
        */
        Bird readBird(std::string line) const;
        
        /*
        Extracts the first 3 characters from line and returns matching Header
        */
        Header getHeader(std::string line) const;

        /*
        Returns the std::string equivalent of header
        */
        std::string formHeader(Header header) const;

        /*
        Drops the first 3 characters / header and returns the content without header
        */
        std::string getContent(std::string line) const;

        /*
        Takes an ifstream and returns the soundpaths in a vector. Stops reading when a line contains "SEN"
        */
        std::vector<std::string> readList(std::ifstream& is, Header h) const;

        /*
        
        */
        

};