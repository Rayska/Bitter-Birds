#pragma once

#include "GUI.hpp"
#include "scene.hpp"
#include "image.hpp"
#include "level.hpp"

#include <optional>

#include <box2d/b2_math.h>
#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>

class PlayScene : public Scene {
public:
    PlayScene(GUI& gui, const Level& level);
    ~PlayScene();

    void update(float ts) override;

    void launch_bird(b2Vec2 pos, b2Vec2 vel);
    b2Vec2 screen_to_world(b2Vec2 pos);
private:
    b2Vec2 gravity_;
    b2World world_;
    Image grass_image_, bird_image_;
    std::optional<b2Vec2> drag_start_;
    float cam_x, cam_y, cam_scale_x, cam_scale_y;
};