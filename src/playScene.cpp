#include "playScene.hpp"

#include <iostream>

PlayScene::PlayScene(GUI &gui, const Level& level)
    :
    Scene(gui),
    gravity_(0.0f, -10.0f),
    world_(gravity_),
    cam_x(0.f), cam_y(-5.f), cam_scale_x(15.f), cam_scale_y(15.f),
    grass_image_("res/grass.png"),
    bird_image_("res/test_bird.png")
{
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0);

    b2Body* groundBody = world_.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, .5f);

    groundBody->CreateFixture(&groundBox, 0.0f);

    for(auto& ent : level.getEntities()){
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(ent->getX(), ent->getY());
        b2Body* body = world_.CreateBody(&bodyDef);

        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(.5f, .5f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;

        body->CreateFixture(&fixtureDef);
    }
}

PlayScene::~PlayScene()
{}

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

    // Render
    gui_.setViewport(cam_x, cam_y, cam_scale_x, cam_scale_y);

    // Entities
    auto body = world_.GetBodyList();
    while(body){
        auto pos = body->GetPosition();
        auto ang = body->GetAngle();
        gui_.drawSprite(pos.x, pos.y, 1, 1, ang, bird_image_);

        body = body->GetNext();
    }

    for(int i = -50; i < 50; i++){
        gui_.drawSprite(i, 0.f, 1.f, 1.f, 0.f, grass_image_);
    }
}

void PlayScene::launch_bird(b2Vec2 pos, b2Vec2 velocity) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(pos.x, pos.y);

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

b2Vec2 PlayScene::screen_to_world(b2Vec2 pos){
    return {0.5f * (pos.x * 2.f - 1.f) * cam_scale_x + cam_x, 0.5f * (pos.y * 2.f - 1.f) * cam_scale_y - cam_y};
}
