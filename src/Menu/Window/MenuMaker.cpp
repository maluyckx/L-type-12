#include "MenuMaker.hpp"

using namespace std;

// Create
void Menu::MenuMaker::createWin(int x, int y, int w, int h) {
  _win.emplace_back(x, y, w, h);
}

void Menu::MenuMaker::createWin(size_t who, int x, int y, int w, int h) {
  _win.emplace_back(_win[who], x, y, w, h);
}

void Menu::MenuMaker::createButton(std::string n, int x, int y, int w, int h) {
  _btn.emplace_back(_win[MAIN_WINDOW], n, x, y, w, h);
}

void Menu::MenuMaker::createTxtInput(size_t max, int x, int y, int w, int h, bool is_password) {
    _txt_input.emplace_back(_win[MAIN_WINDOW], max, x, y, w, h, is_password);
}

void Menu::MenuMaker::createObj(size_t who, const std::string& txt, int pos_x, int pos_y, int color) {
  _obj.emplace_back(_win[who] ,pos_x, pos_y, txt, color);
}
// Input Text actions
KEY Menu::MenuMaker::writeText(size_t who){
  if  (who < _txt_input.size())
    return _txt_input[who].textWrite();
  else
    return Menu::TextInput::getInput();
}

char* Menu::MenuMaker::getText(size_t who) {
  return _txt_input[who].getTxt();
}
// Buttons actions
void Menu::MenuMaker::scrollButtons(size_t min, size_t max, bool direction) {
  if (direction){
    int tmp_x = _btn[min].getPosX();
    int tmp_y = _btn[min].getPosY();
    for (size_t i = min; i < max;++i) {
      _btn[i].setX(_btn[i+1].getPosX());
      _btn[i].setY(_btn[i+1].getPosY());
    }
    _btn[max].setX(tmp_x);
    _btn[max].setY(tmp_y);
  }
  else {
    int tmp_x = _btn[max].getPosX();
    int tmp_y = _btn[max].getPosY();
    for (size_t i = max; i > min;i--) {
        _btn[i].setX(_btn[i-1].getPosX());
        _btn[i].setY(_btn[i-1].getPosY());
    }
    _btn[min].setX(tmp_x);
    _btn[min].setY(tmp_y);
  }
  for (size_t i = min; i < _btn.size(); i++){
    _btn[i].move(_win[MAIN_WINDOW]);
  }
}

void Menu::MenuMaker::changeButtons(size_t who, char* new_txt) {
  if (new_txt[0]=='\0') {
    _btn[who].setX(SIZE::WIDTH);
    _btn[who].setY(SIZE::HEIGHT);
    _btn[who].move(_win[MAIN_WINDOW]);
    _btn.erase(_btn.begin() + static_cast<long>(who));
  }
  else
    _btn[who].changeTxt(new_txt);
}
// Highlight
void Menu::MenuMaker::setHiglight(size_t who, bool special) noexcept {
    if (who < _btn.size()){
      _btn[who].setHighlight();
      if (special)
        _btn[who].setSpecial();
    }
    else
      _txt_input[who-_btn.size()].setHighlight();
}

void Menu::MenuMaker::unsetHiglight(size_t who, bool special) noexcept {
    if (who < _btn.size()) {
      if (!special)
        _btn[who].unsetHighlight();
      _btn[who].unsetSpecial();
    }
    else 
      _txt_input[who-_btn.size()].unsetHighlight();
}
// Object
void Menu::MenuMaker::changeObject(size_t who, string s, int c) noexcept {
  if (s != "") {
    _obj[who].clearObject();
    _obj[who].setText(s);
  }
  else
    _obj[who].clearObject();
  _obj[who].setColor(c);
}

void Menu::MenuMaker::move(size_t who, int x, int y) noexcept {
  _obj[who].clearObject();
  _obj[who].setPosX(x);
  _obj[who].setPosY(y);
}

int Menu::MenuMaker::getPos(size_t who, bool xy) noexcept {
  if (xy)
    return _obj[who].getPosX();
  else
    return _obj[who].getPosY();
}
// Draw
void Menu::MenuMaker::drawButtons() {
  for (auto & i : _btn) {
    i.initButton();
  }
}

void Menu::MenuMaker::drawTxtInput() {
  for (auto & i : _txt_input) {
    i.initTxtInput();
  }
}

void Menu::MenuMaker::drawObjects() {
  for (auto & i : _obj) {
    i.drawObject();
  }
}

void Menu::MenuMaker::drawBox(size_t who, int color) {
  wattron(_win[who] ,COLOR_PAIR(color));
  box(_win[who], 0, 0);
  wattroff(_win[who] ,COLOR_PAIR(color));
}

// Utilities
void Menu::MenuMaker::updateWin(size_t who) {
  // refresh the main WINDOW
  wrefresh(_win[who]);
}