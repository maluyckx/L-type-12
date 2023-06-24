#ifndef LTYPE_MENU_CONNECTIONINFO_H
#define LTYPE_MENU_CONNECTIONINFO_H

#include "../../MenuInfo.hpp"

namespace Menu {

    class ConnectionInfo: protected MenuInfo {
    protected:
        ConnectionInfo() noexcept = default;
    public:
        // If the connection menu is for the user or for a friend from roommenu
        static bool _is_user; // IDK HOW TO DO BETTER
        virtual bool testValues() noexcept = 0;
        virtual void setTextError() noexcept = 0;

        ~ConnectionInfo() noexcept override = default;
    };
}

#endif
