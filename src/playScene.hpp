#pragma once

#include "GUI.hpp"
#include "scene.hpp"
#include "image.hpp"
#include "level.hpp"
#include "structure.hpp"

#include <optional>

#include <box2d/b2_math.h>
#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_contact.h>

#include <box2d/b2_timer.h>

#include "readerWriter.hpp"

/**
 * @brief Different states the game can be in
 * 
 */
enum struct gameState {
    won,
    lost,
    playing
};

/**
 * @brief Physics entities hold a copy of userDataStruct they can reference
 */
struct userDataStruct {
    Image*                      image;
    bodyType                    type;
    std::shared_ptr<Entity>     entity;
    std::shared_ptr<Bird>       bird;
    int hp;
};

/**
 * @brief Enumeration for different types of explosions in the game
 */
enum struct explosionType {
    cloud,
    fireball
};

/**
 * @brief Structure to store data for each explosion instance 
 */
struct ExplosionData {
    b2Vec2 position;
    float time;
    explosionType type;
};

/**
 * @brief The game play scene to play the levels in the game.
 */
class PlayScene : public Scene {
public:
    /**
     * @brief Construct a new Play Scene object
     * 
     * @param gui Reference to GUI
     * @param level Level object to play
     * @param current_player Name of current player
     */
    PlayScene(GUI& gui, const Level& level, std::string current_player);
    /**
     * @brief Destroy the Play Scene object
     */
    ~PlayScene();

    /**
     * @brief Update and render everything required by the PlayScene
     * 
     * @param ts Timestep
     */
    void update(float ts) override;
private:
    void launch_bird(b2Vec2 pos, b2Vec2 vel);
    b2Vec2 screen_to_world(b2Vec2 pos);
    void spawn_explosion(b2Vec2 pos, explosionType type);
    void lose_sequence();
    void win_sequence();
    void destroy_bird(b2Body* birdBody);

    void add_score(int amt);

    int get_bird_count() const;
    std::string get_current_bird_type() const;
    int get_score() const;

    void retry_level();
    void exit_to_menu();
    void next_level();
    Image* get_bird_image(birdType type);
private:
    ReaderWriter writer_;
    std::optional<std::string> next_level_;
    std::string current_player_;
    int current_score_;
    Level level_;
    b2Vec2 gravity_;
    b2World world_;
    Image grass_image_, enemy_bird_image_, normal_bird_image_, yellow_bird_image_, blue_bird_image_, explosion_image_, cloud_image_, strcture_image_, sling_image_;
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