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
        while (std::getline(is, line)) {
            switch (this->getHeader(line)) {
                case Header::nme:
                    break;
                case Header::bgr:
                    break;
                case Header::std:
                    break;
                case Header::sst:
                    break;
                case Header::sen:
                    break;
                case Header::est:
                    break;     
                case Header::een:
                    break;
                case Header::bst:
                    break;
                case Header::ben:
                    break;
            }
        }
    }
}

void ReaderWriter::writeFile(Level level, std::string fileName) const {
    std::ofstream os("outfile");

}

std::vector<std::string> ReaderWriter::fetchFiles() const {

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
}