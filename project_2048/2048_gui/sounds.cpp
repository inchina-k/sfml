#include "sounds.hpp"

Sounds::Sounds()
    : m_play_sound(true)
{
    sf::SoundBuffer buff_cell_moved, buff_game_won, buff_game_lost;

    if (!buff_cell_moved.loadFromFile("data/pop.wav") || !buff_game_won.loadFromFile("data/game_won.wav") || !buff_game_lost.loadFromFile("data/game_lost.wav"))
    {
        exit(1);
    }

    m_sound_cell_moved.setBuffer(buff_cell_moved);
    m_sound_game_won.setBuffer(buff_game_won);
    m_sound_game_lost.setBuffer(buff_game_lost);

    m_sound_game_won.setLoop(false);
    m_sound_game_lost.setLoop(false);
}

void Sounds::play_sound_cell_moved()
{
    m_sound_cell_moved.play();
}

void Sounds::play_sound_game_won()
{
    m_sound_game_won.play();
}

void Sounds::play_sound_game_lost()
{
    m_sound_game_lost.play();
}

bool Sounds::get_play_sound() const
{
    return m_play_sound;
}

void Sounds::set_play_sound(bool play_sound)
{
    m_play_sound = play_sound;
}