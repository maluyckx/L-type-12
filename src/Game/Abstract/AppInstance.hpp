#ifndef LTYPE_GAME_APPINSTANCE_H
#define LTYPE_GAME_APPINSTANCE_H

namespace Game {
    /**
     * Generalization of the GameClient and GameServer, to allow the conditional compilation to be more elegante
     */
    class AppInstance {
    protected:
        const char* _client2server_matchmaker_pipe_path = "/tmp/ltype_gr12_c2s_matchmaker_pipe";
        const char* _server2client_matchmaker_pipe_path = "/tmp/ltype_gr12_s2c_matchmaker_pipe";
        const unsigned BAD_SESSION = ~0u;
    public:
        virtual void run() = 0;
        virtual ~AppInstance() = default;
    };
}

#endif
