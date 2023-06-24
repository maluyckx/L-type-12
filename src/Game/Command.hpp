#ifndef LTYPE_GAME_COMMAND_H
#define LTYPE_GAME_COMMAND_H

#include "../Utils/ThreadSafeQueue.hpp"
#include "Server/GameSession.hpp"
#include <ctime>
#include "Vector2D.hpp"
#include "../Utils/Constants.hpp"
#include "GameSettings.hpp"
#include "../Database/FriendRequest.hpp"
#include "Abstract/EntityType.hpp"
#include <fstream>
#include "Client/Sound.hpp"
#include "../Utils/Time.hpp"
#include "Client/SFML/ParticleHandler.hpp"

namespace Game {
    /**
     * A structure that represents the command, it can be written/read from a
     * pipe.
     * The union is the collection of possible command data, there should be
     * 1 struct inside for each CommandType possible.
     */
    struct Command {
        /**
         * An enum that represent the type the command, it is easier to use an
         * enum for serialization than a class generalization.
         */
        enum class Type {
            Undefined,
            Ping,
            Input,
            UpdateEntityPosition,
            CreateEntity,
            DestroyEntity,
            CreateAccount,
            AuthLogin,
            AuthLogout,
            AuthRestore,
            SendError,
            UserData,
            UserFriendList,
            FriendProfileInfo,
            Leaderboard,
            GameStatus,
            FriendRequest,
            UpdateHUD,
            CreateLevel,
            LikeLevel,
            UserLevelList,
            IsLiked,
            PlaySound,
            Particle
        } type = Type::Undefined;
        /**
         * - Ping:
         *  A command that makes a roundtrip to the server and displays
         *  the time that it has taken for the trip. Mostly for debug purposes
         *  and showing that the clients connects to the server.
         */
        struct Ping {
            Utils::StopWatch chrono;
        };
        /**
         *  - Input:
         *  A command that sends a list of inputs (chars) that were
         *  pressed in a frame by the client. The session interpretes them and
         *  takes the according gameplay actions.
         */
        struct Input {
            char chars[INPUT_MAX_SIZE];
        };
        /**
         * - UpdateEntityPosition:
         *  A command the server sends to the client after
         *  it has resolved all collisions. It contains the id of the entity
         *  to be updated and the value of the position on the server. The
         *  client reads that vector and sets the "server_vector" of the
         *  entity with this value.
         */
        struct UpdateEntityPosition {
            unsigned long id;
            Vector2D      position;
        };
        /**
         * - CreateEntity:
         *  A command the server sends to the client when it has
         *  created a new entity. Should send id/position/type_of_entity
         *  (obstacle, enemy1, enemy2, boss?)
         */
        struct CreateEntity {
            EntityType    type;
            unsigned long id;
            Vector2D      position;
            bool is_hostile;
        };
        /**
         * - DestroyEntity:
         *  A command the server sends to the client when it has
         *  destroyed an entity. Should just send the id of the entity in
         *  question. The client could play an animation when the entity is
         *  dying? (KISS: maybe not yet, only the boss?)
         */
        struct DestroyEntity {
            unsigned long id;
        };
        /**
         * - CreateAccount:
         *  A command that the client sends with the username + password +
         *  secret_question + secret_answer to the server. The initial state is
         *  "Request". The servers creates the accound in the DB and sets the state
         *  "Success" or "Failed" (duplicate username is "failed"). Sends back to
         *  the client, at which point it can return to login screen and proceed
         *  to login.
         */
        struct CreateAccount {
            enum class State {
                Request,
                Success,
                Failed,
            };
            State state;
            char  username[USERNAME_MAX_SIZE];
            char  password[PASSWORD_MAX_SIZE];
            char  secret_question[SECRET_QUESTION_MAX_SIZE];
            char  secret_answer[SECRET_ANSWER_MAX_SIZE];
        };
        /**
         * - AuthLogin:
         *  A command that the client sends with the username +
         *  password to the server. The command has a response field of type
         *  class enum AuthState that has the following states:
         *  1) Undefined: this is the starting state the clients sends to the
         *  server;
         *  2) Accepted: the server sets this when the login was successful;
         *  3) Denied: the servers sets this when the login was a failure;
         *  When the server has made the request to the DB and sets the response
         *  state accordingly, it sends it back to the client and he (client) can
         *  take actions.
         */
        struct AuthLogin {
            enum class State {
                Undefined,
                Accepted,
                Denied,
            };
            State state;
            char  username[USERNAME_MAX_SIZE];
            char  password[PASSWORD_MAX_SIZE];
        };
        /**
         * - AuthLogout:
         * A command that the client sends to logout a user from the server,
         * mostly used to logout the second player, since the first user can't
         * logout without quitting the game, which will clear the user from
         * the login table.
         */
        struct AuthLogout {
            char username[USERNAME_MAX_SIZE];
        };
        /**
         * - AuthRestore:
         *  A command that is used when a player has forgotten his
         *  password. It will make a few roundtrips between the client and the
         *  server for the process (just like AuthLogin, it has "states" to
         *  understand at which step of the process we are currently) :
         *  1) Clients sends the username that wishes to recover his account to
         *  server (the state is "RequestSecretQuestion").
         *  2) The server reads the db and populates the field "secret_question"
         *  with the question of the user in question. (the state is set to
         *  "ResponseSecretQuestion") Sends it back to the client.
         *  3) The client adds the answer in question to the command and sends it
         *  back to the server (the state is set to "CheckSecretAnswer")
         *  4) The server checks with the DB if it is the case. (if the answer
         *  was correct, set the state to "SecretAnswerAccepted", if the answer
         *  was false, set the state to "SecretAnswerDenied"). Send it back to
         *  the client.
         *  5) The Client reads the state if it SecretAnswerAccepted or
         *  SecretAnswerDenied and input new password. Set state to "NewPassword"
         *  and sends to server.
         *  6) The server update the DB with the new password.
         */
        struct AuthRestore {
            enum class State {
                RequestSecretQuestion,
                ResponseSecretQuestion,
                CheckSecretAnswer,
                SecretAnswerAccepted,
                SecretAnswerDenied,
                NewPassword,
            };
            State state;
            char  username[USERNAME_MAX_SIZE];
            char  secret_question[SECRET_QUESTION_MAX_SIZE];
            char  secret_answer[SECRET_ANSWER_MAX_SIZE];
            char  new_password[PASSWORD_MAX_SIZE];
        };
        /**
         * - SendError:
         * send a error to user
         */
        struct SendError {
            char error[ERROR_MSG_SIZE];
        };
        /**
         * - UserData:
         *  A command the client sends to request user information (we
         *  assume the client is already logged in). The command has a few states:
         *  1) Clients sends command with state "Request" and the username of the
         *  user.
         *  2) The server query the DB and sets the state to "Completed" or
         *  "NotFound" depending if the query was successful or not. If the query
         *   was ok, the server populated the fields of the query with the data in
         *  question. Sends command to client.
         *  3) Client interprets the state(Completed/NotFound) and acts accordingly.
         */
        struct UserData {
            enum class State {
                Request,
                Completed,
                NotFound,
            };
            State state;
            int   id;
            int   xp;
            char  username[USERNAME_MAX_SIZE];
            char  password[PASSWORD_MAX_SIZE];
            char  secret_question[SECRET_QUESTION_MAX_SIZE];
            char  secret_answer[SECRET_ANSWER_MAX_SIZE];
            char  friend_list[USERNAME_MAX_SIZE * MAX_FRIEND_COUNT];
            char  pending_friend_requests[
                      USERNAME_MAX_SIZE * MAX_FRIENDREQUEST_COUNT];
        };
        /**
         * - UserFriendList:
         *  A command the client sends to request user
         *  information
         *  (we assume the client is already logged in). The command has a states:
         *  1) Clients sends command with state "Request" and the username of the
         *  user.
         *  2) The server query the DB and sets the state to "Completed" or
         *  "NotFound" depending if the query was successful or not. If the query
         *  was ok, the server populated the fields of the query with a list of
         *  usernames that are friend with the requesting user.
         *  Sends command to client.
         *  3) The client interpretes the result depending on the state, then can
         *  make UserData command to get more info about a particular user.
         */
        struct UserFriendList {
            enum class State {
                Request,
                Completed,
                NotFound,
            };
            State    state;
            char     username[USERNAME_MAX_SIZE];
            unsigned friends_count;
            char     friend_list[USERNAME_MAX_SIZE * MAX_FRIEND_COUNT];
        };
        /** 
         * - FriendProfileInfo:
         * Retrieve info about the profile of a certain friend
        */
        struct FriendProfileInfo {
            enum class State {
                FRIEND,
                NOT_FRIEND,
                PENDING
            };
            char  from_username[USERNAME_MAX_SIZE];
            char  to_username[USERNAME_MAX_SIZE];
            State state;
            int   score;
        };
        /**
         * - Leaderboard:
         *  A command that request the leaderboard of the server:
         *  1) Client sends with state "Request" to server.
         *  2) Server query DB and set state to "Success" or "Failed" and
         *  populate the data (list of pair username + score) in the command.
         *  3) Client interprets it.
         */
        struct Leaderboard {
            enum class State {
                Request,
                Success,
                Failed,
            };
            State    state;
            unsigned top_player_count;
            char     username_list[USERNAME_MAX_SIZE * LEADERBOARD_MAX_COUNT];
            int      xps[LEADERBOARD_MAX_COUNT];
        };
        /**
         * - GameStatus:
         * A command that notify the session/client that a game has
         * started/finished, respectivly
         */
        struct GameStatus {
            enum class State {
                Starts,
                ClientInitialized,
                Ends,
                Quit,
                Pause,
                Resume,
            };
            State        state;
            GameSettings settings;
            char         username_player1[USERNAME_MAX_SIZE];
            char         username_player2[USERNAME_MAX_SIZE];
            int level_remain;
            int score_player1;
            int score_player2;
        };
        /**
         * - FriendRequest:
         * A command that the client sends to the server to notify that was
         * a request from user1 to user2 to be friends
         */
        struct FriendRequest {
            enum class State {
                Pending,
                Accepted,
                Denied,
                Delete
            };
            State state;
            char  from_username[USERNAME_MAX_SIZE];
            char  to_username[USERNAME_MAX_SIZE];
        };
        /**
         * - UpdateHUD:
         * A command that the server sends to the client to update the HUD
         */
        struct UpdateHUD {
            char username_player1[USERNAME_MAX_SIZE];
            char username_player2[USERNAME_MAX_SIZE];
            int  player1_hp;
            int  player1_score;
            int  player2_hp;
            int  player2_score;
            int  boss_hp;
            int  player_max_hp;
            int  boss_max_hp;
        };
        /**
         * - CreateLevel:
         * A command that can use the client for 2 things:
         *  1. Know if a name is already taken for a level (send Check State)
         *  2. Create the level
         */
        struct CreateLevel {
            enum class State {
                Created,    // Le niveau à bien été créé
                Error,      // Une erreur s'est produite (sauvegarde sur dique, ou autre pb avec le stream)
                Check,      // Client -> Serveur, demande si le nom existe déjà
                Doublon,    // Serveur -> Client, si le nom existe déjà
                Name_ok     // Serveur -> Client, si le nom n'existe pas déjà
            };
            State   state;
            char    username_creator[USERNAME_MAX_SIZE];
            char    name_level[LEVEL_NAME_MAX_SIZE];
            char    level[LEVEL_MAX_SIZE]; // TODO : Trouver une taille optimale
        };

