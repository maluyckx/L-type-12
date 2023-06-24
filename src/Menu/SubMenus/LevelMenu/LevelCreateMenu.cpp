#include "LevelCreateMenu.hpp"
#include "LevelChoiceSettingsMenu.hpp"
#include "../../../Utils/Constants.hpp"
#include "../../../Game/Abstract/EntityType.hpp"
#include <fstream>
#include <algorithm> // For sort function

Menu::LevelCreateMenu::LevelCreateMenu() noexcept : 
    _factor_time(Menu::LevelChoiceSettingsMenu::getTimeLevel()),
    _level_name(Menu::LevelChoiceSettingsMenu::getNameLevel()) {
    Menu::LevelCreateMenu::initAll();
    sf::Texture *swarmling_texture = new sf::Texture;
    sf::Texture *carrier_texture = new sf::Texture;
    sf::Texture *boss_poulpe_texture = new sf::Texture;
    sf::Texture *boss_dragon_texture = new sf::Texture;
    sf::Texture *cruiser_texture = new sf::Texture;
    sf::Texture *escorter_texture = new sf::Texture;
    sf::Texture *lasergun_texture = new sf::Texture;
    sf::Texture *obstacle_texture = new sf::Texture;
    sf::Texture *boss_entreprise_texture = new sf::Texture;
    sf::Texture *boss_asteroid_base_texture = new sf::Texture;
    sf::Texture *background_texture = new sf::Texture;
    swarmling_texture->loadFromFile(PATH_SWARMLING+".png"); 
    carrier_texture->loadFromFile(PATH_BATTLESHIP+".png");  
    boss_poulpe_texture->loadFromFile(PATH_POULPE+".png");
    boss_dragon_texture->loadFromFile(PATH_DRAGON+"0.png");
    cruiser_texture->loadFromFile(PATH_CRUISER+".png");  
    escorter_texture->loadFromFile(PATH_ESCORTER+".png");  
    lasergun_texture->loadFromFile(PATH_LASERGUN+".png");
    obstacle_texture->loadFromFile("sprites/obstacle/asteroid.png");
    boss_entreprise_texture->loadFromFile(PATH_ENTERPRISE+".png");
    boss_asteroid_base_texture->loadFromFile(PATH_ASTEROID_BASE_CLOSE+".png");
    background_texture->loadFromFile("sprites/background/background_1.png");
   _texture_mob[ChoicedMob::SWARMLING]        = swarmling_texture;
   _texture_mob[ChoicedMob::BATTLESHIP]       = carrier_texture;
   _texture_mob[ChoicedMob::CRUISER]          = cruiser_texture;
   _texture_mob[ChoicedMob::ESCORTER]         = escorter_texture;
   _texture_mob[ChoicedMob::LASERGUN]         = lasergun_texture;
   _texture_mob[ChoicedMob::OBSTACLE]         = obstacle_texture;
   _texture_mob[ChoicedMob::BOSS_POULPE]      = boss_poulpe_texture;
   _texture_mob[ChoicedMob::BOSS_DRAGON]      = boss_dragon_texture;
   _texture_mob[ChoicedMob::BOSS_ENTERPRISE]  = boss_entreprise_texture;
   _texture_mob[ChoicedMob::BOSS_ASTEROID]    = boss_asteroid_base_texture;
   _texture_mob[ChoicedMob::BACKGROUND]       = background_texture;
   _background_view.setTexture(*_texture_mob[ChoicedMob::BACKGROUND]);
}

