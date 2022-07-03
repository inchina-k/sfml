#pragma once

#include <SFML/Audio.hpp>

class Sounds
{
    sf::Sound m_sound_cell_moved;
    sf::Sound m_sound_game_won;
    sf::Sound m_sound_game_lost;

    bool m_play_sound;

public:
    Sounds();

    void play_sound_cell_moved();

    void play_sound_game_won();

    void play_sound_game_lost();

    bool get_play_sound() const;

    void set_play_sound(bool play_sound);
};