

#include "GUI.hpp"
#include "menuScene.hpp"

int main() {
    // BOX2D testing
    /*
    b2Vec2 gravity(0.0f, -10.0f);
    b2World world(gravity);

    

    float timeStep = 1.0f / 60.0f;

    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    for (int32 i = 0; i < 60; ++i)
    {
        world.Step(timeStep, velocityIterations, positionIterations);
        b2Vec2 position = body->GetPosition();
        float angle = body->GetAngle();
        printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
    }
    */

    GUI app;

    app.setScene<MenuScene>();

    app.run();
   
    return 0;
}