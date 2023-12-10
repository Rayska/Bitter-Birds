#include "playScene.hpp"

#include <iostream>

#include "menuScene.hpp"
#include "enemy.hpp"

PlayScene::PlayScene(GUI &gui, const Level& level, std::string current_player)
    :
    Scene(gui),
    current_player_(current_player),
    next_level_(""),
    current_score_(0),
    level_(level),
    gravity_(0.0f, -10.0f),
    world_(gravity_),
    cam_x_(0.f), cam_y_(-7.f), cam_scale_x_(15.f), cam_scale_y_(15.f),
    grass_image_("res/grass.png"),
    enemy_bird_image_("res/enemy_bird.png"),
    normal_bird_image_("res/normal_bird.png"),
    blue_bird_image_("res/blue_bird.png"),
    yellow_bird_image_("res/yellow_bird.png"),
    strcture_image_("res/wood.png"),
    explosion_image_("res/explosion.png"),
    cloud_image_("res/cloud.png"),
    sling_image_("res/slingshot.png"),
    background_image("res/background.png"),
    state_(gameState::playing),
    endSoundCalled_(false),
    timer_(nullptr),
    added_score_(false),
    mostRecentBird_(nullptr),
    stopFollow_(false),
    resetCamera_(false)
{
    next_level_ = writer_.get_next_level(level.get_save_name());

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
    
    for(auto& ent : level.get_entities()){
        b2BodyDef bodyDef;
        bodyDef.position.Set(ent->get_x(), ent->get_y());

        bool isCircle = false;
        b2CircleShape circle;
        b2PolygonShape dynamicBox;

        bodyType type = (*ent).get_type();
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
                    ent_structure->get_health_points(),
                };
                
                dynamicBox.SetAsBox(.5f * ent_structure->get_width(), .5f * ent_structure->get_height());
                bodyDef.angle = ent_structure->get_rotation();
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
                    ent_enemy->get_health_points()
                };
                circle.m_radius = 0.3f;
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
    birds_ = level.get_birds();
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