void Menu::LevelCreateMenu::initAll() noexcept {
    _window->setMouseCursorVisible(false);
    _window->setFramerateLimit(350); // On en a besoin pour l'animation kirby
    _view_scroll = sf::View(sf::Vector2f(WIDTH_WINDOW_SFML / 2, HEIGHT_WINDOW_SFML / 2 * static_cast<float>(_factor_time)),
                            sf::Vector2f(WIDTH_WINDOW_SFML, HEIGHT_WINDOW_SFML * static_cast<float>(_factor_time)));
    _view_scroll.setViewport(sf::FloatRect(0, 0, viewport, 1 * static_cast<float>(_factor_time))); // Pour le scrolling
    
    // Enemy ship
    _buttons.emplace_back(new SFML::Button(12 + (90 * 0), 30, 80, 80, "sprites/level_editor/button_swarmling", NAME_SWARMLING));
    _buttons.emplace_back(new SFML::Button(12 + (90 * 1), 30, 80, 80, "sprites/level_editor/button_battleship", NAME_BATTLESHIP));
    _buttons.emplace_back(new SFML::Button(12 + (90 * 2), 30, 80, 80, "sprites/level_editor/button_cruiser", NAME_CRUISER));
    _buttons.emplace_back(new SFML::Button(12 + (90 * 3), 30, 80, 80, "sprites/level_editor/button_escorter", NAME_ESCORTER));
    _buttons.emplace_back(new SFML::Button(12 + (90 * 4), 30, 80, 80, "sprites/level_editor/button_lasergun", NAME_LASERGUN));

    // Obstacles
    _buttons.emplace_back(new SFML::Button(12 + (90 * 5), 30, 80, 80, "sprites/level_editor/button_obstacle", NAME_OBSTACLE));

    // Bosses
    _buttons.emplace_back(new SFML::Button(12 + (90 * 6), 30, 80, 80, "sprites/level_editor/button_boss_poulpe", NAME_BOSS_POULPE));
    _buttons.emplace_back(new SFML::Button(12 + (90 * 7), 30, 80, 80, "sprites/level_editor/button_dracofeu", NAME_BOSS_DRAGON));
    _buttons.emplace_back(new SFML::Button(12 + (90 * 8), 30, 80, 80, "sprites/level_editor/button_entreprise", NAME_BOSS_ENTERPRISE));
    _buttons.emplace_back(new SFML::Button(12 + (90 * 9), 30, 80, 80, "sprites/level_editor/button_asteroid_base", NAME_BOSS_ASTEROID));

    // Buttons
    _buttons.emplace_back(new SFML::Button(12 + (90 * 10), 30, 80, 80, "sprites/level_editor/button_undo", "Undo"));
    _buttons.emplace_back(new SFML::Button(12 + (90 * 11), 30, 80, 80, "sprites/level_editor/button_save", "Save"));
    _buttons.emplace_back(new SFML::Button(12 + (90 * 12), 30, 80, 80, "sprites/level_editor/button_eraser", "Eraser"));
    _buttons.emplace_back(new SFML::Button(12 + (90 * 13) + 30, 30, 80, 80, "sprites/level_editor/button_quit", "Quit"));
    
    // Texte d'erreur
    _textes.emplace_back(new SFML::Texte("", 400, 150, 15.f, COLORS_SFML::WHITE));  // Erreur
    _textes.emplace_back(new SFML::Texte("BEGIN", 50, 150, 15.f, COLORS_SFML::WHITE));
    _textes.emplace_back(new SFML::Texte("", 50, 800, 15.f, COLORS_SFML::WHITE));
    _textes.emplace_back(new SFML::Texte("The level is seen from the perspective of the enemy ships", 225, 125, 15.f, COLORS_SFML::WHITE));  // Indication level
    
    // Sprites (kirby)
    _sprites.emplace_back(new SFML::SpriteSimple(0.f, 0.f, 77.f, 49.f, "sprites/animations/Kirby_cut/anim_cut_kirby_", 4));

    // Sprites vaisseaux joueur
    _sprites.emplace_back(new SFML::SpriteSimple(12 + (90 * 3), 200, 77.f, 49.f, "sprites/level_editor/spaceship"));
    _sprites.emplace_back(new SFML::SpriteSimple(12 + (90 * 12), 200, 77.f, 49.f, "sprites/level_editor/spaceship"));
    _sprites[1]->rotate(180);
    _sprites[2]->rotate(180);
}

Game::EntityType Menu::LevelCreateMenu::getTypeFromName(std::string name){
    if(name==NAME_SWARMLING) {
        return Game::EntityType::ShipSwarmling;
    } else if (name==NAME_OBSTACLE) {
    	return Game::EntityType::Asteroid;
    } else if(name==NAME_BOSS_POULPE){
        return Game::EntityType::BossPoulpe;
    } else if(name==NAME_BATTLESHIP){
        return Game::EntityType::ShipBattleship;
    } else if(name==NAME_CRUISER){
    	return Game::EntityType::ShipCruiser;
    } else if(name==NAME_LASERGUN){
        return Game::EntityType::ShipLaserGun;
    } else if(name==NAME_BOSS_ASTEROID){
        return Game::EntityType::BossAsteroidBase;
    } else if(name==NAME_ESCORTER){
        return Game::EntityType::ShipEscorter;
    } else if(name==NAME_BOSS_ENTERPRISE){
        return Game::EntityType::BossEnterprise;
    } else if(name==NAME_BOSS_DRAGON){
        return Game::EntityType::BossDragon;
    } else {
    	return Game::EntityType::ShipSwarmling;
    }
}

