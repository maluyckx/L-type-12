#ifndef L_TYPE_GROUPE_12_2020_SOUND_HPP
#define L_TYPE_GROUPE_12_2020_SOUND_HPP

#include <SFML/Audio.hpp>

namespace Game::Client{
    enum class SoundEffect{
        HitRockSound,
        HitEnemySound,
        BonusRecovery,
        ShipFire,
        BossArrive,
        LevelChange
    };

    class Sound{
    bool _is_mute = false;
    sf::Music _generic{};
    sf::Music _level_sound{};
    sf::Music _win_or_loose{};
    sf::Music _boss{};

    sf::SoundBuffer _hit_rock_sound_buffer{};
    sf::SoundBuffer _hit_enemy_sound_buffer{};
    sf::SoundBuffer _bonus_recovery_buffer{};
    sf::SoundBuffer _ship_fire_buffer{};

    sf::Sound _hit_rock_sound{};
    sf::Sound _hit_enemy_sound{};
    sf::Sound _bonus_recovery{};
    sf::Sound _ship_fire{};
    
    public:
        Sound();
        void playSoundEffect(Game::Client::SoundEffect);
        void isOnPause(bool);
        void isWinner(bool);  //revoir le nom de cette methode
        void setLevelSound(int);
        void bossArriving(int);
        void toggleMute();
        inline bool isMute() noexcept {return _is_mute;}
        void stopMusic();
        //~Sound() noexcept = default;
    };
}
#endif //L_TYPE_GROUPE_12_2020_SOUND_HPP