#include "Level.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../../Utils/DebugLogger.hpp"

bool Game::Level::fileExists(const std::string& path) {
	std::ifstream f(path, std::ifstream::in);
	if(f)
		return true;
	else
		return false;
}

bool Game::Level::retrieveLevel(const std::string& path){
    std::ifstream f(path, std::ifstream::in);  // open file from path for reading
    if(f) {
        std::string ligne;
        while(getline(f, ligne)){  // reading the file line by line

            // retrieves the 3 data of the line to put them in 3 distinct string variables
            const std::string& str_line = ligne;
            std::istringstream iss(str_line);
            std::string time;
            std::string pos_x;
            std::string type;
            iss >> type;
            iss >> time;
            iss >> pos_x;

            // stoi() convert string to int
            EntityType entity;
            entity = static_cast<EntityType>(std::stoi(type)); // atoi(type.c_str())

            _spawn.emplace_back(std::stoi(time), entity, std::stoi(pos_x)); // place the retrieved data in the tuple vector

        }
		return true;

    }else{
        DEBUGERR("Erreur: Impossible d'ouvrir le fichier en lecture.");
    	return false;
    }
}