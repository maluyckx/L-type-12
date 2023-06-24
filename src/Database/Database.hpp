/* GNU C++ -std=c++20
 * 5.2.21
 * Database class
 * Remarque:
 * - Seul class qui communique avec la Database, ici on part sur Sqlite3,
 *  PAS DROIT AU COPIE/TRANSFERT DE CLASSE DATABASE !!!!
 */

#ifndef LTYPE_DATABASE_DATABASE_H
#define LTYPE_DATABASE_DATABASE_H

#include <sqlite3.h>
#include <iostream>
#include "User.hpp"
#include <pthread.h>
namespace Database {
      
    enum State {
        UNDEFINED,
        ACCEPT, 
        REFUSE
    };

    struct leaderboard{
        std::string _pseudo{};
        int _xp{};
    };
    
    struct levels{
        std::string _creator{};
        int _likes{};
    };
    


    class Database {
      sqlite3* _db{};
      pthread_mutex_t* _mutex = new pthread_mutex_t();
    public:
      Database() { sqlite3_open("./ltypeDB", &_db); pthread_mutex_init(_mutex,nullptr); }

      Database& operator=(const Database&) = delete;
      Database(const Database&) = delete;
      Database& operator=(Database&&) = delete;
      Database(Database&&) = delete;

      void lock() noexcept {pthread_mutex_lock(_mutex); }
      void unlock() noexcept { pthread_mutex_unlock(_mutex);}
      void load(const char* path) noexcept; 
      void save() noexcept;

      int getLastId() noexcept;
      User getUser(int id);
      User getUser(const std::string& pseudo);

      int getId(std::string pseudo) {return getUser(pseudo).getId();}
      std::string getPasswd(std::string pseudo) {return getUser(pseudo).getPassword();}
      std::string getSecretQuestion(std::string pseudo) {return getUser(pseudo).getSecretQuestion();}
      std::string getSecretAnswer(std::string pseudo) {return getUser(pseudo).getSecretAnswer();}
      int getXp(std::string pseudo) {return getUser(pseudo).getXp();}

      void addFriendRequest(const std::string& from, const std::string& to);
      void acceptFriendRequest(const std::string& from, const std::string& to);
      void refuseFriendRequest(const std::string& from, const std::string& to);

      std::vector<std::string> getPendingRequest(std::string from);

      void deleteFriend(const std::string& me, const std::string& enemy);

      State getStateFriendRequest(std::string from, std::string to);

      bool testSecretQuestion(std::string pseudo, std::string answer)
        {return getSecretAnswer(pseudo) == answer;}
      
      std::vector<std::string> getFriendList(std::string pseudo);
      std::vector<leaderboard> getLeaderboard();

      void updateXp(const std::string& pseudo, int xp);
      
      bool logIn(std::string pseudo, std::string passwd) 
        {return getPasswd(pseudo) == passwd;};
      bool signIn(const std::string& pseudo, const std::string& passwd, const std::string& secret_question, const std::string& secret_answer);
      void changePasswd(const std::string& pseudo, const std::string& new_passwd);

      bool newLevel(std::string name, std::string pseudo);
      void updateLikes(std::string level);
      void removeLikes(std::string level);
      std::vector<levels> showLevels(std::string pseudo);
      int getLikes(std::string level);
      bool isLevelNameAlreadyTaken(std::string level);

      int getIdLevel(std::string name_level);
      bool likedBy(std::string name_level, std::string name_user);
      void newLike(std::string name_level, std::string name_user);
      void deleteLike(std::string name_level, std::string name_user);

      void execute(const std::string&);

      ~Database() noexcept;
    };
}
#endif