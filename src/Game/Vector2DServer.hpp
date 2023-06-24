#ifndef LTYPE_GAME_VECTOR2DSERVER_H
#define LTYPE_GAME_VECTOR2DSERVER_H

#include "Vector2D.hpp"

namespace Game {
    /**
     * This class is responsible to interpolate whatever it's internal value is with another Vector,
     * with the goal of putting a vector closer to the vector it has internally.
     * The idea is that the server sends the new entity position via a command to the client,
     * the client (CommandExecutor) will update the "server_vector" with the value of the server's vector,
     * Each frame the Client will call "interpolate(local_vector)" on the server_vector and will return a new vector,
     * that is slightly closer in values to the server's vector, we can replace the old local_vector with the new one.
     */
    class Vector2DServer : public Vector2D {

    public:
        constexpr Vector2DServer() noexcept = default;

        Vector2DServer(const Vector2D& vector) : Vector2D(vector) {}

        Vector2D blend(Vector2D other, float alpha=0.5f);
        ~Vector2DServer() noexcept = default;
    };
}

#endif
