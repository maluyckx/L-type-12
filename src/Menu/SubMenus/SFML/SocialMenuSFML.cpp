#include "SocialMenuSFML.hpp"

const size_t BTN_SOCIAL = 3;

Menu::SocialMenuSFML::SocialMenuSFML() noexcept {
   initAll();
}

void Menu::SocialMenuSFML::initAll() noexcept {
    requestFriendList();
   _buttons.emplace_back(new SFML::Button(50.f,  HEIGHT_WINDOW_SFML-100.f, 300.f, 65.f, "sprites/button/btn_red_", "BACK", 50, COLORS_SFML::WHITE));
   _buttons.emplace_back(new SFML::Button(60.f, 500.f, 550.f, 120.f, "sprites/button/simple_grey", _user_name, 55, COLORS_SFML::WHITE));
   _buttons.emplace_back(new SFML::Button(600.f, 380.f, 60, 60, "sprites/button/button_send_", "SEARCH"));
   _txt_fields.emplace_back(new SFML::TexteField(65.f, 380.f, 520.f, 60.f, COLORS_SFML::WHITE, 25, COLORS_SFML::WHITE,19));
   _textes.emplace_back(new SFML::Texte("", 30.f, 460.f, 17, COLORS_SFML::WHITE));
   _sprites.emplace_back(new SFML::SpriteSimple(175.f, 40.f, 280.f, 280.f, "sprites/background/planets/earth_animate", 200, true));
   _sprites.emplace_back(new SFML::SpriteSimple(WIDTH_WINDOW_SFML-580.f, 10.f, 560.f, 880.f, "sprites/background/rectangle/fond_simple"));
   _textes.emplace_back(new SFML::Texte("Friends", 930.f, 50.f, 25, COLORS_SFML::WHITE));
   _view_scroll = sf::View(sf::FloatRect(0, 0, 500.f, 700.f));
   _view_scroll.setViewport(sf::FloatRect(0.606996f, 0.131f, 0.338006f, 0.772003f));
}

void Menu::SocialMenuSFML::display() {  // Need to override for this fucking scrolling
   timer(30); //0.585996 0.133 0.366006 0.774003
   _window->setView(_window->getDefaultView());
   _background->draw(*_window);
   _stars->draw(*_window);
   for (size_t i = 0; i < _bg_sprites.size(); i++)
      _bg_sprites[i]->draw(*_window);
   for (size_t i = 0; i < _sprites.size(); i++)
      _sprites[i]->draw(*_window);
   for (size_t i = 0; i < _textes.size(); i++)
      _textes[i]->draw(*_window);
   for (size_t i = 0; i < _txt_fields.size(); i++)
      _txt_fields[i]->draw(*_window);

   for (size_t i = 0; i < BTN_SOCIAL; i++)
      _buttons[i]->draw(*_window);
   _window->setView(_view_scroll);
   for (size_t i = BTN_SOCIAL; i < _buttons.size(); i++)
      _buttons[i]->draw(*_window);
   
}

MNAME Menu::SocialMenuSFML::windowInputs(sf::Event &event) noexcept {
   if(event.type == sf::Event::MouseMoved){
      for(size_t i = 0; i < BTN_SOCIAL; i++)
         _buttons[i]->isOn(event.mouseMove.x,event.mouseMove.y);
      _txt_fields[0]->isOn(event.mouseMove.x, event.mouseMove.y);

      _window->setView(_view_scroll);
      sf::Vector2i pixelPos = sf::Mouse::getPosition(*_window);
      sf::Vector2f worldPos = _window->mapPixelToCoords(pixelPos);
      for (size_t i = BTN_SOCIAL; i < _buttons.size(); i++)
         _buttons[i]->isOn(static_cast<float>(worldPos.x), static_cast<float>(worldPos.y));
      _window->setView(_window->getDefaultView());
      
   }
   if (event.type == sf::Event::MouseWheelMoved) {
      if (event.mouseWheel.delta==1) {
         if (_scrolling > 0)
            scrollBtn(10.f);
      }
      if (event.mouseWheel.delta==-1) {
         if (_buttons.size()>=BTN_SOCIAL+6&&
            static_cast<float>(_scrolling) < 50+105.f*static_cast<float>(_buttons.size()-(BTN_SOCIAL+7)))
            scrollBtn(-10.f);
      }
   }
   if(event.type == sf::Event::MouseButtonPressed){
      _txt_fields[0]->isCLickOn();
      if (_buttons[0]->isClickOn())
            return MNAME::MAIN_MENU;
      else if(_buttons[1]->isClickOn())
            return MNAME::MY_PROFILE_MENU;
      else if(_buttons[2]->isClickOn()) {
            searchFriend();
            return MNAME::NOTHING;
      }else
         return goTo();
   }
   if (event.type==sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::Enter) {
         searchFriend();
         return MNAME::NOTHING;
      }
      if (event.key.code == sf::Keyboard::Up) {
         if (_scrolling > 0)
            scrollBtn(5.f);
      }
      if (event.key.code==sf::Keyboard::Down) {
         if (_buttons.size()>=BTN_SOCIAL+6&&
            static_cast<float>(_scrolling) < 50+105.f*static_cast<float>(_buttons.size()-(BTN_SOCIAL+7)))
            scrollBtn(-5.f);
      }
   } else if (event.type==sf::Event::TextEntered) {
      if (event.text.unicode < 128) {
         _txt_fields[0]->update(static_cast<char>(event.text.unicode));
      }
   }
   return MNAME::NOTHING;
}

void Menu::SocialMenuSFML::animation() noexcept{
   Menu::BaseMenuSFML::animation();
   _sprites[0]->animate(0.2f);
}

void Menu::SocialMenuSFML::scrollBtn(float scroll) noexcept {
   _scrolling -= scroll;
   for (size_t i = BTN_SOCIAL; i < _buttons.size(); i++) {
      _buttons[i]->update(0, scroll);
   }
}

MNAME Menu::SocialMenuSFML::goTo() noexcept {
   for (size_t i = BTN_SOCIAL; i < _buttons.size(); i++){
      if (_buttons[i]->isClickOn()) {
         setFriendName(_friends_list[i-3]);
         return MNAME::PROFILE_FRIENDS;
      }
   }
   return MNAME::NOTHING;
}

void Menu::SocialMenuSFML::createButtons() noexcept {
   for (size_t i = 0; i < _friends_list.size(); i++){
      _buttons.emplace_back(new SFML::Button(
            0, static_cast<float>(i)*105.f, 500.f, 100.f,
            "sprites/button/button_friends_", _friends_list[i], 30, COLORS_SFML::WHITE));
   }
}

 void Menu::SocialMenuSFML::setFriendsList(const char* fl, unsigned count_friends) noexcept {
   Menu::SocialMenu::setFriendsList(fl, count_friends);
   createButtons();
 }

 void Menu::SocialMenuSFML::searchFriend() {
   if (strcmp(_user_name, _txt_fields[0]->getTexte())!=0) {
      auto* command = Game::CommandFactory::userData(_txt_fields[0]->getTexte());
      _client->getConnection()->getSendingCommandQueue()->push(command);
      setFriendName(_txt_fields[0]->getTexte());
   }else {
      _textes[0]->setTexte("Why are you looking for you ?");
   }
 }

 void Menu::SocialMenuSFML::setSearchText() {
   std::string tmp_str = _txt_fields[0]->getTexte();
   _textes[0]->setTexte(tmp_str+" does not exist !");
 }
