#include "Vector2DServer.hpp"

Game::Vector2D Game::Vector2DServer::blend(Vector2D other, float alpha) {
    return (1-alpha) * *this + alpha * other;
}
