#include "playScene.hpp"

#include <iostream>

#include "menuScene.hpp"
#include "enemy.hpp"

PlayScene::PlayScene(GUI &gui, const Level& level, std::string current_player)
    :
    Scene(gui),
    current_player_(current_player),
    level_(level),
    gravity_(0.0f, -10.0f),
    world_(gravity_),
    cam_x(0.f), cam_y(-6.f), cam_scale_x(15.f), cam_scale_y(15.f),
    grass_image_("res/grass.png"),
    enemy_bird_image_("res/enemy_bird.png"),
    bird_image_("res/test_bird.png"),
    strcture_image_("res/wood.png"),
    explosion_image_("res/explosion.png"),
    cloud_image_("res/cloud.png"),
    sling_image_("res/slingshot.png"),
    state_(gameState::playing),
    endSoundCalled_(false),
    timer_(nullptr),
    added_score_(false),
    mostRecentBird_(nullptr),
    stopFollow_(false)
    {
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0);
    groundBodyDef.userData.pointer = (uintptr_t)new userDataStruct{&grass_image_,
                                                                    bodyType::ground,
                                                                    NULL,
                                                                    NULL};
    b2Body* groundBody = world_.CreateBody(&groundBodyDef);
    
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 0.5f);

    groundBody->CreateFixture(&groundBox, 0.0f);
    
    for(auto& ent : level.getEntities()){
        b2BodyDef bodyDef;
        bodyDef.position.Set(ent->getX(), ent->getY());

        bool isCircle = false;
        b2CircleShape circle;
        b2PolygonShape dynamicBox;

        bodyType type = (*ent).getType();
        switch (type)
        {
        case bodyType::structure:
            {
                bodyDef.type = b2_dynamicBody;
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

PlayScene::~PlayScene()
{
    auto body = world_.GetBodyList();
    while(body){
        delete (userDataStruct*)body->GetUserData().pointer;
        body = body->GetNext();
    }
    if (timer_) {
        delete timer_;
        timer_ = nullptr;
    }
}

void PlayScene::update(float ts)
{
    if(gui_.keyState(sf::Keyboard::Escape)){
        gui_.setScene<MenuScene>(current_player_);
    }
    
    // Update
    int32 velocityIterations = 6;
    int32 positionIterations = 2;
    world_.Step(ts, velocityIterations, positionIterations);

    if(gui_.keyState(sf::Keyboard::A)){
        cam_x -= 5.f * ts;
    }
    if(gui_.keyState(sf::Keyboard::D)){
        cam_x += 5.f * ts;
    }
    if(gui_.buttonState(sf::Mouse::Button::Left)){
        if(!drag_start_){
            auto[cx, cy] = gui_.cursorPosition();
            auto p = screen_to_world({cx, cy});
            if (p.x <= -4.5 && p.x >= -5.5 && p.y <= 2 && p.y >= 1) {
                drag_start_ = screen_to_world({cx, cy});
            } else {
                stopFollow_ = true;
            }
        }
    }
    else{
        if(drag_start_){
            auto[cx, cy] = gui_.cursorPosition();
            auto vel = *drag_start_ - screen_to_world({cx, cy});
            vel*=0.5;
            if (vel.Length() >= 1) {
                vel.Normalize();
            }
            auto p = screen_to_world({cx, cy});
            std::cout << p.x << " " << p.y << std::endl;
            launch_bird(b2Vec2(-5, 2), {vel.x * 10.f, vel.y * 10.f});
            drag_start_ = {};
        }
    }

    {
        std::vector<b2Body*> to_delete;

        auto currentBody = world_.GetBodyList();
        while(currentBody) {
            auto currentContact = currentBody->GetContactList();
            bool deleted = false;
            while(currentContact) {
                auto other = currentContact->other;

                userDataStruct* curData = (userDataStruct*)currentBody->GetUserData().pointer;
                userDataStruct* otherData = (userDataStruct*)other->GetUserData().pointer;

                if(curData && (curData->type == bodyType::enemy || curData->type == bodyType::structure) && otherData && otherData->type != bodyType::ground) {
                    auto manifold = currentContact -> contact -> GetManifold();
                    if (curData->type == bodyType::enemy) {
                        for (int i = 0; i < manifold->pointCount; ++i) {
                            b2ManifoldPoint point = manifold->points[i];

                            float normalImpulse = point.normalImpulse * 25;

                            curData->hp -= int(normalImpulse);
                            
                        }

                        if(!deleted && curData->hp < 0){
                            deleted = true;
                            to_delete.push_back(currentBody);
                        }
                    } else if (curData->type == bodyType::structure) {
                        for (int i = 0; i < manifold->pointCount; ++i) {
                            b2ManifoldPoint point = manifold->points[i];

                            float normalImpulse = point.normalImpulse * 8;

                            curData->hp -= int(normalImpulse);
                            
                        }

                        if(!deleted && curData->hp < 0){
                            deleted = true;
                            to_delete.push_back(currentBody);
                        }
                    }
                }

                currentContact = currentContact->next;
            }

            currentBody = currentBody->GetNext();
        }

        for(auto& td : to_delete){
            spawn_explosion(td->GetPosition(), explosionType::fireball);
            userDataStruct* data = (userDataStruct*)td->GetUserData().pointer;
            if (data->type==bodyType::structure) {
                 gui_.playSound("res/sounds/wood_smash_sound.wav", 50);
            } else if (data->type==bodyType::enemy) {
                 gui_.playSound("res/sounds/enemy_kill.wav", 50);
            }

            world_.DestroyBody(td);
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
                    if (data->bird->getTime().asSeconds() > 5) {
                        destroyBird(worldBody);
                    }
                    else {
                        birdCount++;
                    }
                }
                break;
            default:
                break;
            }
        }
        worldBody = worldBody->GetNext();
    }
    birdCount += birds_.size();
    if (enemyCount == 0) {
        state_ = gameState::won;
    }
    else if (birdCount > 0) {
        state_ = gameState::playing;
    }
    else {
        if (!timer_) {
            timer_ = new b2Timer();
        }
        if (timer_->GetMilliseconds() > 5000) {
            state_ = gameState::lost;
        }
    }

    if(gui_.buttonReleased(sf::Mouse::Button::Right)){
        if (mostRecentBird_ && !mostRecentAbilityUsed_) {
            userDataStruct* userData = (userDataStruct*)mostRecentBird_->GetUserData().pointer;

            switch (userData->bird->getBirdType())
            {
            case birdType::special1: {
                b2Vec2 vec = mostRecentBird_->GetLinearVelocity();
                vec.Normalize();
                mostRecentBird_->SetLinearVelocity(22.f * vec);
                break;
            }
            case birdType::special2 :{
                auto body = world_.GetBodyList();
                b2Vec2 birdPos = mostRecentBird_->GetPosition();
                spawn_explosion(birdPos, explosionType::fireball);
                while (body) {
                    b2Vec2 pos = body->GetPosition();
                    float distance = abs((birdPos - pos).Length());
                    if (distance < 3) {
                        b2Vec2 force = (pos - birdPos);
                        force.Normalize();
                        force *= (3 - distance) * 2;
                        body->ApplyLinearImpulseToCenter(force, true);
                    }
                    body = body->GetNext();
                }
                destroyBird(mostRecentBird_);
                break;
            }
            default:
                break;
            }
            mostRecentAbilityUsed_ = true;
        }
    }

    // Update explosions
    for(auto& expl : explosions_){
        expl.time += ts;
    }
    auto to_rem = std::remove_if(explosions_.begin(), explosions_.end(), [](auto& expl){
        return expl.time > 1.f / 5.f;
    });
    explosions_.erase(to_rem, explosions_.end());

    // Camera follows bird if bird still exists
    if (mostRecentBird_ && !stopFollow_) {
        cam_x = mostRecentBird_->GetPosition().x;
    }

    // Rendering
    {
        // Render world
        gui_.setViewport(cam_x, cam_y, cam_scale_x, cam_scale_y * gui_.getAspectRatio());

        // Draw slingshot
        gui_.drawSprite(-5, 1.5, 2.f, 2.f, 0.f, sling_image_);

        // Draw entities by iterating over body list in b2World
        auto body = world_.GetBodyList();
        while(body){
            auto pos = body->GetPosition();
            auto ang = body->GetAngle() * -57.2957795; // convert to degrees
            userDataStruct* userData = (userDataStruct*)body->GetUserData().pointer;
            if(userData) {
                Image* img = userData->image;
                if(img != nullptr) {
                    auto ent_structure = std::dynamic_pointer_cast<Structure>(userData->entity);
                    if(userData->type == bodyType::structure && ent_structure){
                        gui_.drawSprite(pos.x, pos.y, ent_structure->getWidth(), ent_structure->getHeight(), ang, *img);
                    }
                    else{
                        gui_.drawSprite(pos.x, pos.y, 1, 1, ang, *img);
                    }
                }
            }
            body = body->GetNext();
        }
        // Draw ground as 100 sequential grass squares
        for(int i = -50; i < 50; i++){
            gui_.drawSprite(i, 0.f, 1.f, 1.f, 0.f, grass_image_);
        }

        // Draw explosions
        for(auto& expl : explosions_){
            float t = 5.f * expl.time;
            float scale = -5.f * t * t * (t - 1.f);
            auto& img = expl.type == explosionType::cloud ?  cloud_image_ : explosion_image_;
            gui_.drawSprite(expl.position.x, expl.position.y, 0.1f + scale, 0.1f + scale, expl.time, img);
        }

        // UI 
        gui_.setViewport(0.5f, 0.5f, 1.f, 1.f);

        // Draw ui based on state
        
        switch(state_){
            case gameState::won:
            {
                winSequence();
                break;
            }
            case gameState::lost:
            { 
                loseSequence();
                break;
            }
            case gameState::playing:
            {
                gui_.drawText(0.05f, .95f, .1f, "Birds left: " + std::to_string(get_bird_count()), Alignment::LeftCenter);
                gui_.drawText(0.05f, .85f, .1f, "Current Bird: " + get_current_bird_type(), Alignment::LeftCenter);
                gui_.drawText(0.95f, .95f, .1f, "Score: " + std::to_string(get_score()), Alignment::RightCenter);
                break;
            }
        }
    }
}

void PlayScene::launch_bird(b2Vec2 pos, b2Vec2 velocity) {
    if (!birds_.empty() && state_ == gameState::playing) {
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(pos.x, pos.y);

        std::shared_ptr<Bird> bird = birds_.front();
        birds_.erase(birds_.begin());

        bodyDef.userData.pointer = (uintptr_t)new userDataStruct{
        bird->getImage(),
        bodyType::bird,
        NULL,
        bird,
        0
        };

        stopFollow_ = false;

        bird->resetTime();
        
        b2Body* body = world_.CreateBody(&bodyDef);
        mostRecentBird_ = body;
        mostRecentAbilityUsed_ = false;
        b2CircleShape circle;
        circle.m_radius = 0.5f;

        /*b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(.5f, .5f);*/

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &circle;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;

        body->CreateFixture(&fixtureDef);

        body->SetLinearVelocity({velocity.x, velocity.y});
    }
    else if (state_ != gameState::playing) {
        std::cout << "Game is no longer in progress!" << std::endl;        
    }
    else {
        std::cout << "No more birds left!" << std::endl;
    }
}

b2Vec2 PlayScene::screen_to_world(b2Vec2 pos){
    return {0.5f * (pos.x * 2.f - 1.f) * cam_scale_x + cam_x, 0.5f * (pos.y * 2.f - 1.f) * cam_scale_y - cam_y};
}

void PlayScene::spawn_explosion(b2Vec2 pos, explosionType type){
    explosions_.push_back(ExplosionData{ 
        {pos.x + 0.1f, pos.y + 0.2f},
        0.f, type
    });
    explosions_.push_back(ExplosionData{ 
        {pos.x - 0.2f, pos.y + 0.2f},
        0.f, type
    });
    explosions_.push_back(ExplosionData{ 
        {pos.x - 0.1f, pos.y - 0.1f},
        0.f, type
    });
}

int PlayScene::get_bird_count() const
{
    return birds_.size();
}

std::string PlayScene::get_current_bird_type() const
{
    if (!birds_.empty()) {
        switch (birds_.front()->getBirdType())
        {
        case birdType::normal:
            return "Red bird";
        case birdType::special1:
            return "Yellow bird";
        case birdType::special2:
            return "Blue bird";
        default:
            return "Unknown bird";
        }
    }
    return "None";
}

void PlayScene::destroyBird(b2Body* birdBody) {
    world_.DestroyBody(birdBody);
    gui_.playSound("res/sounds/bird_death.wav");
    spawn_explosion(birdBody->GetPosition(), explosionType::cloud);
    if (mostRecentBird_ == birdBody) {
        mostRecentBird_ = nullptr;
    }
}

int PlayScene::get_score() const
{
    return 1000;
}


void PlayScene::retry_level() {
    gui_.setScene<PlayScene>(level_, current_player_);
}

void PlayScene::exit_to_menu() {
    gui_.setScene<MenuScene>(current_player_);
}

void PlayScene::next_level() {
}

void PlayScene::winSequence()
{   
    if(!added_score_){
        int score = get_score();
        most_recent_score_ = score;
        level_.addScore(current_player_, score);
        added_score_ = true;
    }

    if (!endSoundCalled_) {
        endSoundCalled_ = true;
        gui_.playSound("res/sounds/win_sound.wav");
    }
    sf::Color color(0,255,0);
    gui_.drawText(0.5f, 0.8f, 0.2f, "Victory", Alignment::Center, color);

    std::vector<ScoreBoardEntry> scores = level_.getScores();

    gui_.drawText(0.5f, 0.7f, 0.15f, "Scoreboard", Alignment::Center);
    bool first_matching_highlighted = false;
    for(int i = 0; (i < 5) && (i < scores.size()); i++){
        auto& e = scores[i];
        gui_.drawText(0.2f, 0.6f - i * 0.07f, 0.1f, std::to_string(i + 1), Alignment::LeftCenter);
        if(e.name == current_player_ && e.score == most_recent_score_ && !first_matching_highlighted) {
            gui_.drawText(0.5f, 0.6f - i * 0.07f, 0.1f, e.name, Alignment::Center, sf::Color(50, 255, 50));
            first_matching_highlighted = true;
        }
        else{
            gui_.drawText(0.5f, 0.6f - i * 0.07f, 0.1f, e.name, Alignment::Center);
        }
        gui_.drawText(0.8f, 0.6f - i * 0.07f, 0.1f, std::to_string(e.score), Alignment::RightCenter);
    }

    if(gui_.drawButton("Retry", 0.2f, 0.2f, 0.2f, 0.1f)){
        retry_level();
    }
    if(gui_.drawButton("Menu", 0.5f, 0.2f, 0.2f, 0.1f)){
        exit_to_menu();
    }
    if(gui_.drawButton("Next", 0.8f, 0.2f, 0.2f, 0.1f)){
        next_level();
    }
}

void PlayScene::loseSequence() 
{
    if (!endSoundCalled_) {
        endSoundCalled_ = true;
        gui_.playSound("res/sounds/lose_sound.wav");
    }

    sf::Color color(255,0,0);
    gui_.drawText(0.5f,0.6f,0.2f, "Game Over", Alignment::Center, color);

    if(gui_.drawButton("Retry", 0.4f, 0.4f, 0.2f, 0.1f)){
        retry_level();
    }
    if(gui_.drawButton("Menu", 0.6f, 0.4f, 0.2f, 0.1f)){
        exit_to_menu();
    }
}