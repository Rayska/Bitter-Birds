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
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_contact.h>

#include <box2d/b2_timer.h>

#include "ReaderWriter.hpp"

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
    PlayScene(GUI& gui, const Level& level, std::string current_player);
    ~PlayScene();

    void update(float ts) override;
private:
    void launch_bird(b2Vec2 pos, b2Vec2 vel);
    b2Vec2 screen_to_world(b2Vec2 pos);
    void spawn_explosion(b2Vec2 pos, explosionType type);
    void loseSequence();
    void winSequence();
    void destroyBird(b2Body* birdBody);

    void add_score(int amt);

    int get_bird_count() const;
    std::string get_current_bird_type() const;
    int get_score() const;

    void retry_level();
    void exit_to_menu();
    void next_level();
private:
    ReaderWriter writer_;
    std::optional<std::string> next_level_;
    std::string current_player_;
    int current_score_;
    Level level_;
    b2Vec2 gravity_;
    b2World world_;
    Image grass_image_, enemy_bird_image_, bird_image_, explosion_image_, cloud_image_, strcture_image_, sling_image_;
    std::optional<b2Vec2> drag_start_;
    float cam_x_, cam_y_, cam_scale_x_, cam_scale_y_;
    gameState state_;
    std::vector<std::shared_ptr<Bird>> birds_;
    std::vector<ExplosionData> explosions_;
    b2Body* mostRecentBird_;
    bool mostRecentAbilityUsed_;
    bool endSoundCalled_;
    bool stopFollow_;
    bool resetCamera_;
    b2Timer* timer_;
    bool added_score_;
    int most_recent_score_;
};