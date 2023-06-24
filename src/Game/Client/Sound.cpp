#include "Sound.hpp"
#include <iostream>
#include <sstream>
#include "../../Utils/DebugLogger.hpp"
using namespace std;

Game::Client::Sound::Sound() { // loading sound effect and generic
    if (!_generic.openFromFile("AudioFiles/generic.ogg")) {
        DEBUGMSG("ERROR loading Generic Sound file");
    }else{
        _generic.setLoop(true);
        _generic.setVolume(50);
        _generic.play();
    }

    if (!_hit_rock_sound_buffer.loadFromFile("AudioFiles/SoundEffect/rock_crash.ogg")) {
        DEBUGMSG("ERROR loading Hit Rock Sound Effect");
    }else{
        _hit_rock_sound.setBuffer(_hit_rock_sound_buffer);
    }

    if (!_hit_enemy_sound_buffer.loadFromFile("AudioFiles/SoundEffect/enemy_ship_crash.ogg")) {
        DEBUGMSG("ERROR loading Hit Enemy Sound Effect");
    }else{
        _hit_enemy_sound.setBuffer(_hit_enemy_sound_buffer);
    }

    if (!_bonus_recovery_buffer.loadFromFile("AudioFiles/SoundEffect/bonus_recovery.ogg")) {
        DEBUGMSG("ERROR loading Bonus Recovery Sound Effect");
    }else{
        _bonus_recovery.setBuffer(_bonus_recovery_buffer);
    }

    if (!_ship_fire_buffer.loadFromFile("AudioFiles/SoundEffect/ship_fire.ogg")) {
        DEBUGMSG("ERROR loading Ship Fire Sound Effect");
    }else{
        _ship_fire.setBuffer(_ship_fire_buffer);
    }
}

void Game::Client::Sound::setLevelSound(int lvl) {
    if (lvl == 1) {
        _generic.stop();
    } else {
    	_boss.stop();
    }
    string str;
    stringstream ss;
    ss << lvl;
    ss >> str; // line [47-50] => to convert an int to a string
    string buffer = "AudioFiles/LevelSound/lvl_";
    buffer += str;
    buffer += ".ogg";
    if (!_level_sound.openFromFile(buffer)) {
        DEBUGMSG("ERROR loading level Sound file");
    } else{
        _level_sound.setLoop(true);
        if(_is_mute)
            _level_sound.setVolume(0);
        else
            _level_sound.setVolume(50);
        _level_sound.play();
    }
}

void Game::Client::Sound::bossArriving(int lvl) {
    _level_sound.stop();
    string str;
    stringstream ss;
    ss << lvl;
    ss >> str; // line [65-68] => to convert an int to a string
    string buffer = "AudioFiles/LevelSound/boss_";
    buffer += str;
    buffer += ".ogg";
    if (!_boss.openFromFile(buffer)){
        DEBUGMSG("ERROR loading boss Sound file");
    }else{
        _boss.setLoop(true);
        if(_is_mute)
            _boss.setVolume(0);
        else
            _boss.setVolume(30);
        _boss.play();
    }
}

void Game::Client::Sound::isOnPause(bool pause) {
    if (pause){ // to pause during the game
        if(_level_sound.getStatus() == sf::SoundSource::Playing){ // pause if we don't play against the boss
            _level_sound.pause();
        }else{
            _boss.pause();
        }
    }else {
        if(_level_sound.getStatus() == sf::SoundSource::Paused){
            _level_sound.play();
        }else{
            _boss.play();
        }
    }
}

void Game::Client::Sound::isWinner(bool win){
    _boss.stop();
    if (win){ // load the music depending on whether we won or not
        if (!_win_or_loose.openFromFile("AudioFiles/winner.ogg")){
            DEBUGMSG("ERROR loading winner Sound file");
        }
    }else {
        if (!_win_or_loose.openFromFile("AudioFiles/looser.ogg")){
            DEBUGMSG("ERROR loading looser Sound file");
        }
    }
    if (_level_sound.getStatus() == sf::SoundSource::Playing) {
        _level_sound.stop();
    }
    _win_or_loose.setLoop(true);
    _win_or_loose.play();
    if(_is_mute)
        _win_or_loose.setVolume(0);
    else
        _win_or_loose.setVolume(50);

}

void Game::Client::Sound::toggleMute(){
    if (_generic.getVolume()!=0){ // if the music is unmute
        _generic.setVolume(0);
        _is_mute=true;
    }else{
        _generic.setVolume(50);
        _is_mute=false;
    }
}

void Game::Client::Sound::stopMusic(){
    if (_win_or_loose.getStatus() == sf::SoundSource::Playing){
        _win_or_loose.stop();
    }else if (_level_sound.getStatus() == sf::SoundSource::Playing){
        _level_sound.stop();
    }else if (_boss.getStatus() == sf::SoundSource::Playing){
        _boss.stop();
    }
    _generic.play();
}

void Game::Client::Sound::playSoundEffect(Game::Client::SoundEffect sound) {
	if(!_is_mute) {
		switch(sound){
		    case SoundEffect::HitRockSound:{
		        _hit_rock_sound.play();
		        break;
		    }
		    case SoundEffect::HitEnemySound:{
		        _hit_enemy_sound.play();
		        break;
		    }
		    case SoundEffect::BonusRecovery:{
		        _bonus_recovery.play();
		        break;
		    }
		    case SoundEffect::ShipFire:{
		        _ship_fire.play();
		        break;
		    }
            default:
                break;
		}
    }
}
