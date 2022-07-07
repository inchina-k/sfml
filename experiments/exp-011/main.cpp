#include <SFML/Graphics.hpp>

#include <iostream>
#include "background_animations.hpp"
#include "snowflakes.hpp"
#include "flowers.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML app", sf::Style::Fullscreen);
    window.setVerticalSyncEnabled(true);

    int bg_anim_index = 0;

    // Snowflakes snowflakes(window, window.getSize().x / 20);
    Flowers flowers(window, window.getSize().x / 20);

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }
            // else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
            // {
            // }
        }

        // snowflakes.load_animation();
        flowers.load_animation();

        window.clear();
        // snowflakes.draw_animation();
        flowers.draw_animation();
        window.display();
    }

#ifdef AUCA_DEBUG
    std::clog << "ok" << std::endl;
#endif
}