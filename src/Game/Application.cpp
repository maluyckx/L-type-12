#include "Application.hpp"
#include "Client/GameClient.hpp"
#include "Server/GameServer.hpp"

Game::Application::Application(ApplicationType app_type, bool isGfx) noexcept:
        _instance(
                app_type == ApplicationType::Client ?
                static_cast<AppInstance*>(new Client::GameClient(isGfx)) :
                static_cast<AppInstance*>(new Server::GameServer())
        ) {}

void Game::Application::run() {
    this->_instance->run();
}
