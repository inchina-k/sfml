#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "player.hpp"
#include "block.hpp"
#include "ball.hpp"

#include <iostream>
#include <math.h>

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML app", sf::Style::Fullscreen);

    const sf::Time frames_per_sec = sf::seconds(0.01f);
    sf::Time total_time = sf::Time::Zero;
    sf::Clock clock;

    Player player(window, window.getSize().x / 2, window.getSize().y - player.get_size().y - 20);

    Block block(window, window.getSize().x / 2, window.getSize().y / 3);

    Ball ball(window, window.getSize().x / 2, window.getSize().y / 2);

    sf::Vertex points[2];
    float speed_val = std::min(window.getSize().x, window.getSize().y);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }

            // player.set_pos(sf::Mouse::getPosition(window).x);
            if (ball.get_state() == States::SetPos && !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                ball.set_pos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
            }
            else if (ball.get_state() == States::SetPos && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                ball.set_state(States::SetDir);
                points[0].position.x = sf::Mouse::getPosition().x;
                points[0].position.y = sf::Mouse::getPosition().y;

                points[1].position.x = sf::Mouse::getPosition().x;
                points[1].position.y = sf::Mouse::getPosition().y;
            }
            else if (ball.get_state() == States::SetDir && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                points[1].position.x = sf::Mouse::getPosition().x;
                points[1].position.y = sf::Mouse::getPosition().y;
            }
            else if (ball.get_state() == States::SetDir && !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                ball.set_state(States::Move);

                float dx = sf::Mouse::getPosition().x - ball.get_pos().x;
                float dy = sf::Mouse::getPosition().y - ball.get_pos().y;

                float len = std::sqrt(dx * dx + dy * dy);

                sf::Vector2f speed(speed_val * dx / len, speed_val * dy / len);
                ball.set_speed(speed);
            }
        }

        total_time += clock.restart();

        while (total_time > frames_per_sec)
        {
            total_time -= frames_per_sec;

            if (ball.get_state() == States::Move)
            {
                ball.move(frames_per_sec.asSeconds(), player, block);
            }
        }

        window.clear();

        // player.draw();
        block.draw();
        ball.draw();

        if (ball.get_state() == States::SetDir)
        {
            window.draw(points, 2, sf::Lines);
        }

        window.display();
    }

#ifdef AUCA_DEBUG
    std::clog << "ok" << std::endl;
#endif
}