// Petite fonction qui retourne le type de mob en fonction du texte
// Pour ne pas que le code de windowInputs fasse 37 kilomètres
// Si le nom n'existe pas renvoie un Swarmling
Menu::ChoicedMob getMobFromText(std::string txt){
    if (txt == NAME_SWARMLING)             return Menu::ChoicedMob::SWARMLING;
    else if (txt == NAME_BATTLESHIP)       return Menu::ChoicedMob::BATTLESHIP;
    else if (txt == NAME_CRUISER)          return Menu::ChoicedMob::CRUISER;
    else if (txt == NAME_ESCORTER)         return Menu::ChoicedMob::ESCORTER;
    else if (txt == NAME_LASERGUN)         return Menu::ChoicedMob::LASERGUN;
    else if (txt == NAME_OBSTACLE)         return Menu::ChoicedMob::OBSTACLE;
    else if (txt == NAME_BOSS_POULPE)      return Menu::ChoicedMob::BOSS_POULPE;
    else if (txt == NAME_BOSS_DRAGON)       return Menu::ChoicedMob::BOSS_DRAGON;
    else if (txt == NAME_BOSS_ENTERPRISE)  return Menu::ChoicedMob::BOSS_ENTERPRISE;
    else if (txt == NAME_BOSS_ASTEROID)    return Menu::ChoicedMob::BOSS_ASTEROID;
    // else if (txt == "")         return Menu::ChoicedMob::;   // Pour en rajouter
    else return Menu::ChoicedMob::SWARMLING;
}

// TODO: Créer une struct plus compréhensible pour les dimensions que Vector2f
sf::Vector2f getDimensionFromEntity(Menu::ChoicedMob mob){
    sf::Vector2f res;
    switch (mob){
        case Menu::ChoicedMob::SWARMLING:
            res.x = Game::EntityDimensions::ShipSwarmling_WIDTH;
            res.y = Game::EntityDimensions::ShipSwarmling_HEIGHT;
            break;
        case Menu::ChoicedMob::BATTLESHIP:
            res.x = Game::EntityDimensions::ShipBattleship_WIDTH;
            res.y = Game::EntityDimensions::ShipBattleship_HEIGHT;
            break;
        case Menu::ChoicedMob::CRUISER:
            res.x = Game::EntityDimensions::ShipCruiser_WIDTH;
            res.y = Game::EntityDimensions::ShipCruiser_HEIGHT;
            break;
        case Menu::ChoicedMob::ESCORTER:
            res.x = Game::EntityDimensions::ShipEscorter_WIDTH;
            res.y = Game::EntityDimensions::ShipEscorter_HEIGHT;
            break;
        case Menu::ChoicedMob::LASERGUN:
            res.x = Game::EntityDimensions::ShipLaserGun_WIDTH;
            res.y = Game::EntityDimensions::ShipLaserGun_HEIGHT;
            break;
        case Menu::ChoicedMob::OBSTACLE:
            res.x = Game::EntityDimensions::Asteroid_WIDTH;
            res.y = Game::EntityDimensions::Asteroid_HEIGHT;
            break;
        case Menu::ChoicedMob::BOSS_POULPE:
            res.x = Game::EntityDimensions::BossPoulpe_WIDTH;
            res.y = Game::EntityDimensions::BossPoulpe_HEIGHT;
            break;
        case Menu::ChoicedMob::BOSS_DRAGON:
            res.x = Game::EntityDimensions::BossDragon_WIDTH;
            res.y = Game::EntityDimensions::BossDragon_HEIGHT;
            break;
        case Menu::ChoicedMob::BOSS_ENTERPRISE:
            res.x = Game::EntityDimensions::BossEnterprise_WIDTH;
            res.y = Game::EntityDimensions::BossEnterprise_HEIGHT;
            break;
        case Menu::ChoicedMob::BOSS_ASTEROID:
            res.x = Game::EntityDimensions::BossAsteroidBase_WIDTH;
            res.y = Game::EntityDimensions::BossAsteroidBase_HEIGHT;
            break;
        default:
            break;
    }
    res.x*=GRAPHIC_X_MULTIPLIER;
    res.y*=GRAPHIC_Y_MULTIPLIER;
    return res;
}

