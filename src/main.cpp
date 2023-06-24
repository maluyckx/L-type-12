#include "Game/Application.hpp"
#include "Game/Vector2DServer.hpp"

/// Checks if we launched with -gfx
bool isGfxMode(int argc, char** argv) {
    bool     isGfx = false;
    for (int i     = 1; !isGfx && i < argc; ++i) {
        if (strcmp(argv[i], "-g") == 0) {
            isGfx = true;
        }
        if (strcmp(argv[i], "--graphical") == 0) {
            isGfx = true;
        }
    }
    return isGfx;
}

/// The entry point of the application
int main(int argc, char** argv) {
#ifdef SERVER
    Game::ApplicationType app_type = Game::ApplicationType::Server;
#else
    Game::ApplicationType app_type = Game::ApplicationType::Client;
#endif
    bool isGfx = isGfxMode(argc, argv);
    auto app   = Game::Application(app_type, isGfx);
    app.run();
    return 0;
}