#ifndef LTYPE_DATABASE_FRIEND_REQUEST_H
#define LTYPE_DATABASE_FRIEND_REQUEST_H

#include <iostream>

    enum class State {
        ACCEPT = 0, REFUSE = 1, UNDEFINED = 2
    };

namespace Database {
    
    class FriendRequest {
        int _id_sender, _id_receiver;
        State _state = State::UNDEFINED;
    public:
        FriendRequest() = default;

        FriendRequest(int id_sender, int id_receiver) : _id_sender(id_sender), _id_receiver(id_receiver) {};

        void setState(bool s) {if (s) { _state = State::ACCEPT; } else { _state = State::REFUSE; }};
        bool stateIsUndefined() { return _state == State::UNDEFINED ;};

        int getIdSender() { return _id_sender; };

        friend std::ostream &operator<<(std::ostream &out, FriendRequest &F) {
            out << "Request from " << F._id_sender << " to " << F._id_receiver << ". Status: ";
            if (F._state == State::ACCEPT) out << "accepted";
            else if (F._state == State::REFUSE) out << "refused";
            else out << "undefined";
            return out;
          }
    };

}

#endif