#ifndef LTYPE_GAME_GAMESERVER_H
#define LTYPE_GAME_GAMESERVER_H

#include "../Abstract/AppInstance.hpp"
#include "GameSession.hpp"
#include <unistd.h>
#include <string>
#include "../../Database/Database.hpp"

namespace Game::Server {
    /**
     * The host of all game session, it's a matchmaker between clients and a session.
     * The GameServer is created, it creates an unique reading pipe that the clients connects to.
     * Once the Server gets a client that sends its pid in the pipe, it spawns a new GameSession in a new _reading_thread with
     * the pid of the client in question, and they can connect (the session notify with a signal the client)
     */
    class GameServer final : public AppInstance {
    public:
        static constexpr char process_name[]   = "ltype_game_server";
        static constexpr char lock_file_path[] = "/tmp/ltype_game_server.lock";
    private:
        constexpr static unsigned MAX_SESSIONS          = 4;
        constexpr static int      NO_USER_ID            = -1;
        constexpr static unsigned USER_1                = 0;
        constexpr static unsigned USER_2                = 1;
        int                       _client_to_matchmaker = 0;
        int                       _matchmaker_to_client = 0;
        GameSession       * _current_sessions[MAX_SESSIONS]{};
        Database::Database* _database;
        /// The list of currently logged in users per session
        int _id_logged_in[MAX_SESSIONS][2];
        /// The main listening routine: waits for client to connect
        [[noreturn]] void listen();
        /// Gets a slot for a new GameSession
        unsigned getFirstFreeSessionId();
        /// Cleanup all Terminated Sessions
        void cleanTerminatedSessions();
    public:
        GameServer();
        GameServer(const GameServer& other) = delete;
        [[maybe_unused]] GameServer(GameServer&& other) noexcept;
        GameServer& operator=(const GameServer& rhs) = delete;
        GameServer& operator=(GameServer&& rhs) noexcept;
        ~GameServer() noexcept override;

        Database::Database* getDatabase() const noexcept {
            return _database;
        }

        /// Starting point of execution for the GameServer
        void run() override;
        /// Terminate a session by id
        void endSession(unsigned id);
        /**
         * Creates the lock file
         * @return return true if lock file was created, false if the lock was
         * already present
         */
        static bool createLockFile();
        static void destroyLockFile();
        bool isUserIDLoggedIn(int user_id);
        void addUserIDLoggedIn(unsigned session_id, int user_id);
        void removeUserIDLoggedIn(unsigned session_id, int user_id);
        void removeAllUserIDLoggedInSession(unsigned session_id);
    };
}


#endif