void PlayScene::update(float ts) {
    if(gui_.key_state(sf::Keyboard::Escape)){
        gui_.set_scene<MenuScene>(current_player_);
    }
    
    // Update
    int32 velocityIterations = 6;
    int32 positionIterations = 2;
    world_.Step(ts, velocityIterations, positionIterations);

    // Camera movements
    if(gui_.key_state(sf::Keyboard::A)){
        stopFollow_ = true;
        resetCamera_ = false;
        cam_x_ -= 5.f * ts;
    }
    if(gui_.key_state(sf::Keyboard::D)){
        stopFollow_ = true;
        resetCamera_ = false;
        cam_x_ += 5.f * ts;
    }
    if(gui_.key_state(sf::Keyboard::R)){
        stopFollow_ = true;
        resetCamera_ = true;
    }

    // Update explosions
    for(auto& expl : explosions_){
        expl.time += ts;
    }
    auto to_rem = std::remove_if(explosions_.begin(), explosions_.end(), [](auto& expl){
        return expl.time > 1.f / 5.f;
    });
    explosions_.erase(to_rem, explosions_.end());

    if(gui_.button_state(sf::Mouse::Button::Left)){
        auto[cx, cy] = gui_.cursor_position();
        auto p = screen_to_world({cx, cy});
        if(!drag_start_){
            if (p.x <= -4.5 && p.x >= -5.5 && p.y <= 2 && p.y >= 1) {
                drag_start_ = b2Vec2(-5, 2);
                gui_.play_sound("res/sounds/slingshot_stretch.wav");
            } else {
                stopFollow_ = true;
            }
        }
    }
    else{
        if(drag_start_){
            gui_.play_sound("res/sounds/slingshot_release.wav");
            auto[cx, cy] = gui_.cursor_position();
            auto vel = *drag_start_ - screen_to_world({cx, cy});
            vel*=0.5;
            if (vel.Length() >= 1) {
                vel.Normalize();
            }
            auto p = screen_to_world({cx, cy});
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

                if(curData && (curData->type == bodyType::enemy || curData->type == bodyType::structure) && otherData) {
                    auto manifold = currentContact -> contact -> GetManifold();

                    float minimum_damage = 12;

                    if (curData->type == bodyType::enemy) {
                        for (int i = 0; i < manifold->pointCount; ++i) {
                            b2ManifoldPoint point = manifold->points[i];

                            float normalImpulse = point.normalImpulse * 15;
                            float damage = normalImpulse >= minimum_damage ? normalImpulse : 0;
                            
                            curData->hp -= int(damage);
                        }

                        if(!deleted && curData->hp < 0){
                            deleted = true;
                            to_delete.push_back(currentBody);
                        }
                    } else if (curData->type == bodyType::structure) {
                        for (int i = 0; i < manifold->pointCount; ++i) {
                            b2ManifoldPoint point = manifold->points[i];

                            float normalImpulse = point.normalImpulse * 15;
                            float damage = normalImpulse >= minimum_damage ? normalImpulse : 0;

                            curData->hp -= int(damage);
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
                 gui_.play_sound("res/sounds/wood_smash_sound.wav", 50);
            } else if (data->type==bodyType::enemy) {
                 gui_.play_sound("res/sounds/enemy_kill.wav", 50);
            }

            world_.DestroyBody(td);
            add_score(((float)(get_bird_count() + 1) / level_.get_birds().size()) * 200);
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
                        destroy_bird(worldBody);
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

    if(gui_.button_released(sf::Mouse::Button::Right)){
        if (mostRecentBird_ && !mostRecentAbilityUsed_) {
            userDataStruct* userData = (userDataStruct*)mostRecentBird_->GetUserData().pointer;

            switch (userData->bird->get_bird_type())
            {
            case birdType::special1: {
                b2Vec2 vec = mostRecentBird_->GetLinearVelocity();
                vec.Normalize();
                mostRecentBird_->SetLinearVelocity(17.f * vec);
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
                destroy_bird(mostRecentBird_);
                break;
            }
            default:
                break;
            }
            mostRecentAbilityUsed_ = true;
        }
    }

    

    // Camera follows bird if bird still exists, and the bird has reached the center of the camera - 2.3f
    if (mostRecentBird_ && !stopFollow_ && cam_x_ <= mostRecentBird_->GetPosition().x + 2.3f) {
        cam_x_ = mostRecentBird_->GetPosition().x + 2.3f;
    }

    // Smooth camera reset
    if (resetCamera_ ) {
        if (cam_x_ > -0.005f && cam_x_ < 0.005f) {
            cam_x_ = 0.f;
            resetCamera_ = false;
        }
        else {
            cam_x_ -= cam_x_ * 0.045f;
        }
    }
}

void PlayScene::render() {
    // Rendering
    {
        // Draw background
        gui_.set_viewport(0.5f, 0.5f, 1.f, 1.f);
        gui_.draw_background(background_image);

        // Render world
        gui_.set_viewport(cam_x_, cam_y_, cam_scale_x_, cam_scale_y_ * gui_.get_aspect_ratio());

        // Draw slingshot
        gui_.draw_sprite(-5, 1.5, 2.f, 2.f, 0.f, sling_image_);
        // Draw bird at slingshot
        if(!birds_.empty()){
            Image* image = get_bird_image(birds_.front()->get_bird_type());
            if(drag_start_.has_value()){
                auto[scx, scy] = gui_.cursor_position();
                auto[cx,cy] = screen_to_world({scx, scy});
                b2Vec2 delta{drag_start_->x - cx, drag_start_->y - cy};
                float maxLen = 2.f;
                if(delta.Length() > maxLen){
                    delta = (maxLen / delta.Length()) * delta;
                }
                gui_.draw_sprite(-5.f - delta.x, 2.f - delta.y, 1.f, 1.f, 0.f, *image);
            }
            else{
                gui_.draw_sprite(-5.f, 2.f, 1.f, 1.f, 0.f, *image);
            }
        }

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
                        gui_.draw_sprite(pos.x, pos.y, ent_structure->get_width(), ent_structure->get_height(), ang, *img);
                    }
                    else{
                        gui_.draw_sprite(pos.x, pos.y, 1, 1, ang, *img);
                    }
                }
            }
            body = body->GetNext();
        }
        // Draw ground as 100 sequential grass squares
        for(int i = -50; i < 50; i++){
            gui_.draw_sprite(i, 0.f, 1.f, 1.f, 0.f, grass_image_);
        }

        // Draw explosions
        for(auto& expl : explosions_){
            float t = 5.f * expl.time;
            float scale = -5.f * t * t * (t - 1.f);
            auto& img = expl.type == explosionType::cloud ?  cloud_image_ : explosion_image_;
            gui_.draw_sprite(expl.position.x, expl.position.y, 0.1f + scale, 0.1f + scale, expl.time, img);
        }

        // UI 
        gui_.set_viewport(0.5f, 0.5f, 1.f, 1.f);

        // Draw ui based on state
        
        switch(state_){
            case gameState::won:
            {
                win_sequence();
                break;
            }
            case gameState::lost:
            { 
                lose_sequence();
                break;
            }
            case gameState::playing:
            {
                gui_.draw_text(0.05f, .95f, .1f, "Birds left: " + std::to_string(get_bird_count()), Alignment::LeftCenter);
                gui_.draw_text(0.05f, .85f, .1f, "Current Bird: " + get_current_bird_type(), Alignment::LeftCenter);
                gui_.draw_text(0.95f, .95f, .1f, "Score: " + std::to_string(get_score()), Alignment::RightCenter);
                break;
            }
        }
    }
}

void PlayScene::add_score(int amount){
    if(state_ != gameState::playing)
        return;
    current_score_ += amount;
}