Menu::Mob_pos Menu::LevelCreateMenu::createSpriteFromChoice() noexcept {
    sf::Sprite* sprite = new sf::Sprite();
    // Permet de récup les bonnes coordonnées de la view Wééééééé
    sf::Vector2f worldPos = getPositionsInView();
    float posX = worldPos.x;
    float posY = static_cast<float>(static_cast<float>(sf::Mouse::getPosition(*_window).y) + _current_scroll_value);
    Mob_pos res;
    switch (_current_choice_mob){
        case ChoicedMob::SWARMLING:
            res.height  = Game::EntityDimensions::ShipSwarmling_HEIGHT;
            res.width   = Game::EntityDimensions::ShipSwarmling_WIDTH;
            sprite->setTexture(*_texture_mob[ChoicedMob::SWARMLING]);
            res.name    = NAME_SWARMLING;
            break;
        case ChoicedMob::BATTLESHIP:
            res.height  = Game::EntityDimensions::ShipBattleship_HEIGHT;
            res.width   = Game::EntityDimensions::ShipBattleship_WIDTH;
            sprite->setTexture(*_texture_mob[ChoicedMob::BATTLESHIP]);
            res.name    = NAME_BATTLESHIP;
            break;
        case ChoicedMob::CRUISER:
            res.height  = Game::EntityDimensions::ShipCruiser_HEIGHT;
            res.width   = Game::EntityDimensions::ShipCruiser_WIDTH;
            sprite->setTexture(*_texture_mob[ChoicedMob::CRUISER]);
            res.name    = NAME_CRUISER;
            break;
        case ChoicedMob::ESCORTER:
            res.height  = Game::EntityDimensions::ShipEscorter_HEIGHT;
            res.width   = Game::EntityDimensions::ShipEscorter_WIDTH;
            sprite->setTexture(*_texture_mob[ChoicedMob::ESCORTER]);
            res.name    = NAME_ESCORTER;
            break;
        case ChoicedMob::LASERGUN:
            res.height  = Game::EntityDimensions::ShipLaserGun_HEIGHT;
            res.width   = Game::EntityDimensions::ShipLaserGun_WIDTH;
            sprite->setTexture(*_texture_mob[ChoicedMob::LASERGUN]);
            res.name    = NAME_LASERGUN;
            break;
        case ChoicedMob::OBSTACLE:
            res.height  = Game::EntityDimensions::Asteroid_HEIGHT;
            res.width   = Game::EntityDimensions::Asteroid_WIDTH;
            sprite->setTexture(*_texture_mob[ChoicedMob::OBSTACLE]);
            res.name    = NAME_OBSTACLE;
            break; 
        case ChoicedMob::BOSS_POULPE:
            res.height  = Game::EntityDimensions::BossPoulpe_HEIGHT;
            res.width   = Game::EntityDimensions::BossPoulpe_WIDTH;
            sprite->setTexture(*_texture_mob[ChoicedMob::BOSS_POULPE]);
            _boss_posed = true;
            res.name    = NAME_BOSS_POULPE;
            break;
        case ChoicedMob::BOSS_DRAGON:
            res.height  = Game::EntityDimensions::BossDragon_HEIGHT;
            res.width   = Game::EntityDimensions::BossDragon_WIDTH;
            sprite->setTexture(*_texture_mob[ChoicedMob::BOSS_DRAGON]);
            _boss_posed = true;
            res.name    = NAME_BOSS_DRAGON;
            break;
        case ChoicedMob::BOSS_ENTERPRISE:
            res.height  = Game::EntityDimensions::BossEnterprise_HEIGHT;
            res.width   = Game::EntityDimensions::BossEnterprise_WIDTH;
            sprite->setTexture(*_texture_mob[ChoicedMob::BOSS_ENTERPRISE]);
            _boss_posed = true;
            res.name    = NAME_BOSS_ENTERPRISE;
            break;
        case ChoicedMob::BOSS_ASTEROID:
            res.height  = Game::EntityDimensions::BossAsteroidBase_HEIGHT;
            res.width   = Game::EntityDimensions::BossAsteroidBase_WIDTH;
            sprite->setTexture(*_texture_mob[ChoicedMob::BOSS_ASTEROID]);
            _boss_posed = true;
            res.name    = NAME_BOSS_ASTEROID;
            break;
        default:
            break;
    }
    res.width*=GRAPHIC_X_MULTIPLIER;
    res.height*=GRAPHIC_Y_MULTIPLIER;
    sf::Vector2f dimension = getDimensionFromEntity(_current_choice_mob);
   	sprite->setScale(static_cast<float>(dimension.x / static_cast<float>(sprite->getTexture()->getSize().x)),
                     static_cast<float>(dimension.y / static_cast<float>(sprite->getTexture()->getSize().y))
                     );
        
    sprite->setPosition(posX - res.width/2, posY - res.height/2);
    res.sprite = sprite;
    return res;
}

