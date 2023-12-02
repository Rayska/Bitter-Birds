#include "ReaderWriter.hpp"
#include <fstream>
#include <sstream>


std::optional<Level> ReaderWriter::readFile(std::string fileName) const {
    // If fileName doesn't end in ".txt", adds ".txt" to the end
    if (fileName.substr(fileName.length() - 4) != ".txt") {
        fileName += ".txt";
    }
    std::ifstream is(fileName);
    
    if (is.is_open()) {
        std::string line;
        std::string name, backgroundPath, soundtrackPath;
        std::vector<std::string> soundFX;
        std::vector<std::shared_ptr<Entity>> entities;
        std::vector<std::shared_ptr<Bird>> birds;
        while (std::getline(is, line)) {
            Header header = this->getHeader(line);
            switch (header) {
                case Header::soundFXStart:
                    soundFX = this->readList(is, Header::soundFXEnd); 
                    break;
                case Header::EntityStart:
                    entities = this->formEntities(this->readList(is, Header::EntityEnd));
                    break;     
                case Header::BirdsStart:
                    birds = this->formBirds(readList(is, Header::BirdsEnd));
                    break;
                default:
                    if (header != Header::unknown) {
                        std::string content = this->getContent(line);
                        switch (header)
                        {
                        case Header::levelName:
                            name = content;
                            break;
                        case Header::backgroundPath:
                            backgroundPath = content;
                            break;
                        case Header::soundtrackPath:
                            soundtrackPath = content;
                            break;
                        };
                    };
            }
        }
        if (!entities.empty() && !birds.empty() && !backgroundPath.empty() && !soundtrackPath.empty() && !soundFX.empty() && !name.empty()) {
            return Level(entities, birds, backgroundPath, soundtrackPath, soundFX, name);
        }
    }
    return {};
}

void ReaderWriter::writeFile(Level level, std::string fileName) const {
    // If fileName doesn't end in ".txt", adds ".txt" to the end
    if (fileName.substr(fileName.length() - 4) != ".txt") {
        fileName += ".txt";
    }
    std::ofstream os(fileName);
    if (os.is_open()) {
        // Game version and newline to separate it from the rest of the data
        os << "BitterBirds v." << versionNumber << std::endl;
        os << std::endl;

        // Vectors: SoundFX paths, Entities, and Birds

        // SoundFX paths:
        os << "SST" << std::endl;
        for (auto it : level.getSoundFX()) {
            os << it << std::endl;
        }
        os << "SEN" << std::endl;

        // Entities:
        os << "EST" << std::endl;
        for (auto it : level.getEntities()) {
            os << this->toStringEntity(it) << std::endl;
        }
        os << "EEN" << std::endl;

        // Birds:
        os << "BST" << std::endl;
        for (auto it : level.getBirds()) {
            os << this->toStringBird(it) << std::endl;
        }
        os << "BEN" << std::endl;

        // Level name:
        os << "NME" << level.getName() << std::endl;

        // Background path:
        os << "BGR" << level.getBackground() << std::endl;

        // Soundtrack path:
        os << "SDT" << level.getSoundtrack() << std::endl;
    }
}

std::vector<std::string> ReaderWriter::fetchFiles() const {
    return {};
}

std::vector<std::string> ReaderWriter::readList(std::ifstream& is, Header h) const {
    std::vector<std::string> r;
    std::string line;
    while (std::getline(is, line)) {
        if (line.length() < 5 && this->getHeader(line) == h) {
            break;
        }
        r.push_back(line);
    }
    return r;
}

std::shared_ptr<Entity> ReaderWriter::formEntity(std::string line) const {
    int healthPoints, typeOrHeight, width;
    double initRotation, x, y;
    std::istringstream ss(line);
    ss >> healthPoints >> initRotation >> x >> y >> typeOrHeight >> width;
    if (width < 0) {
        return std::make_shared<Enemy>(healthPoints, initRotation, x, y, typeOrHeight);
    }
    else {
        return std::make_shared<Structure>(healthPoints, initRotation, x, y, typeOrHeight, width);
    }
    return nullptr;
    //return Entity(movable, destructible, healthPoints, initRotation, x, y);
}

std::shared_ptr<Bird> ReaderWriter::formBird(std::string line) const {
    if (line.size() == 2) {
        int type = std::stoi(line.substr(1, 1));
        switch (type)
        {
        case 1:
            /* code */
            break;
        case 2:
            /* code */
            break;
        default:
            return nullptr;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Entity>> ReaderWriter::formEntities(std::vector<std::string> entityStrings) const {
    std::vector<std::shared_ptr<Entity>> entities;
    for (auto it : entityStrings) {
        entities.push_back(formEntity(it));
    }
    return entities;
}

std::vector<std::shared_ptr<Bird>> ReaderWriter::formBirds(std::vector<std::string> birdStrings) const {
    std::vector<std::shared_ptr<Bird>> birds;
    for (auto it : birdStrings) {
        birds.push_back(formBird(it));
    }
    return birds;
}

std::string ReaderWriter::toStringBird(std::shared_ptr<Bird> bird) const {
    if (typeid(bird) == typeid(NormalBird())) {
        return "0";
    }
    else if (typeid(bird) == typeid(NormalBird())) {
        return "";
    }
    else if (typeid(bird) == typeid(NormalBird())) {
        return "";
    }
    return "";
}

std::string ReaderWriter::toStringEntity(std::shared_ptr<Entity> e) const {
    std::stringstream line;
    const std::type_info& type = typeid(e);
    const std::type_info& typeEnemy = typeid(Enemy);
    const std::type_info& typeStructure = typeid(Structure);
    if (type == typeEnemy) {
        line << e->getHealthPoints() << " " << e->getRotation() << " " << e->getX() << " " << e->getY(); //<< " " << e.
    }
    else if (type == typeStructure) {
        line << e->getHealthPoints() << " " << e->getRotation() << " " << e->getX() << " " << e->getY() << " "; //<< e.getWidth();
    }
    else {
        line << "Entity type not recognised";
    }
    return line.str();
}

Header ReaderWriter::getHeader(std::string line) const {
    std::string headerString = line.substr(0, 3);
    if (headerString == "NME") {
        return Header::levelName;
    }
    else if (headerString == "BGR") {
        return Header::backgroundPath;
    }
    else if (headerString == "STD") {
        return Header::soundtrackPath;
    }
    else if (headerString == "SST") {
        return Header::soundFXStart;
    }
    else if (headerString == "SEN") {
        return Header::soundFXEnd;
    }
    else if (headerString == "EST") {
        return Header::EntityStart;
    }
    else if (headerString == "EEN") {
        return Header::EntityEnd;
    }
    else if (headerString == "BST") {
        return Header::BirdsStart;
    }
    else if (headerString == "BEN") {
        return Header::BirdsEnd;
    }
    else {
        return Header::unknown;
    }
}

std::string ReaderWriter::getContent(std::string line) const {
    return line.substr(4, line.size() - 4);
}
