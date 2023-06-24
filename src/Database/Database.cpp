#include "Database.hpp"
#include <sqlite3.h>
#include <cstring>
#include <utility>
#include "../Utils/DebugLogger.hpp"

void Database::Database::load(const char* path) noexcept {
    /* Check doc sqlite3 assez simple et load*/
    int exit = sqlite3_open(path, &_db);
    if (exit) {
        DEBUGERR("Error opening the database " << sqlite3_errmsg(_db));
    } else {
        DEBUGMSG("Opened Database Successfully!");
    }
}

void Database::Database::save() noexcept {
    /* Commit keyword to save db
    */
    sqlite3_close(_db);
}

//template<typename Function>
void Database::Database::execute(const std::string& query) {
    /* Execute les commandes SQL qui demande pas de retour
     */
    int rc;
    char* messaggeError;
    rc = sqlite3_exec(_db, query.c_str(), nullptr, nullptr, &messaggeError);;

    if (rc != SQLITE_OK) {
        DEBUGERR("Error : " << messaggeError);
        sqlite3_free(messaggeError);
    } else {
        DEBUGMSG("Query executed successfully");
    }
}

Database::User Database::Database::getUser(int id) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(_db, "SELECT * FROM user WHERE _id = ?", -1, &stmt,
                       nullptr);
    sqlite3_bind_int(stmt, 1, id);
    int _id, _xp;
    const unsigned char* pseudo;
    const unsigned char* passwd;
    const unsigned char* question;
    const unsigned char* answer;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        _id      = sqlite3_column_int(stmt, 0);
        pseudo   = sqlite3_column_text(stmt, 1);
        passwd   = sqlite3_column_text(stmt, 2);
        question = sqlite3_column_text(stmt, 3);
        answer   = sqlite3_column_text(stmt, 4);
        _xp      = sqlite3_column_int(stmt, 5);
        std::string _pseudo   = reinterpret_cast<char*>(const_cast<unsigned char*>(pseudo));
        std::string _passwd   = reinterpret_cast<char*>(const_cast<unsigned char*>(passwd));
        std::string _question = reinterpret_cast<char*>(const_cast<unsigned char*>(question));
        std::string _answer   = reinterpret_cast<char*>(const_cast<unsigned char*>(answer));
        User        res(_id, _pseudo, _passwd, _question, _answer, _xp);
        sqlite3_finalize(stmt);
        return res;
    }
    sqlite3_finalize(stmt);
    User res(-1, "MissingNo.");
    return res;
}

Database::User Database::Database::getUser(const std::string& pseudo) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(_db, "SELECT * FROM user WHERE _pseudo = ?", -1, &stmt,
                       nullptr);
    sqlite3_bind_text(stmt, 1, pseudo.c_str(), static_cast<int>(strlen(pseudo.c_str())), nullptr);
    int _id, _xp;
    const unsigned char* passwd;
    const unsigned char* question;
    const unsigned char* answer;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        _id      = sqlite3_column_int(stmt, 0);
        passwd   = sqlite3_column_text(stmt, 2);
        question = sqlite3_column_text(stmt, 3);
        answer   = sqlite3_column_text(stmt, 4);
        _xp      = sqlite3_column_int(stmt, 5);
        std::string _passwd   = reinterpret_cast<char*>(const_cast<unsigned char*>(passwd));
        std::string _question = reinterpret_cast<char*>(const_cast<unsigned char*>(question));
        std::string _answer   = reinterpret_cast<char*>(const_cast<unsigned char*>(answer));
        User        res(_id, pseudo, _passwd, _question, _answer, _xp);
        sqlite3_finalize(stmt);
        return res;
    }
    sqlite3_finalize(stmt);
    User res(-1, "MissingNo.");
    return res;
}

/* Retourne l'id du dernier élément de la table user
 * Si erreur -> -1
 */
int Database::Database::getLastId() noexcept {
    sqlite3_stmt* stmt;
    int last_id = -1;
    sqlite3_prepare_v2(_db, "SELECT MAX(_id) FROM user", -1, &stmt, nullptr);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        last_id = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        return last_id;
    }
    sqlite3_finalize(stmt);
    return last_id;
}

bool Database::Database::signIn(const std::string& pseudo, const std::string& passwd,
                                const std::string& secret_question,
                                const std::string& secret_answer) {
    if (getUser(pseudo).getId() == -1) { // Si l'user n'existe pas
        std::string query = (
            "INSERT INTO user(_pseudo, _password, _secret_question, _secret_answer, _xp) VALUES('" +
            pseudo + "','" + passwd +
            "','" + secret_question + "','" + secret_answer + "', 0);");
        execute(query);
        return true;
    }
    return false;
}

