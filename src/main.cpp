#include "GUI.hpp"
#include "nameEntryScene.hpp"

int main() {
    GUI app;

    app.setScene<NameEntryScene>();

    app.run();
    return 0;
}