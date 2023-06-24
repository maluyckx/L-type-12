#ifndef LTYPE_UTILS_CONSTANTS_H
#define LTYPE_UTILS_CONSTANTS_H

#include <string>

/// The number of keys the client can send to the server in one frame
constexpr int INPUT_MAX_SIZE           = 10;
/// The maximum size of the name a of a player, '\0' included
constexpr int USERNAME_MAX_SIZE        = 32;
/// The maximum size of the password, '\0' included
constexpr int PASSWORD_MAX_SIZE        = 32;
/// The maximum size of the secret question, '\0' included
constexpr int SECRET_QUESTION_MAX_SIZE = 64;
/// The maximum size of the secret answer, '\0' included
constexpr int SECRET_ANSWER_MAX_SIZE   = 64;
/// The maximum amount of friends a player can have
constexpr int MAX_FRIEND_COUNT         = 32;
/// The maximum amount of friend requests a player can have
constexpr int MAX_FRIENDREQUEST_COUNT  = 32;
/// The maximum number of players the leaderboard can show
constexpr int LEADERBOARD_MAX_COUNT    = 20;
/// The maximum number of levels a player can create
constexpr int LEVEL_MAX_COUNT          = 20;
/// The maximum size of the name of a level, '\0' included
constexpr int LEVEL_NAME_MAX_SIZE      = 32;
/// The maximum number of entity a level can have
constexpr int LEVEL_MAX_SIZE           = 1024;
/// The maximum size of the error message, '\0' included
constexpr int ERROR_MSG_SIZE           = 64;

/// The window's width in NCurse
constexpr int WIDTH_WINDOW = 126;
/// The window's height in NCurse
constexpr int HEIGHT_WINDOW = 42;

/// The window's width in SFML
constexpr float WIDTH_WINDOW_SFML = 1305.f;
/// The window's height in SFML
constexpr float HEIGHT_WINDOW_SFML = 870.f;

/// When set in HUD for boss hp, the boss' hp bar doesn't show up
constexpr int BOSS_HP_UNDEFINED = -1;

/// Scalar to convert X dimmensions from NCurse -> SFML
constexpr float GRAPHIC_X_MULTIPLIER = 10.385625f;
/// Scalar to convert Y dimmensions from NCurse -> SFML
constexpr float GRAPHIC_Y_MULTIPLIER = 20.5325f;

/// The number of random entity that are scolling in the background Menu SFML
constexpr int NB_ENTITY_BACKGROUND   = 31;

/// Normalizing factor for the player ship movement
constexpr float NORMALISING = 0.5f;

/// The thick rate of the server
constexpr unsigned int SERVER_TICK_TIME_MS = 18u;

/// "Unlimited" FPS for the client, used for the level editor"
constexpr int FPS = 1000;

/// Abstraction of the game keys
enum GAME_KEYS {
    UP_P1 = 3,
    DOWN_P1 = 2,
    LEFT_P1 = 4,
    RIGHT_P1 = 5,
    SHOOT_P1 = 10,

    UP_P2 = 122,
    DOWN_P2 = 115,
    LEFT_P2 = 113,
    RIGHT_P2 = 100,
    SHOOT_P2 = 32,

    PAUSE = 80,
    RESUME = 77,
    QUIT = 79
};

enum class TERMINAL_COLORS {
    BLACK = 0,
    RED = 1,
    GREEN = 2,
    YELLOW = 3,
    BLUE = 4,
    MAGENTA = 5,
    CYAN = 6,
    WHITE = 7
};