        /**
         * - UserLevelList
         * Return to client all the level create by username_creator
         */
        struct UserLevelList {
            char     username_creator[USERNAME_MAX_SIZE];
            unsigned nb_level;
            char     level_list[LEVEL_NAME_MAX_SIZE * LEVEL_MAX_COUNT];
            int      level_like[LEVEL_MAX_COUNT];
        };

        /**
         * - LikeLevel
         * Give a like to a level
         */
        struct LikeLevel {
            char    username_liker[USERNAME_MAX_SIZE];
            char    level_name[LEVEL_NAME_MAX_SIZE];
        };

        struct IsLiked {
            char    username_liker[USERNAME_MAX_SIZE];
            char    level_name[LEVEL_NAME_MAX_SIZE];
            bool    liked = false;
        };


        /**
         * - PlaySound
         * A command that the server sends to the client to play a sound
         */
        struct PlaySound {
            Game::Client::SoundEffect sound;
            unsigned level_number;
        };

        /**
         * - Particle
         * A command that plays a VFX
         */
        struct Particle {
            SFML::Particle type;
            float x;
            float y;
            float w;
            float h;
            bool reverse;
        };

        union Data {
            Ping                 ping;
            Input                input;
            UpdateEntityPosition update_entity_position;
            CreateEntity         create_entity;
            DestroyEntity        destroy_entity;
            CreateAccount        create_account;
            AuthLogin            auth_login;
            AuthLogout           auth_logout;
            AuthRestore          auth_restore;
            SendError            send_error;
            UserData             user_data;
            UserFriendList       user_friendlist;
            FriendProfileInfo    friend_profile;
            Leaderboard          leaderboard;
            GameStatus           game_status;
            FriendRequest        friend_request;
            UpdateHUD            update_hud;
            CreateLevel          create_level;
            UserLevelList        user_level_list;
            LikeLevel            like_level;
            IsLiked              is_liked;
            PlaySound			 play_sound;
            Particle             particle;
        } data{};
    };

