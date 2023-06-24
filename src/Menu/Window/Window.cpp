#include "Window.hpp"

using namespace std;

// BASICS
void Menu::Window::ncursesInit() {
  initscr();            /* Start Ncurses */
  cbreak();             /* Keyboard input*/
  noecho();             /* Disable the display of entered characters */
  keypad(stdscr, TRUE); /* Activate specific keys */
  resizeterm(42, 126); // Ncurses info
  timeout(0);
  refresh();            /* Update the display  */
  curs_set(FALSE);
}

void Menu::Window::ncursesStop() {
    endwin();
}

void Menu::Window::ncursesColors() {
    /* Checking _color support */
    if (has_colors() == FALSE) {
        ncursesStop();
        exit(EXIT_FAILURE);
    }
    start_color(); /* Activate Colors */
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
}

KEY Menu::Window::input() {
    switch (getch()) {
        case KEY_UP:
            return KEY::UP;
            break;
        case KEY_DOWN:
            return KEY::DOWN;
            break;
        case KEY_LEFT:
            return KEY::LEFT;
            break;
        case KEY_RIGHT:
            return KEY::RIGHT;
            break;
        case 10:
            return KEY::ENTER;
            break;
        case 'z':
            return KEY::UP;
            break;
        case 's':
            return KEY::DOWN;
            break;
        case 'q':
            return KEY::LEFT;
            break;
        case 'd':
            return KEY::RIGHT;
            break;
        default:
            return KEY::OTHER;
            break;
    }
}

// Utilities
void Menu::Window::update() const {
    // refresh the main WINDOW
    refresh();
}

void Menu::Window::clearAll() {
    // clear all windows
    erase();
}
