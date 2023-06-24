# How to build from terminal
# For client:
# cmake . & make
# For server:
# cmake . -DSERVER=1 & make
# apparently we will be forced to write a custome Makefile, since the VM they provide doesn't support Cmake '-_-
# maybe we can juste make the makefile call the cmake?
cmake_minimum_required(VERSION 3.16)
project(l_type_groupe_12_2020)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER "g++-10")
set(ROGGEMAN_FLAGS "-fcoroutines -fconcepts -pthread -fopenacc -fopenmp -ggdb3 -Wpedantic -Wall -Wextra -Wconversion -Wsign-conversion -Weffc++ -Wstrict-null-sentinel -Wold-style-cast -Wnoexcept -Wctor-dtor-privacy -Woverloaded-virtual -Wsign-promo -Wzero-as-null-pointer-constant -Wsuggest-override")
set(NCURSES_FLAG "-lncurses")
set(SFML_FLAG "-lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROGGEMAN_FLAGS} ${NCURSES_FLAG} ${SFML_FLAG}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ROGGEMAN_FLAGS} ${NCURSES_FLAG} ${SFML_FLAG}")

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

find_package(SQLite3 REQUIRED)
include_directories(${SQLite3_INCLUDE_DIRS})

find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})

add_executable(
        l_type_groupe_12_2020
        src/main.cpp
		
        src/Game/Abstract/AppInstance.hpp
        src/Game/Abstract/Boss.cpp
        src/Game/Abstract/Boss.hpp
        src/Game/Abstract/Destructable.hpp
        src/Game/Abstract/EnemyShip.cpp
        src/Game/Abstract/EnemyShip.hpp
        src/Game/Abstract/EntityType.hpp
        src/Game/Abstract/GameEntity.cpp
        src/Game/Abstract/GameEntity.hpp
        src/Game/Abstract/Ship.cpp
        src/Game/Abstract/Ship.hpp
		
        src/Game/Application.cpp
        src/Game/Application.hpp
        src/Game/Bonus.cpp
        src/Game/Bonus.hpp
        src/Game/Command.cpp
        src/Game/Command.hpp
        src/Game/Connection.cpp
        src/Game/Connection.hpp
        src/Game/GameSettings.cpp
        src/Game/GameSettings.hpp
        src/Game/GameState.cpp
        src/Game/GameState.hpp
        src/Game/HUD.cpp
        src/Game/HUD.hpp
        src/Game/Input.cpp
        src/Game/Input.hpp
        src/Game/Lootable.cpp
        src/Game/Lootable.hpp
        src/Game/PlayerShip.cpp
        src/Game/PlayerShip.hpp
        src/Game/Projectile.cpp
        src/Game/Projectile.hpp
        src/Game/Vector2D.hpp
        src/Game/Vector2DServer.cpp
        src/Game/Vector2DServer.hpp

        src/Game/Client/Drawable.cpp
        src/Game/Client/Drawable.hpp
        src/Game/Client/GameClient.cpp
        src/Game/Client/GameClient.hpp
        src/Game/Client/GameplayWindow.cpp
        src/Game/Client/GameplayWindow.hpp
        src/Game/Client/Sound.cpp
        src/Game/Client/Sound.hpp
        src/Game/Client/Window.cpp
        src/Game/Client/Window.hpp
        
        src/Game/Client/Ncurses/Background.cpp
        src/Game/Client/Ncurses/Background.hpp
        src/Game/Client/Ncurses/TerminalWindow.cpp
        src/Game/Client/Ncurses/TerminalWindow.hpp

        src/Game/Client/SFML/GraphicalWindow.cpp
        src/Game/Client/SFML/GraphicalWindow.hpp
        src/Game/Client/SFML/ParticleHandler.cpp
        src/Game/Client/SFML/ParticleHandler.hpp

        src/Game/EnemyType/AsteroidBase.cpp
        src/Game/EnemyType/AsteroidBase.hpp
        src/Game/EnemyType/BattleShip.cpp
        src/Game/EnemyType/BattleShip.hpp
        src/Game/EnemyType/BossPoulpe.cpp
        src/Game/EnemyType/BossPoulpe.hpp
        src/Game/EnemyType/Cruiser.cpp
        src/Game/EnemyType/Cruiser.hpp
        src/Game/EnemyType/Dragon.cpp
        src/Game/EnemyType/Dragon.hpp
        src/Game/EnemyType/Enterprise.cpp
        src/Game/EnemyType/Enterprise.hpp
        src/Game/EnemyType/Escorter.cpp
        src/Game/EnemyType/Escorter.hpp
        src/Game/EnemyType/LaserGun.cpp
        src/Game/EnemyType/LaserGun.hpp
        src/Game/EnemyType/Obstacle.cpp
        src/Game/EnemyType/Obstacle.hpp
        src/Game/EnemyType/Swarmling.cpp
        src/Game/EnemyType/Swarmling.hpp

        src/Game/Server/CollisionResolver.cpp
        src/Game/Server/CollisionResolver.hpp
        src/Game/Server/GameInstance.cpp
        src/Game/Server/GameInstance.hpp
        src/Game/Server/GameServer.cpp
        src/Game/Server/GameServer.hpp
        src/Game/Server/GameSession.cpp
        src/Game/Server/GameSession.hpp
        src/Game/Server/Level.cpp
        src/Game/Server/Level.hpp
        src/Game/Server/Spawner.cpp
        src/Game/Server/Spawner.hpp

        src/Database/Database.cpp
        src/Database/Database.hpp
        src/Database/FriendRequest.hpp
        src/Database/User.hpp
        src/Database/User.cpp

        src/Utils/ColorsSFML.hpp
        src/Utils/Constants.hpp
        src/Utils/DebugLogger.hpp
        src/Utils/ThreadSafeQueue.hpp
        src/Utils/Time.hpp
        src/Utils/Time.cpp

        src/Menu/SubMenus/ConnectionMenu/ConnectionInfo.cpp
        src/Menu/SubMenus/ConnectionMenu/ConnectionInfo.hpp
        src/Menu/SubMenus/FriendProfileMenu.cpp
        src/Menu/SubMenus/FriendProfileMenu.hpp
        src/Menu/SubMenus/LeaderboardMenu.cpp
        src/Menu/SubMenus/LeaderboardMenu.hpp
        src/Menu/SubMenus/MyProfileMenu.cpp
        src/Menu/SubMenus/MyProfileMenu.hpp
        src/Menu/SubMenus/RoomMenu.cpp
        src/Menu/SubMenus/RoomMenu.hpp
        src/Menu/SubMenus/SocialMenu.cpp
        src/Menu/SubMenus/SocialMenu.hpp
        src/Menu/SubMenus/EndMenu.cpp
        src/Menu/SubMenus/EndMenu.hpp
        src/Menu/Window/Border.cpp
        src/Menu/Window/Border.hpp
        src/Menu/Window/Button.cpp
        src/Menu/Window/Button.hpp
        src/Menu/Window/MenuMaker.cpp
        src/Menu/Window/MenuMaker.hpp
        src/Menu/Window/PosObj.cpp
        src/Menu/Window/PosObj.hpp
        src/Menu/Window/Rectangle.hpp
        src/Menu/Window/TextInput.cpp
        src/Menu/Window/TextInput.hpp
        src/Menu/Window/Win.cpp
        src/Menu/Window/Win.hpp
        src/Menu/Window/Window.cpp
        src/Menu/Window/Window.hpp
        src/Menu/Window_SFML/Background.cpp
        src/Menu/Window_SFML/Background.hpp
        src/Menu/Window_SFML/Button.cpp
        src/Menu/Window_SFML/Button.hpp
        src/Menu/Window_SFML/SpriteSimple.cpp
        src/Menu/Window_SFML/SpriteSimple.hpp
        src/Menu/Window_SFML/Stars.cpp
        src/Menu/Window_SFML/Stars.hpp
        src/Menu/Window_SFML/Texte.cpp
        src/Menu/Window_SFML/Texte.hpp
        src/Menu/Window_SFML/TexteField.cpp
        src/Menu/Window_SFML/TexteField.hpp
        src/Menu/BaseMenu.cpp
        src/Menu/BaseMenu.hpp
        src/Menu/BaseMenuSFML.cpp
        src/Menu/BaseMenuSFML.hpp
        src/Menu/BaseMenuNcurses.cpp
        src/Menu/BaseMenuNcurses.hpp
        src/Menu/MenuInfo.hpp
        src/Menu/MenuInfo.cpp
        src/Menu/WindowMenu.cpp
        src/Menu/WindowMenu.hpp

        src/Menu/SubMenus/ConnectionMenu/Ncurses/ConnectionInfoNcurses.cpp
        src/Menu/SubMenus/ConnectionMenu/Ncurses/ConnectionInfoNcurses.hpp
        src/Menu/SubMenus/ConnectionMenu/Ncurses/ConnectionMenuNcurses.cpp
        src/Menu/SubMenus/ConnectionMenu/Ncurses/ConnectionMenuNcurses.hpp
        src/Menu/SubMenus/ConnectionMenu/Ncurses/LoginMenuNcurses.cpp
        src/Menu/SubMenus/ConnectionMenu/Ncurses/LoginMenuNcurses.hpp
        src/Menu/SubMenus/ConnectionMenu/Ncurses/NewPasswordMenuNcurses.cpp
        src/Menu/SubMenus/ConnectionMenu/Ncurses/NewPasswordMenuNcurses.hpp
        src/Menu/SubMenus/ConnectionMenu/Ncurses/QuestionVerificationMenuNcurses.cpp
        src/Menu/SubMenus/ConnectionMenu/Ncurses/QuestionVerificationMenuNcurses.hpp
        src/Menu/SubMenus/ConnectionMenu/Ncurses/RegisterMenuNcurses.cpp
        src/Menu/SubMenus/ConnectionMenu/Ncurses/RegisterMenuNcurses.hpp
        src/Menu/SubMenus/ConnectionMenu/Ncurses/UserNameVerificationMenuNcurses.cpp
        src/Menu/SubMenus/ConnectionMenu/Ncurses/UserNameVerificationMenuNcurses.hpp
        src/Menu/SubMenus/Ncurses/FriendProfileMenuNcurses.cpp
        src/Menu/SubMenus/Ncurses/FriendProfileMenuNcurses.hpp
        src/Menu/SubMenus/Ncurses/LeaderboardMenuNcurses.cpp
        src/Menu/SubMenus/Ncurses/LeaderboardMenuNcurses.hpp
        src/Menu/SubMenus/Ncurses/ProfileMenuNcurses.cpp
        src/Menu/SubMenus/Ncurses/ProfileMenuNcurses.hpp
        src/Menu/SubMenus/Ncurses/MainMenuNcurses.cpp
        src/Menu/SubMenus/Ncurses/MainMenuNcurses.hpp
        src/Menu/SubMenus/Ncurses/MyProfileMenuNcurses.cpp
        src/Menu/SubMenus/Ncurses/MyProfileMenuNcurses.hpp
        src/Menu/SubMenus/Ncurses/RoomMenuNcurses.cpp
        src/Menu/SubMenus/Ncurses/RoomMenuNcurses.hpp
        src/Menu/SubMenus/Ncurses/SocialMenuNcurses.cpp
        src/Menu/SubMenus/Ncurses/SocialMenuNcurses.hpp
        src/Menu/SubMenus/Ncurses/EndMenuNcurses.cpp
        src/Menu/SubMenus/Ncurses/EndMenuNcurses.hpp
        src/Menu/SubMenus/Ncurses/ErrorMenuNcurses.cpp
        src/Menu/SubMenus/Ncurses/ErrorMenuNcurses.hpp
        src/Menu/SubMenus/Ncurses/LevelFriendMenuNcurses.cpp
        src/Menu/SubMenus/Ncurses/LevelFriendMenuNcurses.hpp

        src/Menu/SubMenus/ConnectionMenu/SFML/ConnectionInfoSFML.cpp
        src/Menu/SubMenus/ConnectionMenu/SFML/ConnectionInfoSFML.hpp
        src/Menu/SubMenus/ConnectionMenu/SFML/ConnectionMenuSFML.cpp
        src/Menu/SubMenus/ConnectionMenu/SFML/ConnectionMenuSFML.hpp
        src/Menu/SubMenus/ConnectionMenu/SFML/LoginMenuSFML.cpp
        src/Menu/SubMenus/ConnectionMenu/SFML/LoginMenuSFML.hpp
        src/Menu/SubMenus/ConnectionMenu/SFML/NewPasswordMenuSFML.cpp
        src/Menu/SubMenus/ConnectionMenu/SFML/NewPasswordMenuSFML.hpp
        src/Menu/SubMenus/ConnectionMenu/SFML/QuestionVerificationMenuSFML.cpp
        src/Menu/SubMenus/ConnectionMenu/SFML/QuestionVerificationMenuSFML.hpp
        src/Menu/SubMenus/ConnectionMenu/SFML/RegisterMenuSFML.cpp
        src/Menu/SubMenus/ConnectionMenu/SFML/RegisterMenuSFML.hpp
        src/Menu/SubMenus/ConnectionMenu/SFML/UserNameVerificationMenuSFML.cpp
        src/Menu/SubMenus/ConnectionMenu/SFML/UserNameVerificationMenuSFML.hpp
        src/Menu/SubMenus/SFML/FriendProfileMenuSFML.cpp
        src/Menu/SubMenus/SFML/FriendProfileMenuSFML.hpp
        src/Menu/SubMenus/SFML/LeaderboardMenuSFML.cpp
        src/Menu/SubMenus/SFML/LeaderboardMenuSFML.hpp
        src/Menu/SubMenus/SFML/MainMenuSFML.cpp
        src/Menu/SubMenus/SFML/MainMenuSFML.hpp
        src/Menu/SubMenus/SFML/MyProfileMenuSFML.cpp
        src/Menu/SubMenus/SFML/MyProfileMenuSFML.hpp
        src/Menu/SubMenus/SFML/RoomMenuSFML.cpp
        src/Menu/SubMenus/SFML/RoomMenuSFML.hpp
        src/Menu/SubMenus/SFML/SocialMenuSFML.cpp
        src/Menu/SubMenus/SFML/SocialMenuSFML.hpp
        src/Menu/SubMenus/SFML/EndMenuSFML.cpp
        src/Menu/SubMenus/SFML/EndMenuSFML.hpp
        src/Menu/SubMenus/SFML/ErrorMenuSFML.cpp
        src/Menu/SubMenus/SFML/ErrorMenuSFML.hpp
        src/Menu/SubMenus/SFML/LevelFriendMenuSFML.cpp
        src/Menu/SubMenus/SFML/LevelFriendMenuSFML.hpp

        src/Menu/SubMenus/LevelMenu/LevelMainMenu.cpp
        src/Menu/SubMenus/LevelMenu/LevelMainMenu.hpp
        src/Menu/SubMenus/LevelMenu/LevelCreateMenu.cpp
        src/Menu/SubMenus/LevelMenu/LevelCreateMenu.hpp
        src/Menu/SubMenus/LevelMenu/LevelChoiceSettingsMenu.cpp
        src/Menu/SubMenus/LevelMenu/LevelChoiceSettingsMenu.hpp
)

target_link_libraries(l_type_groupe_12_2020 ${CURSES_LIBRARIES} ${SQLite3_LIBRARIES} sfml-system sfml-window sfml-graphics sfml-audio)
# adds DEBUG symbole when compiling in debug mode
target_compile_definitions(l_type_groupe_12_2020 PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")

if (DEFINED SERVER)
    add_definitions(-DSERVER)
    # sets the name of the executable to "ltype_game_server"
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "ltype_game_server")
else ()
    # sets the name of the executable to "ltype_game_client"
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "ltype_game_client")
endif ()