void
Database::Database::changePasswd(const std::string& pseudo, const std::string& new_passwd) {
    std::string query = "UPDATE user set _password ='" + new_passwd +
                        "'where _pseudo='" + pseudo + "';";
    execute(query);
}

std::vector<std::string> Database::Database::getFriendList(
    std::string pseudo) { 
    std::vector<std::string> res;
    int                      id = getId(std::move(pseudo));
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(_db,
                       "SELECT _pseudo FROM user as U LEFT JOIN Friends as F ON U._id = F._id_friend WHERE F._id_user = ?",
                       -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
        res.emplace_back(reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(
                stmt, 0))));
    }
    sqlite3_finalize(stmt);
    return res;
}

void Database::Database::updateXp(const std::string& pseudo, int xp) {
    int ancienxp = getXp(pseudo);
    xp += ancienxp;
    std::string query = "UPDATE user SET _xp = " + std::to_string(xp) +
                        " WHERE _pseudo = '" + pseudo + "'";
    execute(query);
}

void Database::Database::addFriendRequest(const std::string& from, const std::string& to) {
    auto             friends        = getFriendList(from);
    auto             pending_from   = getPendingRequest(from);
    auto             pending_to     = getPendingRequest(to);
    bool             already_friend = false;
    for (auto & i : friends) {
        if (i == to || i == from) already_friend = true;
    }
    for (auto & i : pending_from) {
        if (i == from || i == to)
            already_friend = true;
    }
    for (auto & i : pending_to) {
        if (i == from || i == to) already_friend = true;
    }

    if (!already_friend) {
        int         state = State::UNDEFINED;
        std::string query =
                        "INSERT INTO FriendRequest(_id_user, _id_friend, _state) VALUES(" +
                        std::to_string(getId(from)) + ", " +
                        std::to_string(getId(to)) + ", " +
                        std::to_string(state) + ")";
        DEBUGMSG(query);
        execute(query);
    }
}

void Database::Database::acceptFriendRequest(const std::string& from, const std::string& to) {
    if (getStateFriendRequest(from, to) ==
        State::UNDEFINED) { // Il faut quelle soit Undefined pour pouvoir être accepté
        std::string query = "DELETE FROM FriendRequest WHERE _id_user = " +
                            std::to_string(getId(to)) + " AND _id_friend = " +
                            std::to_string(getId(from));
        execute(query);
        query = "INSERT INTO Friends(_id_user, _id_friend) VALUES(" +
                std::to_string(getId(from)) + ", " + std::to_string(getId(to)) +
                ")";
        execute(query);
        query = "INSERT INTO Friends(_id_user, _id_friend) VALUES(" +
                std::to_string(getId(to)) + ", " + std::to_string(getId(from)) +
                ")";
        execute(query);
    }
}

void Database::Database::refuseFriendRequest(const std::string& from, const std::string& to) {
    if (getStateFriendRequest(from, to) == State::UNDEFINED) {
        std::string query = "DELETE FROM FriendRequest WHERE _id_user = " +
                            std::to_string(getId(to)) + " AND _id_friend = " +
                            std::to_string(getId(from));
        execute(query);
    }
}

void Database::Database::deleteFriend(const std::string& me, const std::string& enemy) {
    std::string query  = "DELETE FROM Friends where _id_user = " +
                         std::to_string(getId(me)) + " AND _id_friend = " +
                         std::to_string(getId(enemy));
    std::string query1 = "DELETE FROM Friends where _id_user = " +
                         std::to_string(getId(enemy)) + " AND _id_friend = " +
                         std::to_string(getId(me));
    execute(query);
    execute(query1);
}

Database::State
Database::Database::getStateFriendRequest(std::string from, std::string to) {
    int res{};
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(_db,
                       "SELECT _state FROM FriendRequest  WHERE _id_user = ? AND _id_friend = ?",
                       -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, getId(std::move(from)));
    sqlite3_bind_int(stmt, 2, getId(std::move(to)));
    while (sqlite3_step(stmt) != SQLITE_DONE) {
        res = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    switch (res) {
        case State::UNDEFINED:
            return State::UNDEFINED;
        case State::ACCEPT:
            return State::ACCEPT;
        case State::REFUSE:
            return State::REFUSE;
        default:
            return State::UNDEFINED;
    }
}

std::vector<Database::leaderboard> Database::Database::getLeaderboard() {
    std::vector<leaderboard> res;
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(_db, "SELECT _pseudo, _xp FROM user ORDER BY _xp DESC",
                       -1, &stmt, nullptr);

    while (sqlite3_step(stmt) != SQLITE_DONE) {
        leaderboard r;
        r._pseudo = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(
            stmt, 0)));
        r._xp     = sqlite3_column_int(stmt, 1);
        res.push_back(r);
    }
    sqlite3_finalize(stmt);
    return res;
}

