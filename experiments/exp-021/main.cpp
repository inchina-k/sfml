#include <SFML/Graphics.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>

std::vector<std::vector<std::string>> m_levels;
std::vector<std::vector<std::unique_ptr<sf::RectangleShape>>> objects;

bool load_levels()
{
    std::fstream fs("levels.data");

    if (!fs)
    {
        return false;
    }

    std::string line;

    if (!std::getline(fs, line))
    {
        std::cout << "height is not found" << std::endl;
        return false;
    }

    int height;
    std::istringstream ss(line);

    if (!(ss >> height))
    {
        std::cout << "height is not an int" << std::endl;
        return false;
    }

    std::vector<std::string> level;

    for (int i = 0; i < height; i++)
    {
        if (!std::getline(fs, line))
        {
            std::cout << "level is not found" << std::endl;
            return false;
        }

        level.push_back(line);
    }

    m_levels.push_back(level);

    return true;
}

bool check_collision(sf::Vector2f &pos, sf::RectangleShape &rect)
{
    for (auto &obj : objects)
    {
        for (auto &o : obj)
        {
            if (o)
            {
                sf::FloatRect rect_tmp(pos, rect.getSize());
                sf::FloatRect wall_tmp(o->getPosition(), o->getSize());

                if (rect_tmp.intersects(wall_tmp))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML app");
    window.setVerticalSyncEnabled(true);

    load_levels();
    std::vector<std::string> m_level = m_levels[0];

    float cell_size = std::min(window.getSize().x, window.getSize().y) / (m_level.size() + 2);
    sf::Vector2f block_size(cell_size, cell_size);

    float x = window.getSize().x / 2 - ((m_level.front().size() / 2.0f) * cell_size);
    float y = window.getSize().y / 2 - ((m_level.size() / 2.0f) * cell_size);
    sf::Vector2f cell_pos(x, y);

    objects.resize(m_level.size());

    for (size_t i = 0; i < m_level.size(); i++)
    {
        for (size_t j = 0; j < m_level[i].size(); j++)
        {
            char type = m_level[i][j];

            if (type == 'w')
            {
                objects[i].push_back(std::make_unique<sf::RectangleShape>(block_size));
                objects[i].back()->setPosition(cell_pos);
                objects[i].back()->setFillColor(sf::Color::White);
            }
            else
            {
                objects[i].push_back(nullptr);
            }

            cell_pos.x += cell_size;
        }

        cell_pos.x = x;
        cell_pos.y += cell_size;
    }

    sf::RectangleShape rect;
    sf::Vector2f pos(window.getSize().x / 2, window.getSize().y / 2);
    rect.setPosition(pos);
    rect.setFillColor(sf::Color::Cyan);
    rect.setSize(block_size * 0.98f);

    float step = rect.getSize().x / 20;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                sf::Vector2f new_pos(sf::Vector2f(rect.getPosition().x - step, rect.getPosition().y));
                if (!check_collision(new_pos, rect))
                {
                    rect.setPosition(new_pos);
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                sf::Vector2f new_pos(rect.getPosition().x + step, rect.getPosition().y);
                if (!check_collision(new_pos, rect))
                {
                    rect.setPosition(new_pos);
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                sf::Vector2f new_pos(rect.getPosition().x, rect.getPosition().y - step);
                if (!check_collision(new_pos, rect))
                {
                    rect.setPosition(new_pos);
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                sf::Vector2f new_pos(rect.getPosition().x, rect.getPosition().y + step);
                if (!check_collision(new_pos, rect))
                {
                    rect.setPosition(new_pos);
                }
            }
        }

        window.clear();
        for (auto &obj : objects)
        {
            for (auto &o : obj)
            {
                if (o)
                    window.draw(*o);
            }
        }
        window.draw(rect);
        window.display();
    }
}