void PlayScene::launch_bird(b2Vec2 pos, b2Vec2 velocity) {
    if (!birds_.empty() && state_ == gameState::playing) {
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(pos.x, pos.y);

        std::shared_ptr<Bird> bird = birds_.front();
        birds_.erase(birds_.begin());

        bodyDef.userData.pointer = (uintptr_t)new userDataStruct{
            get_bird_image(bird->get_bird_type()),
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
        circle.m_radius = 0.3f;

        /*b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(.5f, .5f);*/

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &circle;
        fixtureDef.density = 3.0f;
        fixtureDef.friction = 0.3f;

        body->CreateFixture(&fixtureDef);

        body->SetLinearVelocity({velocity.x * 1.15f, velocity.y * 1.15f});
    }
    else if (state_ != gameState::playing) {
        std::cout << "Game is no longer in progress!" << std::endl;        
    }
    else {
        std::cout << "No more birds left!" << std::endl;
    }
}

b2Vec2 PlayScene::screen_to_world(b2Vec2 pos){
    return {0.5f * (pos.x * 2.f - 1.f) * cam_scale_x_ + cam_x_, 0.5f * (pos.y * 2.f - 1.f) * cam_scale_y_ - cam_y_};
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
        switch (birds_.front()->get_bird_type())
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

void PlayScene::destroy_bird(b2Body* birdBody) {
    world_.DestroyBody(birdBody);
    gui_.play_sound("res/sounds/bird_death.wav");
    spawn_explosion(birdBody->GetPosition(), explosionType::cloud);
    if (mostRecentBird_ == birdBody) {
        mostRecentBird_ = nullptr;
    }
}

int PlayScene::get_score() const {
    return current_score_;
}

void PlayScene::retry_level() {
    gui_.set_scene<PlayScene>(level_, current_player_);
}

void PlayScene::exit_to_menu() {
    gui_.set_scene<MenuScene>(current_player_);
    // Save scores
    writer_.write_file(level_, level_.get_save_name());
}

void PlayScene::next_level() {
    // Save scores
    writer_.write_file(level_, level_.get_save_name());
    // Reload scene with next level
    auto next = writer_.read_file(*next_level_);
    if(next){
        gui_.set_scene<PlayScene>(*next, current_player_);
    }
}

void PlayScene::win_sequence()
{   
    if(!added_score_){
        int score = get_score();
        most_recent_score_ = score;
        level_.add_score(current_player_, score);
        added_score_ = true;
    }

    if (!endSoundCalled_) {
        endSoundCalled_ = true;
        gui_.play_sound("res/sounds/win_sound.wav");
    }
    sf::Color color(0,255,0);
    gui_.draw_text(0.5f, 0.8f, 0.2f, "Victory", Alignment::Center, color);

    std::vector<ScoreBoardEntry> scores = level_.get_scores();

    gui_.draw_text(0.5f, 0.7f, 0.15f, "Scoreboard", Alignment::Center);
    bool first_matching_highlighted = false;
    for(int i = 0; (i < 5) && (i < scores.size()); i++){
        auto& e = scores[i];
        gui_.draw_text(0.2f, 0.6f - i * 0.07f, 0.1f, std::to_string(i + 1), Alignment::LeftCenter);
        if(e.name == current_player_ && e.score == most_recent_score_ && !first_matching_highlighted) {
            gui_.draw_text(0.5f, 0.6f - i * 0.07f, 0.1f, e.name, Alignment::Center, sf::Color(50, 255, 50));
            first_matching_highlighted = true;
        }
        else{
            gui_.draw_text(0.5f, 0.6f - i * 0.07f, 0.1f, e.name, Alignment::Center);
        }
        gui_.draw_text(0.8f, 0.6f - i * 0.07f, 0.1f, std::to_string(e.score), Alignment::RightCenter);
    }

    if(next_level_.has_value()){
        if(gui_.draw_button("Retry", 0.2f, 0.2f, 0.2f, 0.1f)){
            retry_level();
        }
        if(gui_.draw_button("Menu", 0.5f, 0.2f, 0.2f, 0.1f)){
            exit_to_menu();
        }
        if(gui_.draw_button("Next", 0.8f, 0.2f, 0.2f, 0.1f)){
            next_level();
        }
    }
    else{
        if(gui_.draw_button("Retry", 0.4f, 0.2f, 0.2f, 0.1f)){
            retry_level();
        }
        if(gui_.draw_button("Menu", 0.6f, 0.2f, 0.2f, 0.1f)){
            exit_to_menu();
        }
    }
}

void PlayScene::lose_sequence() 
{
    if (!endSoundCalled_) {
        endSoundCalled_ = true;
        gui_.play_sound("res/sounds/lose_sound.wav");
    }

    sf::Color color(255,0,0);
    gui_.draw_text(0.5f,0.6f,0.2f, "Game Over", Alignment::Center, color);

    if(gui_.draw_button("Retry", 0.4f, 0.4f, 0.2f, 0.1f)){
        retry_level();
    }
    if(gui_.draw_button("Menu", 0.6f, 0.4f, 0.2f, 0.1f)){
        exit_to_menu();
    }
}

Image* PlayScene::get_bird_image(birdType type) {
    switch (type)
    {
    case birdType::normal:
        return &normal_bird_image_;
    case birdType::special1:
        return &yellow_bird_image_;
    case birdType::special2:
        return &blue_bird_image_;
    default:
        return nullptr;
    }
}