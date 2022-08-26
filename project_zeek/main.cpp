#include <SFML/Graphics.hpp>

#include "game.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "d&d", sf::Style::Fullscreen);
    window.setVerticalSyncEnabled(true);

    sf::Font font;
    if (!font.loadFromFile("data/fonts/PressStart2P-Regular.ttf"))
    {
        exit(1);
    }

    Game game(window, font);

    game.run();
}