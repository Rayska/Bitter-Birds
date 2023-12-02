#include "ReaderWriter.hpp"
#include <fstream>
#include <sstream>
#include <filesystem>

std::vector<std::string> split_string(std::string str, char delim = ' '){
    std::vector<std::string> result;

    std::string tmp;
    for(char c : str){
        if(c == delim){
            result.push_back(tmp);
            tmp = "";
        }
        else{
            tmp.push_back(c);
        }
    }
    result.push_back(tmp);

    return result;
}

std::optional<Level> ReaderWriter::readFile(std::string fileName) const {
    std::ifstream is(fileName);
    
    if (is) {
        std::string line;
        std::string name, backgroundPath, soundtrackPath;
        std::vector<std::string> soundFX;
        std::vector<std::shared_ptr<Entity>> entities;
        std::vector<std::shared_ptr<Bird>> birds;
        while (std::getline(is, line)) {
            Header header = this->getHeader(line);
            switch (header) {
                case Header::soundFXStart:
                {
                    soundFX = this->readList(is, Header::soundFXEnd); 
                    break;
                }
                case Header::EntityStart:
                {
                    entities = this->formEntities(this->readList(is, Header::EntityEnd));
                    break;     
                }
                case Header::BirdsStart:
                {
                    birds = this->formBirds(readList(is, Header::BirdsEnd));
                    break;
                }
                default:
                {
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
                    break;
                }
            }
        }
        return Level(entities, birds, backgroundPath, soundtrackPath, soundFX, name, {});
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

std::vector<std::string> ReaderWriter::getLevels() const {
    std::string levels_path = "levels";
    std::vector<std::string> levels;

    for(const auto& entry : std::filesystem::directory_iterator(levels_path)){
        levels.push_back(entry.path().string());
    }

    return levels;
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
    auto parts = split_string(line);

    if(parts[0] == "STRUCTURE"){
        return std::make_shared<Structure>(
            ::atoi(parts[1].c_str()), 
            ::atof(parts[2].c_str()), 
            ::atof(parts[3].c_str()),
            ::atof(parts[4].c_str()), 
            ::atof(parts[5].c_str()), 
            ::atof(parts[6].c_str())
        );
    }
    else if(parts[0] == "ENEMY") {
        return std::make_shared<Enemy>(
            ::atoi(parts[1].c_str()), 
            ::atof(parts[2].c_str()), 
            ::atof(parts[3].c_str()), 
            ::atof(parts[4].c_str()), 
            ::atoi(parts[5].c_str())
        );
    }
    else{
        return nullptr;
    }
}

std::shared_ptr<Bird> ReaderWriter::formBird(std::string line) const {
    if(line == "NORMAL"){
        return std::make_shared<NormalBird>();
    }
    else if(line == "SPECIAL1"){
        return std::make_shared<SpecialBird1>();
    }
    else if(line == "SPECIAL2"){
        return std::make_shared<SpecialBird2>();
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