const std::string STYLE_BOSS_DRAGON = "     (\\               /)\n    __)\\             /(__\n   __)_ \\  (\\!~!/)  / _(__\n  __)_ `.\\  )d b(  /.' _(__\n___)_ `. \\\\(  _  )// .' _(___\n )_  `. \\  ((q_p))  / .'  _(_\n _)`.  \\  ,-)\\_/(-.  /  .'(_\n  _) ,-._/v/vvvvv\\v\\_,-. (_\n  _)///(._/v(___)v\\_.)\\\\\\(_\n   \\_ ___/v(_____)v\\___ _/\n     /vvv\\(_______)/vvv\\\n     \\vvv/v(_____)v\\vvv/  hjw\n     _\\vv\\\\v(___)v//vv/_\n    '>_`  \\`-._.-'/  '_<`\n    ' >_,-'       `-._<`";
const std::string STYLE_BOSS_POULPE   = "             .---.         ,,\n  ,,        /     \\       ;,,'\n ;, ;      (  o  o )      ; ;\n   ;,';,,,  \\  ~  /      ,; ;\n ,,,  ;,,,,;;,`   '-,;'''',,,'\n;,, ;,, ,,,,   ,;  ,,,'';;,,;''';\n     ;,,,;    ~~'  '';,,''',,;''''\n                         ''' ";
const std::string STYLE_BOSS_ASTEROIDBASE = "         ___---___\n      .--         --.\n    ./   ()      .-. \\.\n   /   o    .   (   )  \\\n  / .            '-'    \\\n | ()    .  O         .  |\n|                         |\n|    o           ()       |\n|       .--.          O   |\n | .   |    |            |\n  \\    `.__.'    o   .  /\n   \\                   /\n    `\\  o    ()      /'\n      `--_________--'\n";  // JT/jgs
const std::string STYLE_BOSS_ASTEROIDBASEOPEN = "         ___---___\n      .--         --.\n    ./   ()      .-. \\.\n   /   o    .   (   )  \\\n  / .            '-'    \\\n | ()    .  O         .  |\n|                         |\n|    o           ()       |\n|       .--.          O   |\n | .   |    |            |\n  \\    `.__.'    o   .  /\n   \\                   /\n    `\\  o    ()      /'\n      `\\            /'\n";  // JT/jgs
const std::string STYLE_BOSS_ENTERPRISE = "___________________          _-_\n\\==============_=_/ ____.---'---`---.____\n            \\_ \\    \\----._________.----/\n              \\ \\   /  /    `-_-'\n          __,--`.`-'..'-_\n         /____          ||\n              `--.____,-'";
const std::string STYLE_PLAYERSHIP    = " _/^\\_\n| \\_/ |\n   ^";
[[maybe_unused]]
const std::string STYLE_ENEMYSHIP     = "    _\n |_/ \\_|\n   \\~/\n";
const std::string STYLE_ENEMYBATTLESHIP = "    _\n |_/ \\_|\n   \\~/\n";
const std::string STYLE_ENEMYESCORTER = "\\o/\n '";
const std::string STYLE_ENEMYCRUISER   = "<[|]>";
const std::string STYLE_ENEMYSWARMLING = "{!}";
const std::string STYLE_ENEMYLASERGUN = "/|\\\n\\!/";
const std::string STYLE_OBSTACLE      = " _\n|_|";
[[maybe_unused]]
const std::string STYLE_BONUSDEFAULT  = "[*]";
const std::string STYLE_BONUSHEAL     = "[+]";
const std::string STYLE_BONUSGUN      = "[¬]";

const std::string NAME_SWARMLING       = "Swarmling";
const std::string NAME_BATTLESHIP      = "BattleShip";
const std::string NAME_CRUISER         = "Cruiser";
const std::string NAME_ESCORTER        = "Escorter";
const std::string NAME_LASERGUN        = "LaserGun";
const std::string NAME_OBSTACLE        = "Asteroid";
const std::string NAME_BOSS_ASTEROID   = "AsteroidBase";
[[maybe_unused]]
const std::string NAME_ASTEROID_BASE   = "AsteroidBase";
const std::string NAME_BOSS_ENTERPRISE = "BossEnterprise";
const std::string NAME_BOSS_POULPE     = "BossPoulpe";
const std::string NAME_BOSS_DRAGON     = "BossDragon";

const std::string PATH_SWARMLING = "sprites/spaceship/Enemy/swarmling";
const std::string PATH_BATTLESHIP = "sprites/spaceship/Enemy/battleship";
const std::string PATH_CRUISER = "sprites/spaceship/Enemy/cruiser";
const std::string PATH_ESCORTER = "sprites/spaceship/Enemy/escorter";
const std::string PATH_LASERGUN = "sprites/spaceship/Enemy/lasergun";
const std::string PATH_OBSTACLE = "sprites/background/objects/object_";
const std::string PATH_ASTEROID_BASE_CLOSE = "sprites/boss/asteroidbase-close";
const std::string PATH_ASTEROID_BASE_OPEN = "sprites/boss/asteroidbase-open";
const std::string PATH_ENTERPRISE = "sprites/boss/enterprise";
const std::string PATH_POULPE = "sprites/boss/poulpe";
const std::string PATH_DRAGON = "sprites/boss/dragon_";

const std::string DEFAULT_LEVEL = "campaign_0.txt";

#endif