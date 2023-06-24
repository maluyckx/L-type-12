#include "ConnectionInfoSFML.hpp"

bool Menu::ConnectionInfoSFML::testValues() noexcept {
    for (auto & _txt_field : _txt_fields) {
        std::string tmp_lenght=_txt_field->getTexte();
        if (tmp_lenght.length()<3) {
            _textes[0]->setTexte("All fields must be filled in with at least 3 characters.");
            return false;
        }
    }
    return true;
}

void Menu::ConnectionInfoSFML::switchField() noexcept {
    int where=-1;
    for (size_t i = 0; i < _txt_fields.size(); i++) {
        if (_txt_fields[i]->isSelect())
            where=static_cast<int>(i);
    }
    if (where!=-1) {
        _txt_fields[static_cast<size_t>(where)]->unselect();
        where=_txt_fields.size()==static_cast<unsigned int>(where+1) ? 0 : where+1;
        _txt_fields[static_cast<size_t>(where)]->select();
    }
}
