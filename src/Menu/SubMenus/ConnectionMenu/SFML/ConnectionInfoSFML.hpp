#ifndef LTYPE_MENU_CONNECTIONINFOSFML_H
#define LTYPE_MENU_CONNECTIONINFOSFML_H

#include "../ConnectionInfo.hpp"
#include "../../../BaseMenuSFML.hpp"
#include <string>

namespace Menu {

    class ConnectionInfoSFML : public ConnectionInfo, public BaseMenuSFML {
    protected:
        ConnectionInfoSFML() noexcept = default;
    public:
        bool testValues() noexcept override;
        void switchField() noexcept;

        ~ConnectionInfoSFML() noexcept override =default;
    };
}

#endif
