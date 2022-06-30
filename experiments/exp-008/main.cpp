#include <SFML/Graphics.hpp>
#include "game.hpp"

#include <iostream>

using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML app", sf::Style::Fullscreen);
    window.setVerticalSyncEnabled(true);

    const int size = 10;

    int width = window.getSize().x;
    int height = window.getSize().y;

    Game game(size, width, height);

    game.run(window);

#ifdef AUCA_DEBUG
    std::clog << "ok" << std::endl;
#endif
}