#include "playScene.hpp"

#include <iostream>

#include "enemy.hpp"

PlayScene::PlayScene(GUI &gui, const Level& level)
    :
    Scene(gui),
    gravity_(0.0f, -10.0f),
    world_(gravity_),
    cam_x(0.f), cam_y(-5.f), cam_scale_x(15.f), cam_scale_y(15.f),
    grass_image_("res/grass.png"),
    enemy_bird_image_("res/enemy_bird.png"),
    bird_image_("res/test_bird.png"),
    state_(gameState::playing)
{
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0);
    groundBodyDef.userData.pointer = (uintptr_t)new userDataStruct{&grass_image_,
                                                                    bodyType::ground,
                                                                    NULL,
                                                                    NULL};
    b2Body* groundBody = world_.CreateBody(&groundBodyDef);
    
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, .5f);

    groundBody->CreateFixture(&groundBox, 0.0f);
    
    for(auto& ent : level.getEntities()){
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(ent->getX(), ent->getY());

        bodyType type = (*ent).getType();
        switch (type)
        {
        case bodyType::structure:
            
            break;
        case bodyType::ground:

            break;
        case bodyType::enemy:
            bodyDef.userData.pointer = (uintptr_t)new userDataStruct{
                &enemy_bird_image_,
                bodyType::enemy,
                ent,
                NULL};
            break;
        default:
            std::cout << "Default case reached, Could not match entity type: " << ent.get() << std::endl;
            break;
        }
        
        b2Body* body = world_.CreateBody(&bodyDef);
        
        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(.5f, .5f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;

        body->CreateFixture(&fixtureDef);
    }
    birds_ = level.getBirds();
}

PlayScene::~PlayScene()
{
    auto body = world_.GetBodyList();
    while(body){
        body = body->GetNext();
    }
}

void PlayScene::update(float ts)
{
    // Update
    int32 velocityIterations = 6;
    int32 positionIterations = 2;
    world_.Step(ts, velocityIterations, positionIterations);

    if(gui_.keyState(sf::Keyboard::A)){
        cam_x -= 2.f * ts;
    }
    if(gui_.keyState(sf::Keyboard::D)){
        cam_x += 2.f * ts;
    }
    if(gui_.keyState(sf::Keyboard::W)){
        cam_y -= 2.f * ts;
    }
    if(gui_.keyState(sf::Keyboard::S)){
        cam_y += 2.f * ts;
    }
    if(gui_.buttonState(sf::Mouse::Button::Left)){
        if(!drag_start_){
            auto[cx, cy] = gui_.cursorPosition();
            drag_start_ = screen_to_world({cx, cy});
        }
    }
    else{
        if(drag_start_){
            auto[cx, cy] = gui_.cursorPosition();
            auto vel = *drag_start_ - screen_to_world({cx, cy});
            vel.Normalize();
            auto p = screen_to_world({cx, cy});
            std::cout << p.x << " " << p.y << std::endl;
            launch_bird(screen_to_world({cx, cy}), {vel.x * 10.f, vel.y * 10.f});
            drag_start_ = {};
        }
    }
    // Win/Lose condition
    auto worldBody = world_.GetBodyList();
    int enemyCount = 0;
    int birdCount = 0;
    while (worldBody) {
        userDataStruct* data = (userDataStruct*)worldBody->GetUserData().pointer;
        if (data) {
            switch (data->type)
            {
            case bodyType::enemy:
                if (data->entity) {
                    enemyCount++;
                }
                break;
            case bodyType::bird:
                if (data->bird) {
                    birdCount++;
                }
                break;
            default:
                break;
            }
            worldBody = worldBody->GetNext();
        }
    }
    if (enemyCount == 0) {
        state_ = gameState::won;
    }
    else if (birdCount > 0) {
        state_ == gameState::playing;
    }
    else {
        state_ == gameState::lost;
    }

    // Render
    gui_.setViewport(cam_x, cam_y, cam_scale_x, cam_scale_y);

    // Entities
    auto body = world_.GetBodyList();
    while(body){
        auto pos = body->GetPosition();
        auto ang = body->GetAngle();
        userDataStruct* userData = (userDataStruct*)body->GetUserData().pointer;
        if(userData) {
            Image* img = userData->image;
            if(img != nullptr) {
                gui_.drawSprite(pos.x, pos.y, 1, 1, ang, *img);
            }
        }
        body = body->GetNext();
    }

    for(int i = -50; i < 50; i++){
        gui_.drawSprite(i, 0.f, 1.f, 1.f, 0.f, grass_image_);
    }
}

void PlayScene::launch_bird(b2Vec2 pos, b2Vec2 velocity) {
    if (!birds_.empty()) {
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(pos.x, pos.y);

        std::shared_ptr<Bird> bird = birds_.front();
        birds_.erase(birds_.begin());

        Image* image;

        if (auto normalBird = std::dynamic_pointer_cast<NormalBird>(bird)) {
            image = normalBird->getImage();
        }
        else if (auto specialBird1 = std::dynamic_pointer_cast<SpecialBird1>(bird)) {
            image = specialBird1->getImage();
        }
        else if (auto specialBird2 = std::dynamic_pointer_cast<SpecialBird2>(bird)) {
            image = specialBird2->getImage();
        }

        bodyDef.userData.pointer = (uintptr_t)new userDataStruct{
        image,
        bodyType::bird,
        NULL,
        bird
        };

        b2Body* body = world_.CreateBody(&bodyDef);

        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(.5f, .5f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;

        body->CreateFixture(&fixtureDef);

        body->SetLinearVelocity({velocity.x, velocity.y});
    }
    else {
        std::cout << "No more birds left!" << std::endl;
    }
}

b2Vec2 PlayScene::screen_to_world(b2Vec2 pos){
    return {0.5f * (pos.x * 2.f - 1.f) * cam_scale_x + cam_x, 0.5f * (pos.y * 2.f - 1.f) * cam_scale_y - cam_y};
}
