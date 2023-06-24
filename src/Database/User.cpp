#include "User.hpp"


Database::FriendRequest Database::User::sendFriendRequest(int id) {
    FriendRequest req(_id, id);
    _friend_request_list.push_back(req);
    return req;
}

Database::FriendRequest Database::User::sendFriendRequest(Database::User &user) {
    FriendRequest req(_id, user.getId());
    _friend_request_list.push_back(req);
    return req;
}

void Database::User::acceptFriendRequest(Database::FriendRequest &f) {
    f.setState(true);
    _friend_list.push_back(f.getIdSender());
}

void Database::User::refuseFriendRequest(Database::FriendRequest &f) {
    f.setState(false);
}

bool Database::User::removeFriend(Database::User &f) {
    for (std::size_t i = 0; i < _friend_list.size(); i++) {
        if (_friend_list.at(i) == f.getId()) {
            _friend_list.erase(_friend_list.begin() + static_cast<int>(i));
            return true;
        }
    }
    return false;
}