    /**
     * A class that is responsible to create command based on external events
     * and push it to the connection queue
     * (see more in doc for the class Connection)
     */
    class CommandFactory {
    public:
        /// serialize the command into the buffer, it consumes the command,
        /// so it is destroyed
        static void serialize(Command* command, char* buffer);
        /// deserialize the buffer into a new command object
        static Command* deserialize(char* buffer);
        /// create a command for Ping
        static Command* ping();
        /// create a command for UpdateEntityPosition
        static Command* updateEntityPosition(unsigned long id,
                                             Vector2D position);
        /// create a command for CreateAccount
        static Command* createAccount(char* username, char* password,
                                      char* secret_question,
                                      char* secret_answer);
        /// create a command for AuthLogin
        static Command* authLogin(char* username, char* password);
        /// create a command for AuthLogout
        static Command* authLogout(char* username);
        /// create a command for AuthRestore
        static Command* authRestore(Command::AuthRestore::State state,
                                    char* username, char* password,
                                    char* secret_question,
                                    char* secret_answer);
        /// Send error 
        static Command* sendError(char* error);
        /// create a command for GameStatus
        static Command* gameStatus(Command::GameStatus::State state,
                                   GameSettings settings,
                                   char* username_player1,
                                   char* username_player2,
                                   int score_player1,
                                   int score_player2,
                                   int level_remain);
        /// create a command for UserFriendList
        static Command* userFriendList(char* username);
        /// create a command for UserData
        static Command* userData(char* username);
        /// create a command for FriendRequest
        static Command* friendRequest(Command::FriendRequest::State state,
                                      char* from_username,
                                      char* to_username);
        /// create a command for FriendProfileInfo
        static Command* friendProfileInfo(char* from_username,
                                          char* to_username);
        /// create a command for Leaderboard
        static Command* leaderboard();
        /// create a command for CreateEntity
        static Command* createEntity(EntityType type,
                                     unsigned long id,
                                     Vector2D position,
                                     bool is_hostile=true);
        /// create a command for DestroyEntity
        static Command* destroyEntity(unsigned long id);
        /// create a command for Input
        static Command* input(char* inputs);
        /// create a command for UpdateHUD
        static Command* updateHUD(
            const char* username_player1,
            const char* username_player2,
            int player1_hp,
            int player1_score,
            int player2_hp,
            int player2_score,
            int boss_hp,
            int player_max_hp,
            int boss_max_hp
        );
        /// create a command for createLevel
        static Command* createLevel(
            const char* username_creator,
            const char* name_level,
            const char* level,
            bool check_name = false
        );
        /// create a command for userLevelList
        static Command* userLevelList(
            const char* username_creator
        );
        /// create a command for likeLevel 
        static Command* likeLevel(
            const char* username_liker,
            const char* name_level
        );
        static Command* isliked(
            const char* username_liker,
            const char* name_level
        );
        /// create a command for PlaySound
        static Command* playSound(Game::Client::SoundEffect sound, unsigned level_number = 0);
        /// create a command for Particle
        static Command* makeParticle(SFML::Particle type, float x=0,
                                 float y=0, float w=0, float h=0, bool is_hostile = true);
    };

    /**
     * A class that is reponsible for executing a command, based on its type
     * and the data in it, taken from the queue
     * of the connection. (see more in doc for the class Connection)
     */
    class CommandExecutor {
    public:
        static void execute(Connection* connection,
                            Command* command);
    };
}

#endif