bool isThatABoss(std::string text){
    return  text == NAME_BOSS_ASTEROID   || 
            text == NAME_BOSS_POULPE     || 
            text == NAME_BOSS_ENTERPRISE || 
            text ==  NAME_BOSS_DRAGON;
}

sf::Vector2f Menu::LevelCreateMenu::getPositionsInView(){
    sf::View prev_view = _window->getView();
    _window->setView(_view_scroll);
    sf::Vector2i pixelPos = sf::Mouse::getPosition(*_window);
    sf::Vector2f worldPos = _window->mapPixelToCoords(pixelPos);
    _window->setView(prev_view);
    return worldPos;
}

void Menu::LevelCreateMenu::setTextBeginEnd() noexcept{
    if ( _current_scroll_value <= 12) _textes[1]->setTexte("BEGIN");
    else _textes[1]->setTexte("");

    if ((HEIGHT_WINDOW_SFML * static_cast<float>(_factor_time-1)) < _current_scroll_value) _textes[2]->setTexte("END");
    else _textes[2]->setTexte("");
}

MNAME Menu::LevelCreateMenu::windowInputs(sf::Event &event) noexcept {
    switch (event.type){
        case sf::Event::Closed:
            _window->close();
            break;
        case sf::Event::KeyPressed:{   
            if (event.key.code == sf::Keyboard::Up){
                if (_current_scroll_value - 10 > 0){
                    _view_scroll.move(0, -10.f); // Scroll de 10 vers le haut
                    _current_scroll_value -= 10;
                }
            }
            if (event.key.code == sf::Keyboard::Down){
                if (_current_scroll_value + HEIGHT_WINDOW_SFML - 10 < HEIGHT_WINDOW_SFML * static_cast<float>(_factor_time)){
                    _view_scroll.move(0, 10.f); // Scroll de 10 vers le bas
                    _current_scroll_value += 10;
                }
            }
            break;
            case sf::Event::MouseMoved: {
                for (auto & _button : _buttons)
                    _button->isOn(sf::Mouse::getPosition(*_window).x, sf::Mouse::getPosition(*_window).y);
                sf::Sprite tmp; // Permet de récupérer la couleur par défaut de la texture, j'ai pas cherché une façon plus simple
                tmp.setTexture(*_texture_mob[_current_choice_mob]);
                if (!_mode_erase){
                    if (collisionResolverEditor())
                        _mouse.setColor(sf::Color::Red);
                    else _mouse.setColor(tmp.getColor());
                }else{
                    _mouse.setColor(tmp.getColor());
                }
                break;
            }
            case sf::Event::MouseWheelMoved: {
                _current_scroll_value += 10 * event.mouseWheelScroll.delta;
                break;
            }
            case sf::Event::MouseButtonPressed : {
                if (sf::Mouse::getPosition(*_window).y < 130){ // Si click dans le HUD
                    for (auto button: _buttons){
                        if (button->isClickOn()){
                            if (button->getTxt() == "Undo"){    // BOUTON UNDO
                                if (!_mobs.empty()) deleteMob(_mobs.size() - 1);
                            }else if (button->getTxt() == "Eraser"){
                                _mode_erase = !(_mode_erase); // Toggle l'effaceur
                            }else if (button->getTxt() == "Quit"){  // BOUTON QUIT
                                _window->setFramerateLimit(FPS);   // TODO: Check si il n'y a pas une meilleur valeur ?
                                return MNAME::LEVEL_MAIN_MENU;
                            }else if(button->getTxt() == "Save"){   // BOUTON SAVE
                                if (testValues()){
                                    writeIntoFile();
                                    _window->setFramerateLimit(FPS);   // TODO: Check si il n'y a pas une meilleur valeur ?
                                    return MNAME::LEVEL_MAIN_MENU;
                                }else
                                    return MNAME::NOTHING;
                            }else{
                                _mode_erase = false;
                                if (!(isThatABoss(button->getTxt()) && _boss_posed)){
                                    _current_choice_mob = getMobFromText(button->getTxt());
                                }
                            }
                        }
                    }
                }else{ // Si click dans le créateur de niveau
                    if (_mode_erase){
                        eraser();
                    }else{
                        if (!(_boss_posed && isCurrentlyABoss())){
                            // Remet le curseur à l'origine de l'image
                            if (!collisionResolverEditor()){
                                _mobs.push_back(createSpriteFromChoice());
                                if (isCurrentlyABoss()) {
                                    _textes[0]->setTexte(""); // Remove le message d'erreur (pour le boss)
                                    _current_choice_mob = ChoicedMob::SWARMLING;
                                }
                            }
                        }
                    }
                }
                break;
            default:
                break;
            }
        }
    }
    return MNAME::NOTHING;
}


