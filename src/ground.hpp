#pragma once

#include "entity.hpp"
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_math.h>

class Ground: public Entity {
public:
    Ground(std::vector<b2Vec> points, double x, double y) : Entity(false, false, 1, 0, x, y) {
        shape.Set(points.begin(), points.size());
    }

    b2PolygonShape getShape() const {
        return shape_;
    }

private:
    b2PolygonShape shape;
}