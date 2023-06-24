#include "GameClient.hpp"
#include <csignal>
#include <fcntl.h>
#include "../../Utils/DebugLogger.hpp"
#include <cstdlib>
#include "Ncurses/TerminalWindow.hpp"
#include "SFML/GraphicalWindow.hpp"

void sigpipe_handle(int) {
    std::cerr << "Connection Lost..." << std::endl;
    endwin();
    exit(EXIT_FAILURE);
}

Game::Client::GameClient::GameClient(bool isGfx) :
    _window(isGfx ?
            static_cast<Window*>(new GraphicalWindow(this)) :
            static_cast<Window*>(new TerminalWindow(this))
    ) {
    signal(SIGPIPE, sigpipe_handle);
    pid_t    client_pid          = getpid();
    unsigned session_id, tryouts = 5;
    do {
        session_id = _request_session_from_server(client_pid);
    } while (session_id == BAD_SESSION && --tryouts);
    _connection                  = new Connection(this, session_id, client_pid);
    _sound = new Sound();
}

Game::Client::GameClient::GameClient(
    Game::Client::GameClient&& other) noexcept:
    _connection(other._connection), _window(other._window) {
    other._connection = nullptr;
    other._window     = nullptr;
}

Game::Client::GameClient&
Game::Client::GameClient::operator=(Game::Client::GameClient&& rhs) noexcept {
    _connection = rhs._connection;
    _window     = rhs._window;
    rhs._connection = nullptr;
    rhs._window     = nullptr;
    return *this;
}

Game::Client::GameClient::~GameClient() {
    delete _connection;
    delete _sound;
    delete _state;
    delete _window;
}

void Game::Client::GameClient::run() {
    if (isSessionBad()) {
        std::cerr << "Server has no more sessions available!" << std::endl;
        endwin();
        exit(EXIT_FAILURE);
    }
    // start the connection with the server
    _connection->open();
    auto receiving_command_queue = _connection->getReceivingCommandQueue();
#ifdef DEBUG
    // for the purpose of debugging we send a ping to the server each seconds
    unsigned         ping_interval_sec = 1;
    Utils::StopWatch chrono{};
    chrono.start();
#endif
    for (;;) {
        // check if we DCed from the server
        if (_connection->getState() == ConnectionState::Closed) {
            // the connection was closed
            _window->clear();
            endwin();
            std::cerr << "Connection Lost..." << std::endl;
            exit(EXIT_FAILURE);
        }
        // handle inputs
        _window->input();
        if (!_window->isRunning()) {
            quit();
        }
#ifdef DEBUG
        chrono.stop();
        if (chrono.getElapsedSecs() > ping_interval_sec) {
            DEBUGMSG("Pinging...");
            _connection->getSendingCommandQueue()->push(
                CommandFactory::ping()
            );
            chrono.start();
        }
#endif
        // process the commands comming from the server
        while (!receiving_command_queue->empty()) {
            CommandExecutor::execute(_connection,
                                     receiving_command_queue->pop());
        }
        _window->display();
    }

}

void Game::Client::GameClient::quit() {
    endwin();
    _connection->close();
    delete _sound;  // explicitly destroy the intance of sound to free driver
    std::cout << "Freeing resources ... " << std::endl;
    sleep(1); // important, it gives time for the threads to end
    exit(EXIT_SUCCESS);
}

unsigned
Game::Client::GameClient::_request_session_from_server(pid_t client_pid) {
    unsigned session_id = 0;
    DEBUGMSG("Requesting session from server...");
    int client_to_matchmaker = open(_client2server_matchmaker_pipe_path,
                                    O_WRONLY | O_NONBLOCK);
    if (client_to_matchmaker < 0) {
        std::cerr << "No Server was found!" << std::endl;
        endwin();
        exit(EXIT_FAILURE);
    }
    DEBUGMSG("Opening " << _client2server_matchmaker_pipe_path
                        << " for writing");
    DEBUGMSG("Writting client pid " << client_pid << " and isGfx=" <<
                                    getWindow()->isDisplayGraphical());
    long bytes_written = write(client_to_matchmaker, &client_pid,
                               sizeof(pid_t));
    bool write_failed_pid = bytes_written < 0;
    auto display_mode  = getWindow()->getDisplayMode();
    bytes_written = write(client_to_matchmaker, &display_mode,
                           sizeof(DisplayMode));
    bool write_failed_gfx = bytes_written < 0;
    if (write_failed_pid || write_failed_gfx) {
        std::cerr << "Sending client data to server failed!" << std::endl;
        session_id = BAD_SESSION;
    } else {
        int matchmaker_to_client = open(_server2client_matchmaker_pipe_path,
                                        O_RDONLY);
        DEBUGMSG("Opening " << _server2client_matchmaker_pipe_path
                            << " for reading");
        long bytes_read = read(matchmaker_to_client, &session_id,
                               sizeof(unsigned));
        if (bytes_read < 0) {
            std::cerr << "Receiving session ID from server failed!"
                      << std::endl;
            session_id = BAD_SESSION;
        }
        DEBUGMSG("Closing matchmaker_to_client pipe...");
        close(matchmaker_to_client);
    }
    DEBUGMSG("Closing client_to_matchmaker pipe...");
    close(client_to_matchmaker);
    return session_id;
}

void Game::Client::GameClient::startGame(char* username_player1,
                                         char* username_player2,
                                         Game::GameSettings settings) {
    auto game_state = new GameState(username_player1, username_player2,
                                    settings);
    setGameState(game_state);
}

void Game::Client::GameClient::endGame() {
    delete getGameState();
    setGameState(nullptr);
    _window->getGameplayWindow()->clearAllLayers();
    _window->setState(WindowState::InMenu);
    _window->getWindowMenu()->activateMenu();
}
