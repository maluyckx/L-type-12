#include "LeaderboardMenuSFML.hpp"

const size_t NB_LEADERBOARD=20;
const float THICKNESS = 5;

Menu::LeaderboardMenuSFML::LeaderboardMenuSFML() noexcept{
    initAll();
}

void Menu::LeaderboardMenuSFML::initAll() noexcept { 
	_buttons.emplace_back(new SFML::Button(WIDTH_WINDOW_SFML - 280.f,  HEIGHT_WINDOW_SFML-100.f, 260.f, 60.f, "sprites/button/btn_red_", "BACK", 40, COLORS_SFML::WHITE));
    _buttons.emplace_back(new SFML::Button(WIDTH_WINDOW_SFML - 280.f,  HEIGHT_WINDOW_SFML-185.f, 260.f, 60.f, "sprites/button/btn_blue_", "REFRESH", 30, COLORS_SFML::WHITE));
                                                                                                    
	_sprites.emplace_back(new SFML::SpriteSimple(30.f, 40.f, WIDTH_WINDOW_SFML - 60.f, HEIGHT_WINDOW_SFML - 230.f, "sprites/background/rectangle/fond_score"));
    _sprites.emplace_back(new SFML::SpriteSimple(30.f, HEIGHT_WINDOW_SFML - 160.f, WIDTH_WINDOW_SFML - 320.f, 120.f, "sprites/background/rectangle/fond_my_score"));

	//STATIC TEXT
    _textes.emplace_back(new SFML::Texte("LEADERBOARD", WIDTH_WINDOW_SFML/2 - 88.f, 55.f, 19, COLORS_SFML::YELLOW));
	_textes.emplace_back(new SFML::Texte("MY SCORE: ", 90.f, HEIGHT_WINDOW_SFML-118.f, 18, COLORS_SFML::WHITE));
	_textes.emplace_back(new SFML::Texte("RANK", 110.f, 107.f, 15, COLORS_SFML::WHITE));
	_textes.emplace_back(new SFML::Texte("PSEUDO", WIDTH_WINDOW_SFML/2 - 35, 107.f, 15, COLORS_SFML::WHITE));
	_textes.emplace_back(new SFML::Texte("SCORE", WIDTH_WINDOW_SFML - 190.f, 107.f, 15, COLORS_SFML::WHITE));
	//DYNAMIC TEXT
	_textes.emplace_back(new SFML::Texte("", WIDTH_WINDOW_SFML/2 - 150, HEIGHT_WINDOW_SFML-118.f, 15, COLORS_SFML::WHITE));
	_textes.emplace_back(new SFML::Texte("", WIDTH_WINDOW_SFML/2 + 185.f, HEIGHT_WINDOW_SFML-118.f, 15, COLORS_SFML::WHITE));

	for(int i=0; i<20;i++){
		_textes.emplace_back(new SFML::Texte("", 130.f, 135.f+(static_cast<float>(i)*25), 15, COLORS_SFML::WHITE));
		_textes.emplace_back(new SFML::Texte("", WIDTH_WINDOW_SFML/2 - 27, 135.f+(static_cast<float>(i)*25), 15, COLORS_SFML::WHITE));
		_textes.emplace_back(new SFML::Texte("", WIDTH_WINDOW_SFML - 190.f, 135.f+(static_cast<float>(i)*25), 15, COLORS_SFML::WHITE));
	}
	
	requestLeaderboard();

}


MNAME Menu::LeaderboardMenuSFML::windowInputs(sf::Event &event) noexcept {
	if(event.type == sf::Event::MouseMoved){
        for(auto & _button : _buttons)
            _button->isOn(event.mouseMove.x,event.mouseMove.y);
    }
    if(event.type == sf::Event::MouseButtonPressed){
        if (_buttons[0]->isClickOn())
            return MNAME::MAIN_MENU;
        else if(_buttons[1]->isClickOn())
        	requestLeaderboard();
    }
    return MNAME::NOTHING;
}

void Menu::LeaderboardMenuSFML::animation() noexcept{
   Menu::BaseMenuSFML::animation();
}

void Menu::LeaderboardMenuSFML::setLeaderboard(const char* usernames_list, int* scores_list) noexcept {
    size_t starting = (_textes.size())-(NB_LEADERBOARD*3);
    int position = 1;
    for (size_t i = 0; i < 20; i++) {
        if (usernames_list[i*32]!='\0') {
            char tmp_str[32];
            int count_tmp=0;
            for (size_t j = i*32; j < 32*(i+1); j++) {
                tmp_str[count_tmp++] = usernames_list[j];
            } 
            _textes[starting+(i*3)]->setTexte(std::to_string(position++));
            _textes[starting+(i*3)+1]->setTexte(tmp_str);
			_textes[starting+(i*3)+2]->setTexte(std::to_string(scores_list[i]));       
		}
    }
}

void Menu::LeaderboardMenuSFML::setLeaderboardMy(int my_score) noexcept {
	size_t starting = (_textes.size())-(NB_LEADERBOARD*3);
	_textes[starting-2]->setTexte(_user_name);
    _textes[starting-1]->setTexte(std::to_string(my_score)); 
}