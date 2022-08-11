#include <SFML/Graphics.hpp>

#include "game.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Doodly", sf::Style::Fullscreen);
    window.setVerticalSyncEnabled(true);

    Game game(window);

    game.run();
}