bool Menu::LevelCreateMenu::isCurrentlyABoss(){
    return isThatABoss(createSpriteFromChoice().name);
}

void Menu::LevelCreateMenu::deleteMob(std::size_t index) noexcept {
    Mob_pos mob_to_remove = _mobs.at(index);
    if (isThatABoss(mob_to_remove.name)) _boss_posed = false;
    _mobs.erase(_mobs.begin() + static_cast<long int>(index));
}

void Menu::LevelCreateMenu::display() {
    // Scroll auto quand curseur sur le bas de l'écran
    if (static_cast<float>(sf::Mouse::getPosition(*_window).y) > HEIGHT_WINDOW_SFML - 50 && _current_scroll_value + HEIGHT_WINDOW_SFML - 10 < HEIGHT_WINDOW_SFML * static_cast<float>(_factor_time)){
        _view_scroll.move(0, 1.37f);
        _current_scroll_value += 1.37f;
    }
    // Scroll auto quand curseur sur le haut de l'écran
    if (sf::Mouse::getPosition(*_window).y < 50 && _current_scroll_value + HEIGHT_WINDOW_SFML - 10 > HEIGHT_WINDOW_SFML){
        _view_scroll.move(0, -1.37f);
        _current_scroll_value -= 1.37f;
    }
    setTextBeginEnd();
    // Dessine les mobs posés
    _window->setView(_view_scroll);
    _window->draw(_background_view);    // Dessine le background, bien le mettre avant les mobs, sinon on voit plus rien
    for (const Mob_pos& mob : _mobs) _window->draw(*(mob.sprite));
    // Dessine les boutons de mobs
    // ATTENTION LES DESSINER APRES LA VIEW POUR QU'ilS APPARAISSENT AU DESSUS

    // Dessine le vaisseau du joueur pour indiquer le sens
    _sprites[1]->draw(*_window);
    _sprites[2]->draw(*_window);

    sf::Vector2f worldPos = getPositionsInView();
    if (_mode_erase){
        if (_is_kirby_cutting){
            _sprites[0]->animate(0.06f); // Constante   
        }
        if (_sprites[0]->isLastFrame()){
            _is_kirby_cutting = false;
            _sprites[0]->animate(1.f);
        }
        _sprites[0]->setPosition(worldPos.x - 14, // -14 pour l'espace "vide" dans le premier sprite de l'animation de kirby 
                                 static_cast<float>(static_cast<float>(sf::Mouse::getPosition(*_window).y) + _current_scroll_value - 5));
        _sprites[0]->draw(*_window);
    }else{
        _mouse.setTexture(*_texture_mob[_current_choice_mob], true);
        sf::Vector2f dimension = getDimensionFromEntity(_current_choice_mob);
    	_mouse.setScale(static_cast<float>(dimension.x / static_cast<float>(_mouse.getTexture()->getSize().x)), static_cast<float>(dimension.y /
                                                                                                                                   static_cast<float>(_mouse.getTexture()->getSize().y)));
        _mouse.setPosition(static_cast<float>(worldPos.x - getDimensionFromEntity(_current_choice_mob).x/2), 
                          static_cast<float>(static_cast<float>(sf::Mouse::getPosition(*_window).y) + _current_scroll_value - getDimensionFromEntity(_current_choice_mob).y / 2));
        _window->draw(_mouse);
    }

    _window->setView(_window->getDefaultView());
    for (auto & _button : _buttons)
      _button->draw(*_window);
    for (auto & _texte : _textes)
      _texte->draw(*_window);
}

