#include "Background.hpp"
#include "../../../Utils/Constants.hpp"
#include <ncurses.h>
#include <cstdlib> 
#include <ctime> 

const int SIDE=3;

Ncurses::Background::Background() noexcept {
    std::srand(static_cast<unsigned int>(std::time(nullptr))); 
    for (auto & _star : _stars) {
        _star[0] = SIDE+std::rand()%(WIDTH_WINDOW-SIDE*2);
        _star[1] = SIDE+std::rand()%(HEIGHT_WINDOW-SIDE*2);
    }  
}

void Ncurses::Background::display() noexcept {
    for (size_t i = 0; i < _stars.size(); i++) {
        if (i < STARS::BACK)
            mvaddch(_stars[i][1],  _stars[i][0], '.');
        else if (i < STARS::BACK+STARS::MIDDLE)
            mvaddch(_stars[i][1],  _stars[i][0], ',');
        else {
            attron(A_BOLD);
            mvaddch(_stars[i][1],  _stars[i][0], ACS_BULLET);
            attroff(A_BOLD);
        }
    }   
}

void Ncurses::Background::clearStars() noexcept {
    for (auto & _star : _stars)
        mvaddch(_star[0],  _star[1], ' ');
}

void Ncurses::Background::respawnStar(size_t who) noexcept {
    _stars[who][0] = std::rand()%(WIDTH_WINDOW-SIDE*2);
    _stars[who][1] = 0;
}
void Ncurses::Background::moveStar(size_t who) noexcept {
    if (_stars[who][1]==HEIGHT_WINDOW-5)
        respawnStar(who);
    else 
        _stars[who][1]++;
}

void Ncurses::Background::move() noexcept {
    if (_frame_timer.isTriggered()) {
        clearStars();
        for (size_t i = 0; i < _stars.size(); i++) {
            if (i < STARS::BACK && timer%4==0)
                moveStar(i);
            else if (i>=STARS::BACK && i<_stars.size()-FRONT && timer%2==0)
                moveStar(i);
            else if (i>=_stars.size()-FRONT)
                moveStar(i);
        }
        if (timer==12)
            timer=0;
        timer++;
    }
}