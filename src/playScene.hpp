#pragma once

#include "GUI.hpp"
#include "scene.hpp"
#include "image.hpp"
#include "level.hpp"
#include "structure.hpp"
#include "ground.hpp"

#include <optional>

#include <box2d/b2_math.h>
#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_contact.h>

enum struct gameState {
    won,
    lost,
    playing
};

struct userDataStruct {
    Image*                      image;
    bodyType                    type;
    std::shared_ptr<Entity>     entity;
    std::shared_ptr<Bird>       bird;
    int hp;
};

enum struct explosionType {
    cloud,
    fireball
};

struct ExplosionData {
    b2Vec2 position;
    float time;
    explosionType type;
};

class PlayScene : public Scene {
public:
    PlayScene(GUI& gui, const Level& level);
    ~PlayScene();

    void update(float ts) override;
private:
    void launch_bird(b2Vec2 pos, b2Vec2 vel);
    b2Vec2 screen_to_world(b2Vec2 pos);
    void spawn_explosion(b2Vec2 pos, explosionType type);
    void loseSequence();
    void winSequence();

    int get_bird_count() const;
    std::string get_current_bird_type() const;
    int get_score() const;
private:
    b2Vec2 gravity_;
    b2World world_;
    Image grass_image_, enemy_bird_image_, bird_image_, explosion_image_, cloud_image_, strcture_image_;
    std::optional<b2Vec2> drag_start_;
    float cam_x, cam_y, cam_scale_x, cam_scale_y;
    gameState state_;
    std::vector<std::shared_ptr<Bird>> birds_;
    std::vector<ExplosionData> explosions_;
    b2Body* mostRecentBird;
    bool mostRecentAbilityUsed;
};