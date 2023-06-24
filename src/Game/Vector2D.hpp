#ifndef LTYPE_GAME_VECTOR2D_H
#define LTYPE_GAME_VECTOR2D_H

#include <cstring>

namespace Game {
    /**
     * Represents a vector in 2 dimensions.
     * It can either used as a point (all points are a vector from the origin) or as a "physics" vector
     */
    class Vector2D {
    private:
        float _x = 0;
        float _y = 0;
    public:
        constexpr Vector2D() noexcept = default;

        constexpr Vector2D(float x, float y) : _x(x), _y(y) {};

        ~Vector2D() noexcept = default;

        inline constexpr void setX(float x) noexcept { _x = x; }

        inline constexpr void setY(float y) noexcept { _y = y; }

        inline constexpr float getX() noexcept { return _x; }

        inline constexpr float getY() noexcept { return _y; }

        static inline constexpr Vector2D zero() noexcept {
            return Vector2D(0, 0);
        }

        static inline constexpr Vector2D one() noexcept {
            return Vector2D(1, 1);
        }

        // direction are ala DirectX, top-left origine, Y down
        static inline constexpr Vector2D right() noexcept {
            return Vector2D(1, 0);
        }

        static inline constexpr Vector2D left() noexcept {
            return Vector2D(-1, 0);
        }

        static inline constexpr Vector2D up() noexcept {
            return Vector2D(0, -1);
        }

        static inline constexpr Vector2D down() noexcept {
            return Vector2D(0, 1);
        }

        inline constexpr Vector2D operator+(Game::Vector2D other) noexcept {
            return Vector2D(getX() + other.getX(),
                            getY() + other.getY());
        }

        inline constexpr Vector2D operator-() noexcept {
            return Vector2D(-getX(), -getY());
        }

        inline constexpr Vector2D operator-(Game::Vector2D other) noexcept {
            return *this + -other;
        }

        /**
         * Produit par un scalaire: Vect(1,1)* 2 = Vect(2,2)
         * https://fr.wikipedia.org/wiki/Multiplication_par_un_scalaire
         */
        inline constexpr Vector2D operator*(float coef) noexcept {
            return Vector2D(getX() * coef, _y * coef);
        }

        inline constexpr friend Vector2D operator*(float coef, Vector2D vec)
        noexcept {
            return vec * coef;
        }

        inline constexpr Vector2D operator/(float coef) noexcept {
            return *this * (1 / coef);
        }

        inline constexpr friend Vector2D operator/(float coef, Vector2D vec)
        noexcept {
            return vec / coef;
        }

        /// Dot product https://en.wikipedia.org/wiki/Dot_product
        inline constexpr float dot(Vector2D other) noexcept {
            return getX() * other.getX() + getY() * other.getY();
        }

        Vector2D& operator+=(Vector2D other) noexcept {
            _x += other.getX();
            _y += other.getY();
            return *this;
        }

        Vector2D& operator-=(Vector2D other) noexcept {
            _x -= other.getX();
            _y -= other.getY();
            return *this;
        }

        Vector2D& operator*=(float coef) noexcept {
            _x *= coef;
            _y *= coef;
            return *this;
        }

        Vector2D& operator/=(float coef) noexcept {
            return *this *= 1 / coef;
        }

        bool operator==(Vector2D other) noexcept {
            return _x == other._x && _y == other._y;
        }

        bool operator!=(Vector2D other) noexcept {
            return !operator==(other);
        }
    };
}


#endif
