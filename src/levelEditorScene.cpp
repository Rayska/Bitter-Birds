#include "levelEditorScene.hpp"

#include "menuScene.hpp"
#include "ReaderWriter.hpp"

LevelEditorScene::LevelEditorScene(GUI& gui, Level& level, const std::string current_player)
    :
    Scene(gui),
    cam_x(0.f), cam_y(-6.f), cam_scale_x(15.f), cam_scale_y(15.f),
    grass_image_("res/grass.png"),
    enemy_bird_image_("res/enemy_bird.png"),
    bird_image_("res/test_bird.png"),
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
    entities_ = level_.getEntities();
}

LevelEditorScene::~LevelEditorScene() {}

void LevelEditorScene::update(float ts)
{
    if(gui_.keyState(sf::Keyboard::Escape)){
        gui_.setScene<MenuScene>(current_player_);
    }

    if(gui_.keyState(sf::Keyboard::A)){
        cam_x -= 5.f * ts;
    }
    if(gui_.keyState(sf::Keyboard::D)){
        cam_x += 5.f * ts;
    }
    if(gui_.keyState(sf::Keyboard::W)){
        cam_y -= 5.f * ts;
    }
    if(gui_.keyState(sf::Keyboard::S)){
        cam_y += 5.f * ts;
    }

    if(gui_.buttonState(sf::Mouse::Button::Left)){
        if (!chosenEntity_) {
            auto[cx, cy] = gui_.cursorPosition();
            auto worldPos = screen_to_world({cx, cy});
            for (auto ent : entities_) {
                if (ent.get()->contains(worldPos.x, worldPos.y)) {
                    chosenEntity_ = ent;
                    break;
                }
            }
        }
        if (chosenEntity_) {
            auto[cx, cy] = gui_.cursorPosition();
            auto worldPos = screen_to_world({cx, cy});
            switch (chosenEntity_ -> getType())
            {
                case bodyType::structure: 
                {
                    auto ent_structure = std::dynamic_pointer_cast<Structure>(chosenEntity_);
                    auto newEnt = std::make_shared<Structure>(ent_structure->getHealthPoints(),ent_structure->getRotation(), worldPos.x, worldPos.y, ent_structure->getHeight(), ent_structure->getWidth());
                    
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
                    auto newEnt = std::make_shared<Enemy>(ent_structure->getHealthPoints(),ent_structure->getRotation(), worldPos.x, worldPos.y, 0);
                    
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

    if(gui_.buttonState(sf::Mouse::Button::Right)){
        auto[cx, cy] = gui_.cursorPosition();
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
        gui_.setViewport(0.5f, 0.5f, 1.f, 1.f);
        switch (input_)
        {
            case 1:
            {
                gui_.drawText(0.5f, 0.7f, 0.2f, "Enter structure width");
                gui_.drawText(0.5f, 0.5f, 0.2f, current_input_);
                if(gui_.drawButton("Ok", 0.5f, 0.4f, 0.1f, 0.05f)){
                    input_++;
                    width_=atof(current_input_.c_str());
                    current_input_ = "";
                }
                break;
            }
            case 2:
            {
                gui_.drawText(0.5f, 0.7f, 0.2f, "Enter structure height");
                gui_.drawText(0.5f, 0.5f, 0.2f, current_input_);
                if(gui_.drawButton("Ok", 0.5f, 0.4f, 0.1f, 0.05f)){
                    input_++;
                    height_=atof(current_input_.c_str());
                    current_input_ = "";
                }
                break;
            }
            case 3:
            {
                gui_.drawText(0.5f, 0.7f, 0.2f, "Enter hp, 100 is default");
                gui_.drawText(0.5f, 0.5f, 0.2f, current_input_);
                if(gui_.drawButton("Ok", 0.5f, 0.4f, 0.1f, 0.05f)){
                    input_++;
                    healthpoints_=atoi(current_input_.c_str());
                    current_input_ = "0";
                }
                break;
            }
            case 4:
            {
                gui_.drawText(0.5f, 0.7f, 0.2f, "Enter rotation");
                gui_.drawText(0.5f, 0.5f, 0.2f, current_input_);
                if(gui_.drawButton("Ok", 0.5f, 0.4f, 0.1f, 0.05f)){
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
                break;
            }
            case 5:
            {
                gui_.drawText(0.5f, 0.7f, 0.2f, "Enter new level name");
                gui_.drawText(0.5f, 0.5f, 0.2f, current_input_);
                if(gui_.drawButton("Ok", 0.5f, 0.4f, 0.1f, 0.05f)){
                    input_=0;
                    reader_writer_.writeFile(
                        Level(
                            entities_,
                            level_.getBirds(),
                            "",
                            "",
                            std::vector<std::string>(),
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
        gui_.setViewport(cam_x, cam_y, cam_scale_x, cam_scale_y * gui_.getAspectRatio());

        // Draw slingshot
        gui_.drawSprite(-5, 1.5, 2.f, 2.f, 0.f, sling_image_);

        // Draw ground as 100 sequential grass squares
        for(int i = -50; i < 50; i++){
            gui_.drawSprite(i, 0.f, 1.f, 1.f, 0.f, grass_image_);
        }

        // Draw entities
        for(auto& ent : entities_){
            bodyType type = (*ent).getType();
            switch (type)
            {
            case bodyType::structure:
                {
                    auto ent_structure = std::dynamic_pointer_cast<Structure>(ent);
                    Image* img = &strcture_image_;
                    gui_.drawSprite(ent_structure -> getX(), ent_structure -> getY(), ent_structure->getWidth(), ent_structure->getHeight(), ent_structure->getRotation(), *img);
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
                    gui_.drawSprite(ent_enemy -> getX(), ent_enemy -> getY(), 1, 1, 0, *img);
                    break;
                }
            default:
                {
                    break;
                }
            }
        }

        // UI 
        gui_.setViewport(0.5f, 0.5f, 1.f, 1.f);

        //
        sf::Color color(0,255,0);
        if (gui_.drawButton("Structure", 0.05f, 0.95f, 0.1f, 0.095f)) {
            input_=1;
            created_=bodyType::structure;
        }
        if (gui_.drawButton("Enemy", 0.15f, 0.95f, 0.1f, 0.095f)) {
            input_=3;
            created_=bodyType::enemy;
        }
        if (gui_.drawButton("Save Level", 0.85f, 0.95f, 0.2f, 0.095f)) {
            input_=5;
        }
    }
}

b2Vec2 LevelEditorScene::screen_to_world(b2Vec2 pos){
    return {0.5f * (pos.x * 2.f - 1.f) * cam_scale_x + cam_x, 0.5f * (pos.y * 2.f - 1.f) * cam_scale_y - cam_y};
}

void LevelEditorScene::on_input(char c) {
    if(c >= '0' && c <= '9' || c == '.'){
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
