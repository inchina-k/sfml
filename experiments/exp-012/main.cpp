#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "player.hpp"
#include "block.hpp"
#include "ball.hpp"

#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML app", sf::Style::Fullscreen);

    const sf::Time frames_per_sec = sf::seconds(0.01f);
    sf::Time total_time = sf::Time::Zero;
    sf::Clock clock;

    Player player(window, window.getSize().x / 2, window.getSize().y - player.get_size().y - 20);

    Block block(window, window.getSize().x / 2, window.getSize().y / 3);

    Ball ball(window, window.getSize().x / 2, window.getSize().y / 2);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }

            player.set_pos(sf::Mouse::getPosition(window).x);
        }

        total_time += clock.restart();

        while (total_time > frames_per_sec)
        {
            total_time -= frames_per_sec;

            ball.move(frames_per_sec.asSeconds(), player, block);
        }

        window.clear();

        player.draw();
        block.draw();
        ball.draw();

        window.display();
    }

#ifdef AUCA_DEBUG
    std::clog << "ok" << std::endl;
#endif
}