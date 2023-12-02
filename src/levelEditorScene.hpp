#pragma once

#include "GUI.hpp"
#include "scene.hpp"
#include "image.hpp"
#include "level.hpp"
#include "structure.hpp"
#include "ground.hpp"

#include <optional>

struct userDataStruct {
    Image*                      image;
    bodyType                    type;
    std::shared_ptr<Entity>     entity;
    std::shared_ptr<Bird>       bird;
    int hp;
};

class LevelEditorScene : public Scene {
public:
    LevelEditorScene(GUI& gui, Level& level);

    ~LevelEditorScene();

    void update(float ts) override;
private:
    b2Vec2 screen_to_world(b2Vec2 pos);
    void exit_to_menu();

    ReaderWriter reader_writer_;
    Level level_;
    Image grass_image_, enemy_bird_image_, bird_image_, strcture_image_, sling_image_;
    float cam_x, cam_y, cam_scale_x, cam_scale_y;
    std::vector<std::shared_ptr<Bird>> birds_;

};