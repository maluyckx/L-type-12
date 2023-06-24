#ifndef LTYPE_LEVEL_H
#define LTYPE_LEVEL_H

#include "../Abstract/EntityType.hpp"
#include "../Abstract/GameEntity.hpp"
#include <iostream>
#include <vector>
#include <tuple>

namespace Game {
    class Level {
        std::vector<std::tuple<const int, Game::EntityType, int>> _spawn{};  // tuple vector

    public:
        Level() noexcept = default;
        explicit Level(const std::string& path) { retrieveLevel(path); }
		
		bool fileExists(const std::string& path);
		
        bool retrieveLevel(const std::string& path);  // reads a txt file corresponding to a level in order to retrieve the entities to be spawned
		
		void clear() { _spawn.clear(); }  // empty tuple vector
		
        inline unsigned long getSize() noexcept {return _spawn.size();}

        std::tuple<const int, Game::EntityType, int> operator[](std::size_t i) { return _spawn[i]; }

        ~Level() noexcept = default;
    };
}


#endif