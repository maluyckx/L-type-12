#ifndef LTYPE_DB_USER_H
#define LTYPE_DB_USER_H

#include <iostream>
#include <utility>
#include <vector>
#include "FriendRequest.hpp"


namespace Database {
    class User {
        int                        _id{}, _xp{};
        std::string                _pseudo, _password, _secret_question, _secret_answer;
        std::vector<int>           _friend_list{};
        std::vector<FriendRequest> _friend_request_list{};
    public:
        // Constructeur
        User() = default;
        User(int id, std::string pseudo) : _id(id), _xp(0), _pseudo(std::move(pseudo)), _password(), _secret_question(), _secret_answer() {};
        User(int id, std::string pseudo, std::string password, std::string secret_question, std::string secret_answer, int xp) :
            _id(id),_xp(xp), _pseudo(std::move(pseudo)), _password(std::move(password)), _secret_question(std::move(secret_question)), _secret_answer(std::move(secret_answer)) {};

        // Getter
        [[nodiscard]] inline constexpr int getId() const noexcept { return _id; }
        inline std::string getPseudo() noexcept { return _pseudo; }
        inline std::string getPassword() noexcept { return _password; }
        inline std::vector<int> getFriendList() noexcept { return _friend_list; };
        [[nodiscard]] inline constexpr int getXp() const noexcept { return _xp; }
        inline std::string getSecretQuestion() noexcept { return _secret_question; }
        inline std::string getSecretAnswer() noexcept { return _secret_answer; }
        // Friend interaction
        FriendRequest sendFriendRequest(int);
        FriendRequest sendFriendRequest(User&);
        void acceptFriendRequest(FriendRequest&);
        void refuseFriendRequest(FriendRequest&);
        bool removeFriend(User&);
        // Destructeur
        ~User() noexcept = default;  
    };

}


#endif
