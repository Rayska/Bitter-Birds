#include "GUI.hpp"
#include "nameEntryScene.hpp"

int main() {
    GUI app;

    app.set_scene<NameEntryScene>();

    app.run();
    return 0;
}