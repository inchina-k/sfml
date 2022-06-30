#include <SFML/Graphics.hpp>

#include <iostream>

using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML app", sf::Style::Fullscreen);
    window.setVerticalSyncEnabled(true);

    const sf::Time frames_per_sec = sf::seconds(0.2f);
    sf::Time total_time = sf::Time::Zero;
    sf::Clock clock;

    const int size = 10;

    int width = window.getSize().x;
    int height = window.getSize().y;

    int cell_size = min(width, height) / (2 * size);

    int rect_x = width / 2 - cell_size * (size / 2);
    int rect_y = height / 2 - cell_size * (size / 2);

    sf::RectangleShape cell(sf::Vector2f(cell_size, cell_size));
    cell.setFillColor(sf::Color(189, 203, 219));
    cell.setOutlineThickness(1);
    cell.setOutlineColor(sf::Color(225, 237, 252));

    int circle_x = width / 2 - cell_size * (size / 2);
    int circle_y = height / 2 - cell_size * (size / 2);

    sf::CircleShape circle;
    circle.setRadius(cell_size / 2);
    circle.setFillColor(sf::Color::Blue);

    circle.setPosition(circle_x, circle_y);

    int dir = 0;
    int col = 0;
    int row = 0;

    while (window.isOpen())
    {
        total_time += clock.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }
        }

        if (total_time > frames_per_sec)
        {
            total_time -= frames_per_sec;

            if (dir == 0)
            {
                circle_x += cell_size;
                ++col;
                if (col == size)
                {
                    dir = 1;
                    col = size - 1;
                    circle_x -= cell_size;
                }
            }
            else if (dir == 1)
            {
                circle_y += cell_size;
                ++row;
                if (row == size)
                {
                    dir = 2;
                    row = size - 1;
                    circle_y -= cell_size;
                }
            }
            else if (dir == 2)
            {
                circle_x -= cell_size;
                --col;
                if (col == -1)
                {
                    dir = 3;
                    col = 0;
                    circle_x += cell_size;
                }
            }
            else if (dir == 3)
            {
                circle_y -= cell_size;
                --row;
                if (row == -1)
                {
                    dir = 0;
                    row = 0;
                    circle_y += cell_size;
                }
            }
        }

        window.clear();

        sf::Vector2f pos(rect_x, rect_y);

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                cell.setPosition(pos.x, pos.y);
                window.draw(cell);

                pos.x += cell.getSize().x;
            }

            pos.x = rect_x;
            pos.y += cell.getSize().x;
        }

        circle.setPosition(circle_x, circle_y);
        window.draw(circle);

        window.display();
    }

#ifdef AUCA_DEBUG
    std::clog << "ok" << std::endl;
#endif
}