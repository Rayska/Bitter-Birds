#pragma once

#include "GUI.hpp"
#include "scene.hpp"
#include "image.hpp"
#include "level.hpp"
#include "structure.hpp"
#include "readerWriter.hpp"

#include <box2d/b2_world.h>

#include <optional>
#include <vector>

class LevelEditorScene : public Scene {
public:
    LevelEditorScene(GUI& gui, Level& level, const std::string current_player);

    ~LevelEditorScene();

    void update(float ts) override;
private:
    b2Vec2 screen_to_world(b2Vec2 pos);
    void on_input(char c);

    ReaderWriter reader_writer_;
    Level level_;
    Image grass_image_, enemy_bird_image_, strcture_image_, sling_image_;
    float cam_x_, cam_y_, cam_scale_x_, cam_scale_y_;
    int input_, healthpoints_;
    float rotation_, width_, height_;
    bodyType created_;
    std::string current_player_, current_input_;
    std::vector<std::shared_ptr<Entity>> entities_; 
    std::shared_ptr<Entity> chosenEntity_;
    std::vector<std::shared_ptr<Bird>> birds_;

};