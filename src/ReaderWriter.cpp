#include "ReaderWriter.hpp"
#include <fstream>


std::optional<Level> ReaderWriter::readFile(std::string fileName) const {
    // If fileName doesn't end in ".txt", adds ".txt" to the end
    if (fileName.substr(fileName.length() - 4) != ".txt") {
        fileName += ".txt";
    }
    std::ifstream is("filename");
    
    // Check that file has been opened successfully
    if (is.is_open()) {
        std::string line;
        // Read the file line by line until EOF
        std::string name, backgroundPath, soundtrackPath;
        std::vector<std::string> soundFX;
        std::vector<Entity> entities;
        std::vector<Bird> birds;
        while (std::getline(is, line)) {
            Header header = this->getHeader(line);
            switch (header) {
                case Header::sst:
                    soundFX = this->readList(is, Header::sen); // Could be named soundFXPaths, but in order to be in line with level.hpp it will be like this for now
                    break;
                case Header::est:
                    entities = this->formEntities(this->readList(is, Header::een));
                    break;     
                case Header::bst:
                    birds = this->formBirds(readList(is, Header::ben));
                    break;
                default:
                    if (header != Header::unknown) {
                        std::string content = this->getContent(line);
                        switch (header)
                        {
                        case Header::nme:
                            name = content;
                            break;
                        case Header::bgr:
                            backgroundPath = content;
                            break;
                        case Header::std:
                            soundtrackPath = content;
                            break;
                        };
                    };
            }
        }
        // Check whether any of the contructor variables are empty. If even one is empty, construction has failed.
        if (!entities.empty() && !birds.empty() && !backgroundPath.empty() && !soundtrackPath.empty() && !soundFX.empty() && !name.empty()) {
            return Level(entities, birds, backgroundPath, soundtrackPath, soundFX, name);
        }
        return std::nullopt;
    }
}

void ReaderWriter::writeFile(Level level, std::string fileName) const {
    std::ofstream os("outfile");

}

std::vector<std::string> ReaderWriter::fetchFiles() const {

}

std::vector<std::string> ReaderWriter::readList(std::ifstream& is, Header h) const {
    std::vector<std::string> r;
    std::string line;
    while (std::getline(is, line)) {
        if (line.length() < 5 && this->getHeader(line) == Header::sen) {
            break;
        }
        r.push_back(line);
    }
    return r;
}

std::vector<Entity> ReaderWriter::formEntities(std::vector<std::string> entityStrings) const {

}

std::vector<Bird> ReaderWriter::formBirds(std::vector<std::string> birdStrings) const {

}

Header ReaderWriter::getHeader(std::string line) const {
    if (line == "NME") {
        return Header::nme;
    }
    else if (line == "BGR") {
        return Header::bgr;
    }
    else if (line == "STD") {
        return Header::std;
    }
    else if (line == "SST") {
        return Header::sst;
    }
    else if (line == "SEN") {
        return Header::sen;
    }
    else if (line == "EST") {
        return Header::est;
    }
    else if (line == "EEN") {
        return Header::een;
    }
    else if (line == "BST") {
        return Header::bst;
    }
    else if (line == "BEN") {
        return Header::ben;
    }
    else {
        return Header::unknown;
    }
}

std::string ReaderWriter::formHeader(Header header) const {
    if (header == Header::nme) {
        return "NME";
    }
    else if (header == Header::bgr) {
        return "BGR";
    }
    else if (header == Header::bgr) {
        return "BGR";
    }
    else if (header == Header::bgr) {
        return "BGR";
    }
    else if (header == Header::bgr) {
        return "BGR";
    }
    else if (header == Header::bgr) {
        return "BGR";
    }
    else if (header == Header::bgr) {
        return "BGR";
    }
    else if (header == Header::bgr) {
        return "BGR";
    }
    else if (header == Header::bgr) {
        return "BGR";
    }
    else {
        return "UNKNOWN";
    }
}