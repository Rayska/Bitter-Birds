#pragma once

#include <vector>

#include "entity.hpp"

#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_math.h>

class Ground: public Entity {
public:
    Ground(std::vector<b2Vec2> points, double x, double y) : Entity(false, false, 1, 0, x, y) {
        shape_.Set(points.data(), points.size());
    }

    bodyType getType() const override {
        return bodyType::ground;
    }

    b2PolygonShape getShape() const {
        return shape_;
    }

    bool contains(double x, double y) {
        return false;
    } 

private:
    b2PolygonShape shape_;
};
