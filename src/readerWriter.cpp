#include "readerWriter.hpp"
#include "readerWriter.hpp"
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

std::optional<Level> ReaderWriter::read_file(std::string fileName) const {
    std::ifstream is(fileName);
    
    if (is) {
        std::string line;
        std::string name, backgroundPath, soundtrackPath;
        std::vector<std::string> soundFX;
        std::vector<std::shared_ptr<Entity>> entities;
        std::vector<std::shared_ptr<Bird>> birds;
        std::vector<ScoreBoardEntry> scores;
        while (std::getline(is, line)) {
            Header header = this->get_header(line);
            switch (header) {
                case Header::soundFXStart:
                {
                    RW_LOG("SoundFX");
                    soundFX = this->read_list(is, Header::soundFXEnd);
                    break;
                }
                case Header::EntityStart:
                {
                    RW_LOG("Entitys");
                    entities = this->form_entities(this->read_list(is, Header::EntityEnd));
                    break;     
                }
                case Header::BirdsStart:
                {
                    RW_LOG("Birds");
                    birds = this->form_birds(read_list(is, Header::BirdsEnd));
                    break;
                }
                case Header::ScoresStart:
                {
                    RW_LOG("Scores");
                    scores = this->form_scores(read_list(is, Header::ScoresEnd));
                    break;
                }
                default:
                {
                    if (header != Header::unknown) {
                        std::string content = this->get_content(line);
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
        return Level(entities, birds, backgroundPath, name, scores, fileName);
    }
    return {};
}

void ReaderWriter::write_file(Level level, std::string fileName) const {
    std::ofstream os(fileName);

    if (os.is_open()) {
        // Game version and newline to separate it from the rest of the data
        os << "BitterBirds v." << versionNumber_ << std::endl;
        os << std::endl << std::endl;

        os << "NME " << level.get_name() << std::endl << std::endl;

        // Entities:
        os << "EST" << std::endl;
        for (auto it : level.get_entities()) {
            os << this->to_string_entity(it) << std::endl;
        }
        os << "EEN" << std::endl << std::endl;

        // Birds:
        os << "BST" << std::endl;
        for (auto it : level.get_birds()) {
            os << this->to_string_bird(it) << std::endl;
        }
        os << "BEN" << std::endl << std::endl;
        
        // Scores:
        os << "SCR" << std::endl;
        for(auto& it : level.get_scores()){
            os << it.name << " " << it.score << std::endl;
        }
        os << "ESC" << std::endl << std::endl;
    }
}

std::vector<LevelInfo> ReaderWriter::get_levels() const {
    std::string levels_path = "levels";
    std::vector<std::string> levels;    

    for(const auto& entry : std::filesystem::directory_iterator(levels_path)){
        levels.push_back(entry.path().string());
    }

    std::vector<LevelInfo> presetLevel_infos;
    std::vector<LevelInfo> level_infos;
    for(auto& level : levels){
        if(auto lvl = read_file(level)){
            std::string lvlName = lvl->get_name();
            try {
                if (lvlName.find("Level ") == 0 && lvlName.size() > 6) {
                    std::stoi(lvlName.substr(6));
                    presetLevel_infos.push_back({
                    lvlName,
                    lvl->get_save_name()
                    });
                }
                else {
                    level_infos.push_back({
                        lvlName,
                        lvl->get_save_name()
                    });
                }
            } catch (const std::invalid_argument& e) {
                level_infos.push_back({
                lvlName,
                lvl->get_save_name()
                });
            } catch (const std::out_of_range& e) {
                level_infos.push_back({
                lvlName,
                lvl->get_save_name()
                });
            }
        }
    }
    std::sort(presetLevel_infos.begin(), presetLevel_infos.end(), compare_preset_lvl_name);
    std::sort(level_infos.begin(), level_infos.end(), compare_lvl_name);
    level_infos.insert(level_infos.end(), presetLevel_infos.begin(), presetLevel_infos.end());
    return level_infos;
}

std::optional<std::string> ReaderWriter::get_next_level(std::string path) const {
    auto lvls = get_levels();
    bool next = false;
    for(auto& lvl : lvls){
        if(lvl.path == path && !next)
            next = true;
        else if(next)
            return lvl.path;
    }
    return {};
}

std::vector<std::string> ReaderWriter::read_list(std::ifstream& is, Header h) const {
    std::vector<std::string> r;
    std::string line;
    while (std::getline(is, line)) {
        if (line.length() < 5 && this->get_header(line) == h) {
            break;
        }
        r.push_back(line);
    }
    return r;
}


std::shared_ptr<Entity> ReaderWriter::form_entity(std::string line) const {
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

std::shared_ptr<Bird> ReaderWriter::form_bird(std::string line) const {
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

std::vector<std::shared_ptr<Entity>> ReaderWriter::form_entities(std::vector<std::string> entityStrings) const {
    std::vector<std::shared_ptr<Entity>> entities;
    for (auto it : entityStrings) {
        auto ent = form_entity(it);
        if(ent != nullptr)
            entities.push_back(ent);
    }
    return entities;
}

std::vector<std::shared_ptr<Bird>> ReaderWriter::form_birds(std::vector<std::string> birdStrings) const {
    std::vector<std::shared_ptr<Bird>> birds;
    for (auto it : birdStrings) {
        auto brd = form_bird(it);
        if(brd != nullptr)
            birds.push_back(brd);
    }
    return birds;
}

std::string ReaderWriter::to_string_bird(std::shared_ptr<Bird> bird) const {
    switch(bird->get_bird_type()){
        case birdType::normal: return "NORMAL";
        case birdType::special1: return "SPECIAL1";
        case birdType::special2: return "SPECIAL2";
    }
    return "UNKNOWN";
}

std::string ReaderWriter::to_string_entity(std::shared_ptr<Entity> e) const {
    if(auto struct_ent = std::dynamic_pointer_cast<Structure>(e)){
        return 
            "STRUCTURE " 
            + std::to_string(struct_ent->get_health_points()) + " " 
            + std::to_string(struct_ent->get_rotation()) + " " 
            + std::to_string(struct_ent->get_x()) + " " 
            + std::to_string(struct_ent->get_y()) + " " 
            + std::to_string(struct_ent->get_height()) + " " 
            + std::to_string(struct_ent->get_width());
    }
    else if(auto enemy_ent = std::dynamic_pointer_cast<Enemy>(e)){
        return 
            "ENEMY " 
            + std::to_string(enemy_ent->get_health_points()) + " " 
            + std::to_string(enemy_ent->get_rotation()) + " " 
            + std::to_string(enemy_ent->get_x()) + " " 
            + std::to_string(enemy_ent->get_y()) + " "
            + std::to_string(0);
    }
    else{
        return "UNKNOWN";        
    }
}

Header ReaderWriter::get_header(std::string line) const {
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


std::vector<ScoreBoardEntry> ReaderWriter::form_scores(std::vector<std::string> scoreStrings) const {
    std::vector<ScoreBoardEntry> scores;
    for(auto& ln : scoreStrings){
        scores.push_back(form_score(ln));
    }
    return scores;
}

ScoreBoardEntry ReaderWriter::form_score(std::string line) const {
    auto split = split_string(line);
    int score = ::atoi(split[split.size() - 1].c_str());
    return ScoreBoardEntry{ split[0], score };
}

std::string ReaderWriter::get_content(std::string line) const {
    return line.substr(4, line.size() - 4);
}

bool ReaderWriter::compare_lvl_name(LevelInfo first, LevelInfo second) {
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

bool ReaderWriter::compare_preset_lvl_name(LevelInfo first, LevelInfo second) {
    return std::stoi(first.name.substr(6)) < std::stoi(second.name.substr(6));
}