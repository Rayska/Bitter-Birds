#include "levelEditorScene.hpp"

#include "menuScene.hpp"
#include "readerWriter.hpp"

LevelEditorScene::LevelEditorScene(GUI& gui, Level& level, const std::string current_player)
    :
    Scene(gui),
    cam_x_(0.f), cam_y_(-6.f), cam_scale_x_(15.f), cam_scale_y_(15.f),
    grass_image_("res/grass.png"),
    enemy_bird_image_("res/enemy_bird.png"),
    strcture_image_("res/wood.png"),
    sling_image_("res/slingshot.png"),
    level_(level),
    current_player_(current_player),
    chosenEntity_(nullptr),
    input_(0),
    width_(0.0),
    height_(0.0),
    healthpoints_(0.0),
    rotation_(0.0),
    current_input_("")
{
    entities_ = level_.get_entities();
    birds_ = level_.get_birds();
}

LevelEditorScene::~LevelEditorScene() {}

void LevelEditorScene::update(float ts) {
    if(gui_.key_state(sf::Keyboard::Escape)){
        gui_.set_scene<MenuScene>(current_player_);
    }

    if(gui_.key_state(sf::Keyboard::A)){
        cam_x_ -= 5.f * ts;
    }
    if(gui_.key_state(sf::Keyboard::D)){
        cam_x_ += 5.f * ts;
    }
    if(gui_.key_state(sf::Keyboard::W)){
        cam_y_ -= 5.f * ts;
    }
    if(gui_.key_state(sf::Keyboard::S)){
        cam_y_ += 5.f * ts;
    }
}

