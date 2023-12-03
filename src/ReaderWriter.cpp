#include "ReaderWriter.hpp"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>

#if 0
#define RW_LOG(msg) std::cout << (msg) << std::endl
#else
#define RW_LOG(msg)
#endif

std::vector<std::string> split_string(std::string str, char delim = ' '){
    std::vector<std::string> result;

    std::string tmp;
    for(char c : str){
        if(c == delim){
            if(tmp.size() > 0)
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
        std::vector<ScoreBoardEntry> scores;
        while (std::getline(is, line)) {
            Header header = this->getHeader(line);
            switch (header) {
                case Header::soundFXStart:
                {
                    RW_LOG("SoundFX");
                    soundFX = this->readList(is, Header::soundFXEnd);
                    break;
                }
                case Header::EntityStart:
                {
                    RW_LOG("Entitys");
                    entities = this->formEntities(this->readList(is, Header::EntityEnd));
                    break;     
                }
                case Header::BirdsStart:
                {
                    RW_LOG("Birds");
                    birds = this->formBirds(readList(is, Header::BirdsEnd));
                    break;
                }
                case Header::ScoresStart:
                {
                    RW_LOG("Scores");
                    scores = this->formScores(readList(is, Header::ScoresEnd));
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
        return Level(entities, birds, backgroundPath, soundtrackPath, soundFX, name, scores, fileName);
    }
    return {};
}

void ReaderWriter::writeFile(Level level, std::string fileName) const {
    std::ofstream os(fileName);

    if (os.is_open()) {
        // Game version and newline to separate it from the rest of the data
        os << "BitterBirds v." << versionNumber << std::endl;
        os << std::endl << std::endl;

        os << "NME " << level.getName() << std::endl << std::endl;

        // Entities:
        os << "EST" << std::endl;
        for (auto it : level.getEntities()) {
            os << this->toStringEntity(it) << std::endl;
        }
        os << "EEN" << std::endl << std::endl;

        // Birds:
        os << "BST" << std::endl;
        for (auto it : level.getBirds()) {
            os << this->toStringBird(it) << std::endl;
        }
        os << "BEN" << std::endl << std::endl;
        
        // Scores:
        os << "SCR" << std::endl;
        for(auto& it : level.getScores()){
            os << it.name << " " << it.score << std::endl;
        }
        os << "ESC" << std::endl << std::endl;
    }
}

std::vector<LevelInfo> ReaderWriter::getLevels() const {
    std::string levels_path = "levels";
    std::vector<std::string> levels;    

    for(const auto& entry : std::filesystem::directory_iterator(levels_path)){
        levels.push_back(entry.path().string());
    }

    std::vector<LevelInfo> presetLevel_infos;
    std::vector<LevelInfo> level_infos;
    for(auto& level : levels){
        if(auto lvl = readFile(level)){
            try {
                if (lvl->getName().find("Level ") == 0 && lvl->getName().size() > 6) {
                    std::stoi(lvl->getName().substr(6));
                    presetLevel_infos.push_back({
                    lvl->getName(),
                    lvl->getSaveName()
                    });
                }
                else {
                    level_infos.push_back({
                        lvl->getName(),
                        lvl->getSaveName()
                    });
                }
            } catch (const std::invalid_argument& e) {
                level_infos.push_back({
                lvl->getName(),
                lvl->getSaveName()
                });
            } catch (const std::out_of_range& e) {
                level_infos.push_back({
                lvl->getName(),
                lvl->getSaveName()
                });
            }
        }
    }
    std::sort(presetLevel_infos.begin(), presetLevel_infos.end(), compareLvlName);
    std::sort(level_infos.begin(), level_infos.end(), compareLvlName);
    level_infos.insert(level_infos.end(), presetLevel_infos.begin(), presetLevel_infos.end());
    return level_infos;
}

std::optional<std::string> ReaderWriter::getNextLevel(std::string path) const {
    auto lvls = getLevels();
    bool next = false;
    for(auto& lvl : lvls){
        if(lvl.path == path && !next)
            next = true;
        else if(next)
            return lvl.path;
    }
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
    auto parts = split_string(line);

    RW_LOG("formEntity");
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
        RW_LOG("UNABLE TO PARSE ENTITY");
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
    RW_LOG("UNABLE TO PARSE BIRD");
    return nullptr;
}

std::vector<std::shared_ptr<Entity>> ReaderWriter::formEntities(std::vector<std::string> entityStrings) const {
    std::vector<std::shared_ptr<Entity>> entities;
    for (auto it : entityStrings) {
        auto ent = formEntity(it);
        if(ent != nullptr)
            entities.push_back(ent);
    }
    return entities;
}

std::vector<std::shared_ptr<Bird>> ReaderWriter::formBirds(std::vector<std::string> birdStrings) const {
    std::vector<std::shared_ptr<Bird>> birds;
    for (auto it : birdStrings) {
        auto brd = formBird(it);
        if(brd != nullptr)
            birds.push_back(brd);
    }
    return birds;
}

std::string ReaderWriter::toStringBird(std::shared_ptr<Bird> bird) const {
    switch(bird->getBirdType()){
        case birdType::normal: return "NORMAL";
        case birdType::special1: return "SPECIAL1";
        case birdType::special2: return "SPECIAL2";
    }
    return "UNKNOWN";
}

std::string ReaderWriter::toStringEntity(std::shared_ptr<Entity> e) const {
    if(auto struct_ent = std::dynamic_pointer_cast<Structure>(e)){
        return 
            "STRUCTURE " 
            + std::to_string(struct_ent->getHealthPoints()) + " " 
            + std::to_string(struct_ent->getRotation()) + " " 
            + std::to_string(struct_ent->getX()) + " " 
            + std::to_string(struct_ent->getY()) + " " 
            + std::to_string(struct_ent->getHeight()) + " " 
            + std::to_string(struct_ent->getWidth());
    }
    else if(auto enemy_ent = std::dynamic_pointer_cast<Enemy>(e)){
        return 
            "ENEMY " 
            + std::to_string(enemy_ent->getHealthPoints()) + " " 
            + std::to_string(enemy_ent->getRotation()) + " " 
            + std::to_string(enemy_ent->getX()) + " " 
            + std::to_string(enemy_ent->getY()) + " "
            + std::to_string(0);
    }
    else{
        return "UNKNOWN";        
    }
}

Header ReaderWriter::getHeader(std::string line) const {
    std::string headerString = line.substr(0, 3);

    std::unordered_map<std::string, Header> mapping{
        {"NME", Header::levelName},
        {"BGR", Header::backgroundPath},
        {"STD", Header::soundtrackPath},
        {"SST", Header::soundFXStart},
        {"SEN", Header::soundFXEnd},
        {"EST", Header::EntityStart},
        {"EEN", Header::EntityEnd},
        {"BST", Header::BirdsStart},
        {"BEN", Header::BirdsEnd},
        {"SCR", Header::ScoresStart},
        {"ESC", Header::ScoresEnd},
    };

    return (mapping.count(headerString) == 1) ? mapping[headerString] : Header::unknown;
}


std::vector<ScoreBoardEntry> ReaderWriter::formScores(std::vector<std::string> scoreStrings) const {
    std::vector<ScoreBoardEntry> scores;
    for(auto& ln : scoreStrings){
        scores.push_back(formScore(ln));
    }
    return scores;
}

ScoreBoardEntry ReaderWriter::formScore(std::string line) const {
    auto split = split_string(line);
    int score = ::atoi(split[split.size() - 1].c_str());
    return ScoreBoardEntry{ split[0], score };
}

std::string ReaderWriter::getContent(std::string line) const {
    return line.substr(4, line.size() - 4);
}

bool ReaderWriter::compareLvlName(LevelInfo first, LevelInfo second) {
    std::string firstName = first.name;
    std::string secondName = second.name;
    for (char& c : firstName) {
        c = std::tolower(c);
    }
    for (char& c : secondName) {
        c = std::tolower(c);
    }
    return firstName < secondName;
}