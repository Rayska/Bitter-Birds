#include "levelEditorScene.hpp"

#include "menuScene.hpp"
#include "ReaderWriter.hpp"

LevelEditorScene::LevelEditorScene(GUI& gui, Level& level)
    :
    Scene(gui),
    cam_x(0.f), cam_y(-6.f), cam_scale_x(15.f), cam_scale_y(15.f),
    grass_image_("res/grass.png"),
    enemy_bird_image_("res/enemy_bird.png"),
    bird_image_("res/test_bird.png"),
    strcture_image_("res/wood.png"),
    sling_image_("res/slingshot.png")
{
    for(auto& ent : level.getEntities()){
        bodyType type = (*ent).getType();
        switch (type)
        {
        case bodyType::structure:
            {
                auto ent_structure = std::dynamic_pointer_cast<Structure>(ent);
                bodyDef.userData.pointer = (uintptr_t)new userDataStruct{
                    &strcture_image_,
                    bodyType::structure,
                    ent,
                    NULL,
                    ent_structure->getHealthPoints(),
                };
                
                dynamicBox.SetAsBox(.5f * ent_structure->getWidth(), .5f * ent_structure->getHeight());
                bodyDef.angle = ent_structure->getRotation();
                break;
            }
        case bodyType::ground:
            
            break;
        case bodyType::enemy:
            {
                bodyDef.type = b2_dynamicBody;
                isCircle = true;
                auto ent_enemy = std::dynamic_pointer_cast<Enemy>(ent);
                bodyDef.userData.pointer = (uintptr_t)new userDataStruct{
                    &enemy_bird_image_,
                    bodyType::enemy,
                    ent,
                    NULL,
                    ent_enemy->getHealthPoints()
                };
                circle.m_radius = 0.5f;
                break;
            }
        default:
            {
                break;
            }
        }

        b2Body* body = world_.CreateBody(&bodyDef);

        b2FixtureDef fixtureDef;
        if (isCircle) {
            fixtureDef.shape = &circle;
        } else {
            fixtureDef.shape = &dynamicBox;
        }
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;

        body->CreateFixture(&fixtureDef);
    }
    birds_ = level.getBirds();
}

void LevelEditorScene::update(float ts)
{
    if(gui_.keyState(sf::Keyboard::Escape)){
        gui_.setScene<MenuScene>();
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
        auto[cx, cy] = gui_.cursorPosition();
        auto p = screen_to_world({cx, cy});
        if (p.x <= -4.5 && p.x >= -5.5 && p.y <= 2 && p.y >= 1) {
            
        }
    }
    std::string levelName;
    auto loaded_level = reader_writer_.readFile(levelName);
}