void LevelEditorScene::render() {
    

    if(gui_.button_state(sf::Mouse::Button::Left)){
        if (!chosenEntity_) {
            auto[cx, cy] = gui_.cursor_position();
            auto worldPos = screen_to_world({cx, cy});
            for (auto ent : entities_) {
                if (ent.get()->contains(worldPos.x, worldPos.y)) {
                    chosenEntity_ = ent;
                    break;
                }
            }
        }
        if (chosenEntity_) {
            auto[cx, cy] = gui_.cursor_position();
            auto worldPos = screen_to_world({cx, cy});
            switch (chosenEntity_ -> get_type())
            {
                case bodyType::structure: 
                {
                    auto ent_structure = std::dynamic_pointer_cast<Structure>(chosenEntity_);
                    auto newEnt = std::make_shared<Structure>(ent_structure->get_health_points(),ent_structure->get_rotation(), worldPos.x, worldPos.y, ent_structure->get_height(), ent_structure->get_width());
                    
                    entities_.erase(
                    std::remove_if(entities_.begin(), entities_.end(),
                        [&](std::shared_ptr<Entity> ptr)-> bool 
                                        { return ptr.get() == chosenEntity_.get(); } 
                    ),
                    entities_.end());

                    entities_.push_back(newEnt);
                    chosenEntity_ = newEnt;
                    break;
                }

                case bodyType::enemy: 
                {
                    auto ent_structure = std::dynamic_pointer_cast<Enemy>(chosenEntity_);
                    auto newEnt = std::make_shared<Enemy>(ent_structure->get_health_points(),ent_structure->get_rotation(), worldPos.x, worldPos.y, 0);
                    
                    entities_.erase(
                    std::remove_if(entities_.begin(), entities_.end(),
                        [&](std::shared_ptr<Entity> ptr)-> bool 
                                        { return ptr.get() == chosenEntity_.get(); } 
                    ),
                    entities_.end());

                    entities_.push_back(newEnt);
                    chosenEntity_ = newEnt;
                    break;
                }
                
                default:
                {
                    break;
                }
            }
        }
    } else {
        if (chosenEntity_) {
            chosenEntity_ = nullptr;
        }
    }

    if(gui_.button_state(sf::Mouse::Button::Right)){
        auto[cx, cy] = gui_.cursor_position();
        auto worldPos = screen_to_world({cx, cy});
        for (auto ent : entities_) {
            if (ent.get()->contains(worldPos.x, worldPos.y)) {
                entities_.erase(
                    std::remove_if(entities_.begin(), entities_.end(),
                        [&](std::shared_ptr<Entity> ptr)-> bool 
                                        { return ptr.get() == ent.get(); } 
                    ),
                    entities_.end());
                break;
            }
        }
    }

    // Rendering 
    if (input_ != 0) {
        gui_.set_viewport(0.5f, 0.5f, 1.f, 1.f);
        switch (input_)
        {
            case 1:
            {
                gui_.draw_text(0.5f, 0.7f, 0.1f, "Enter structure width");
                gui_.draw_text(0.5f, 0.5f, 0.1f, current_input_);
                if (current_input_ != "0" && current_input_ != "0." && atof(current_input_.c_str()) != 0) {
                    if(gui_.draw_button("Ok", 0.5f, 0.4f, 0.1f, 0.05f) || gui_.key_state(sf::Keyboard::Enter)){
                        input_++;
                        width_=atof(current_input_.c_str());
                        current_input_ = "";
                    }
                }
                break;
            }
            case 2:
            {
                gui_.draw_text(0.5f, 0.7f, 0.1f, "Enter structure height");
                gui_.draw_text(0.5f, 0.5f, 0.1f, current_input_);
                if (current_input_ != "0" && current_input_ != "0." && atof(current_input_.c_str()) != 0) {
                    if(gui_.draw_button("Ok", 0.5f, 0.4f, 0.1f, 0.05f) || gui_.key_state(sf::Keyboard::Enter)){
                        input_++;
                        height_=atof(current_input_.c_str());
                        current_input_ = "";
                    }
                }
                break;
            }
            case 3:
            {
                gui_.draw_text(0.5f, 0.7f, 0.1f, "Enter hp, 100 is default");
                gui_.draw_text(0.5f, 0.5f, 0.1f, current_input_);
                if (current_input_ != "0" && atoi(current_input_.c_str()) != 0) {
                    if(gui_.draw_button("Ok", 0.5f, 0.4f, 0.1f, 0.05f) || gui_.key_state(sf::Keyboard::Enter)){
                        input_++;
                        healthpoints_=atoi(current_input_.c_str());
                        current_input_ = "";
                    }
                }
                break;
            }
            case 4:
            {
                gui_.draw_text(0.5f, 0.7f, 0.1f, "Enter rotation");
                gui_.draw_text(0.5f, 0.5f, 0.1f, current_input_);
                if (current_input_ != "") {
                    if(gui_.draw_button("Ok", 0.5f, 0.4f, 0.1f, 0.05f) || gui_.key_state(sf::Keyboard::Enter)){
                        input_ = 0;
                        rotation_=atof(current_input_.c_str());
                        current_input_ = "";

                        switch (created_)
                        {
                            case bodyType::structure:
                            {
                                entities_.push_back(std::make_shared<Structure>(healthpoints_, rotation_, 0.0f, 1.5f, height_, width_));
                                break;
                            }
                            case bodyType::enemy:
                            {
                                entities_.push_back(std::make_shared<Enemy>(healthpoints_, rotation_, 0.0f, 1.0f, 0));
                                break;
                            }
                            default:
                            {
                                break;
                            }
                        }
                    }
                }
                break;
            }
            case 5:
            {
                gui_.draw_text(0.5f, 0.7f, 0.1f, "Enter new level name");
                gui_.draw_text(0.5f, 0.5f, 0.1f, current_input_);
                if(current_input_ != "" && gui_.draw_button("Ok", 0.5f, 0.4f, 0.1f, 0.05f) || gui_.key_state(sf::Keyboard::Enter)){
                    input_=0;
                    reader_writer_.write_file(
                        Level(
                            entities_,
                            birds_,
                            "",
                            current_input_,
                            std::vector<ScoreBoardEntry>(),
                            "levels/" + current_input_ + ".bblvl"
                        ),
                        "levels/" + current_input_ + ".bblvl"
                    );
                    current_input_ = "";
                }
                break;
            }
            
            default:
                break;
        }
    } else {
        // Render world
        gui_.set_viewport(cam_x_, cam_y_, cam_scale_x_, cam_scale_y_ * gui_.get_aspect_ratio());

        // Draw slingshot
        gui_.draw_sprite(-5, 1.5, 2.f, 2.f, 0.f, sling_image_);

        // Draw ground as 100 sequential grass squares
        for(int i = -50; i < 50; i++){
            gui_.draw_sprite(i, 0.f, 1.f, 1.f, 0.f, grass_image_);
        }

        // Draw entities
        for(auto& ent : entities_){
            bodyType type = (*ent).get_type();
            switch (type)
            {
            case bodyType::structure:
                {
                    auto ent_structure = std::dynamic_pointer_cast<Structure>(ent);
                    Image* img = &strcture_image_;
                    gui_.draw_sprite(ent_structure -> get_x(), ent_structure -> get_y(), ent_structure->get_width(), ent_structure->get_height(), ent_structure->get_rotation(), *img);
                    break;
                }
            case bodyType::ground:
                {
                }
                break;
            case bodyType::enemy:
                {
                    auto ent_enemy = std::dynamic_pointer_cast<Enemy>(ent);
                    Image* img = &enemy_bird_image_;
                    gui_.draw_sprite(ent_enemy -> get_x(), ent_enemy -> get_y(), 1, 1, 0, *img);
                    break;
                }
            default:
                {
                    break;
                }
            }
        }

        // UI 
        gui_.set_viewport(0.5f, 0.5f, 1.f, 1.f);

        //Buttons
        sf::Color color(0,255,0);
        if (gui_.draw_button("Structure", 0.15f, 0.95f, 0.2f, 0.05f)) {
            input_=1;
            created_=bodyType::structure;
            current_input_="";
        }
        if (gui_.draw_button("Enemy", 0.35f, 0.95f, 0.2f, 0.05f)) {
            input_=3;
            created_=bodyType::enemy;
            current_input_="";
        }
        if (gui_.draw_button("Save Level", 0.85f, 0.95f, 0.2f, 0.05f)) {
            input_=5;
            current_input_="Level name";
        }
        if (gui_.draw_button("Reset Birds", 0.55f, 0.95f, 0.2f, 0.05f)) {
            birds_.clear();
        }
        if (gui_.draw_button("Make normal", 0.15f, 0.88f, 0.2f, 0.05f)) {
            birds_.push_back(std::make_shared<NormalBird>());
        }
        if (gui_.draw_button("Make yellow", 0.35f, 0.88f, 0.2f, 0.05f)) {
            birds_.push_back(std::make_shared<SpecialBird1>());
        }
        if (gui_.draw_button("Make blue", 0.55f, 0.88f, 0.2f, 0.05f)) {
            birds_.push_back(std::make_shared<SpecialBird2>());
        }

        gui_.draw_text(0.85f, 0.88f, 0.05f, "Birds:");
        int i = 1;
        for (auto b : birds_) {
            switch (b.get() -> get_bird_type())
            {
            case birdType::normal:
                gui_.draw_text(0.85f, 0.88f - 0.03f * i, 0.05f, "Normal");
                break;
            case birdType::special1:
                gui_.draw_text(0.85f, 0.88f - 0.03f * i, 0.05f, "Yellow");
                break;
            case birdType::special2:
                gui_.draw_text(0.85f, 0.88f - 0.03f * i, 0.05f, "Blue");
                break;
            default:
                break;
            }
            i++;
        }
    }
}

b2Vec2 LevelEditorScene::screen_to_world(b2Vec2 pos){
    return {0.5f * (pos.x * 2.f - 1.f) * cam_scale_x_ + cam_x_, 0.5f * (pos.y * 2.f - 1.f) * cam_scale_y_ - cam_y_};
}

void LevelEditorScene::on_input(char c) {
    if(c >= '0' && c <= '9' || c == '.' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' '){
        std::string tmp = current_input_;
        tmp.push_back(c);
        // Try convert
        float p = atof(tmp.c_str());
        // Check failure
        if(true){
            current_input_ = tmp;
        }  
    } else if (c == 0x08) {
        if(current_input_.size() > 0)
            current_input_.pop_back();
    }
}
