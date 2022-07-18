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
    bg_music.setLoop(true);

    Game game(window, font);

    bg_music.play();

    game.run();

#ifdef AUCA_DEBUG
    std::clog << "ok" << std::endl;
#endif
}