void Menu::LevelCreateMenu::animation() noexcept {}

bool Menu::LevelCreateMenu::collisionResolverEditor() {
    // Dimension of current mob
    float width = getDimensionFromEntity(_current_choice_mob).x;
    float height = getDimensionFromEntity(_current_choice_mob).y;
    // Position of current mob
    sf::Vector2f worldPos = getPositionsInView();
    float posX = worldPos.x - width/2;
    float posY = static_cast<float>(static_cast<float>(sf::Mouse::getPosition(*_window).y) + _current_scroll_value - height / 2);

    if (posX + width > WIDTH_WINDOW_SFML || posY + height > HEIGHT_WINDOW_SFML * static_cast<float>(_factor_time) || posX < 0) return true; // L'entité dépasse dans le mur

	for(auto mob : _mobs){
    	if (posX <= mob.sprite->getPosition().x + mob.width && posX + width >= mob.sprite->getPosition().x &&
            posY <= mob.sprite->getPosition().y + mob.height && posY + height >= mob.sprite->getPosition().y)
            return true;
	}
	return false;
}

void Menu::LevelCreateMenu::eraser() noexcept {
    sf::Vector2f worldPos = getPositionsInView();
    float posX = worldPos.x;
    float posY = static_cast<float>(sf::Mouse::getPosition(*_window).y) + _current_scroll_value;
	for(std::size_t i = 0; i < _mobs.size(); i++){
    	if (posX <= _mobs.at(i).sprite->getPosition().x + _mobs.at(i).width && posX >= _mobs.at(i).sprite->getPosition().x && posY <= _mobs.at(i).sprite->getPosition().y + _mobs.at(i).height && posY >= _mobs.at(i).sprite->getPosition().y){
            _is_kirby_cutting = true;
            deleteMob(i);
            break;
		}
	}
}

void Menu::LevelCreateMenu::writeIntoFile() noexcept {
    std::string level;
    std::sort(_mobs.begin(), _mobs.end(), 
        [](const Mob_pos &a, const Mob_pos &b) -> bool {return a.sprite->getPosition().y < b.sprite->getPosition().y;}); // Heyy le flex, j'ai utilisé une lambda
	for(auto mob : _mobs){
		// file << static_cast<int>(getTypeFromName(mob.name)) << " " << static_cast<int>(((mob.sprite->getPosition()).y)-130) / 49  << " " << static_cast<int>((mob.sprite->getPosition().x) / GRAPHIC_X_MULTIPLIER) << std::endl;
        level += std::to_string(static_cast<int>(getTypeFromName(mob.name)));
        level += " ";
        level += std::to_string(static_cast<int>(((mob.sprite->getPosition()).y)-130) / 49);
        level += " ";
        level += std::to_string(static_cast<int>((mob.sprite->getPosition().x) / GRAPHIC_X_MULTIPLIER));
        level += "\n";
    } 
    auto all_level_to_send = level.c_str();
    auto* command = Game::CommandFactory::createLevel(_user_name, _level_name, all_level_to_send);
    _client->getConnection()->getSendingCommandQueue()->push(command);
}


void Menu::LevelCreateMenu::setTextError() noexcept{
   _textes[1]->setPosition(220.f, 425.f);
   _textes[1]->setTexte("Name of Level already taken");
}

bool Menu::LevelCreateMenu::testValues() noexcept {
    if (!(_boss_posed && !_mobs.empty())){
        _textes[0]->setTexte("A level cannot be created without a boss.");
        return false;
    }
    return true;
}