std::vector<std::string>
Database::Database::getPendingRequest(std::string from) {
    std::vector<std::string> res;
    int                      id = getId(std::move(from));
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(_db,
                       "SELECT _pseudo FROM user as U LEFT JOIN FriendRequest as F ON U._id = F._id_user WHERE F._id_friend = ? AND F._state = ?",
                       -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_int(stmt, 2, State::UNDEFINED);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
        res.emplace_back(reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(
                stmt, 0))));
    }
    sqlite3_finalize(stmt);
    return res;
}


bool Database::Database::newLevel(std::string name, std::string pseudo){
    if (!isLevelNameAlreadyTaken(name)){ // vérifie si le nom du lvl existe deja
        std::string query = "INSERT INTO Levels(_level_name, _id_user, _likes) VALUES('" + name + "'," + std::to_string(getId(pseudo)) + ",0)";
        execute(query);
        return true;
    }
    return false;    
}

void Database::Database::updateLikes(std::string level){
    
    std::string query = "UPDATE lEVELS SET _likes = " + std::to_string((getLikes(level))) + " WHERE _level_name = '" + level + "'";
    execute(query);
}

void Database::Database::removeLikes(std::string level){
    if (getLikes((level)) > 0 ) {
        std::string query = "UPDATE lEVELS SET _likes = " + std::to_string((getLikes(level))) + " WHERE _level_name = '" + level + "'";
        execute(query);
    }
}

std::vector<Database::levels> Database::Database::showLevels(std::string pseudo){
    std::vector<levels> res;
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(_db,
                       "SELECT _level_name, _likes FROM Levels WHERE _id_user = ?;",
                       -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, getId(pseudo));
    while (sqlite3_step(stmt) != SQLITE_DONE) {
        levels r;
        r._creator = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 0)));
        r._likes   = sqlite3_column_int(stmt, 1);
        res.push_back(r);
    }
    return res;
}

int Database::Database::getLikes(std::string level){
    int res{};
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(_db,
                       "SELECT count(Li._id_level) FROM Likes as Li LEFT JOIN Levels as Le ON Le._id = Li._id_level WHERE Li._id_level = ?;",
                       -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, getIdLevel(level));
    while (sqlite3_step(stmt) != SQLITE_DONE) {
        res = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return res;
}

bool Database::Database::isLevelNameAlreadyTaken(std::string level){ // vérifie que le nom du niveau n'est pas déjà utilisé
    std::string res{};
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(_db,
                       "SELECT _level_name FROM Levels where _level_name = ?;",
                       -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, level.c_str(), static_cast<int>(strlen(level.c_str())), nullptr);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
        res = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 0)));
    }
    sqlite3_finalize(stmt);
    return res == level;
}

// Retourne -1 si n'existe pas
int Database::Database::getIdLevel(std::string name_level){
    int res = -1;
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(_db, 
                    "SELECT _id FROM Levels WHERE _level_name = ?",
                    -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name_level.c_str(), static_cast<int>(strlen(name_level.c_str())), nullptr);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
        res = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return res;
}

bool Database::Database::likedBy(std::string name_level, std::string name_user){
    int res{};
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(_db, 
                       "SELECT _id_level FROM Likes WHERE _id_level = ? AND _id_user = ?",
                       -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, getIdLevel(name_level));
    sqlite3_bind_int(stmt, 2, getId(name_user));
    while (sqlite3_step(stmt) != SQLITE_DONE) {
        res = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return res;
}

void Database::Database::newLike(std::string name_level, std::string name_user){
    if (!likedBy(name_level, name_user)){
        std::string query = "INSERT INTO Likes VALUES (" + std::to_string(getIdLevel(name_level)) + ", " + std::to_string(getId(name_user)) + ")";
        execute(query);
        updateLikes(name_level);
    }else{
        deleteLike(name_level, name_user);
    }
}

void Database::Database::deleteLike(std::string name_level, std::string name_user){
    if (likedBy(name_level, name_user)){
        std::string query = "DELETE FROM Likes WHERE _id_user = " + std::to_string(getId(name_user)) + " AND _id_level = " + std::to_string(getIdLevel(name_level));
        execute(query);
        removeLikes(name_level);
    }
}

Database::Database::~Database() noexcept {
    pthread_mutex_destroy(_mutex);
}