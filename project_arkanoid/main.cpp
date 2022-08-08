#include <SFML/Audio.hpp>
#include "game.hpp"

#include <iostream>
#include <memory>

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Arkanoid", sf::Style::Fullscreen);

    sf::Font font;
    font.loadFromFile("data/fonts/PressStart2P-Regular.ttf");

    sf::Music bg_music;
    bg_music.openFromFile("data/sounds/background.wav");

    sf::SoundBuffer hit, pop, crack, unbreakable, bonus_start, bonus_end, game_won, game_lost;
    sf::Sound sound_hit, sound_pop, sound_crack, sound_unbreakable, sound_bonus_start, sound_bonus_end, sound_game_won, sound_game_lost;

    if (!hit.loadFromFile("data/sounds/hit.wav") ||
        !pop.loadFromFile("data/sounds/pop.wav") ||
        !crack.loadFromFile("data/sounds/crack.wav") ||
        !unbreakable.loadFromFile("data/sounds/unbreakable.wav") ||
        !bonus_start.loadFromFile("data/sounds/bonus_start.wav") ||
        !bonus_end.loadFromFile("data/sounds/bonus_end.wav") ||
        !game_won.loadFromFile("data/sounds/game_won.wav") ||
        !game_lost.loadFromFile("data/sounds/game_lost.wav"))
    {
        exit(1);
    }

    sound_hit.setBuffer(hit);
    sound_pop.setBuffer(pop);
    sound_crack.setBuffer(crack);
    sound_unbreakable.setBuffer(unbreakable);
    sound_bonus_start.setBuffer(bonus_start);
    sound_bonus_end.setBuffer(bonus_end);
    sound_game_won.setBuffer(game_won);
    sound_game_lost.setBuffer(game_lost);

    Game game(window, font, bg_music, sound_game_won, sound_game_lost, sound_hit, sound_pop, sound_crack, sound_unbreakable, sound_bonus_start, sound_bonus_end);

    game.run();

#ifdef AUCA_DEBUG
    std::clog << "ok" << std::endl;
#endif
}