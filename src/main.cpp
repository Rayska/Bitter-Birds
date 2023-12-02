

#include "GUI.hpp"
#include "menuScene.hpp"

int main() {
    GUI app;

    app.setScene<MenuScene>();

    app.run();
    return 0;
}