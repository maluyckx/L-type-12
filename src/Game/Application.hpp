#ifndef LTYPE_GAME_APPLICATION_H
#define LTYPE_GAME_APPLICATION_H

#include "Abstract/AppInstance.hpp"

namespace Game {
    enum class ApplicationType {
        Client,
        Server
    };

    /**
     * Logical entry point of the app
     */
    class Application {
    private:
        AppInstance* _instance;
    public:
        Application() = delete;
        explicit Application(ApplicationType app_type, bool isGfx) noexcept;
        void run();
    };
}

#endif
