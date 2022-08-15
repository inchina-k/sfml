#include "game.hpp"

#include <iostream>
#include <unordered_map>

Game::Game(sf::RenderWindow &window)
    : m_window(window), m_curr_level(0), m_player(window)
{
    if (!load_levels())
    {
        exit(1);
    }

    load_field();
}

Game::GameObject::GameObject(Game &game, sf::Texture &texture, int row, int col)
    : m_game(game), m_texture(texture), m_row(row), m_col(col)
{
    m_body.setTexture(m_texture);
    m_body.scale(game.m_cells.front()->get_size().x / m_texture.getSize().x,
                 game.m_cells.front()->get_size().y / m_texture.getSize().y);
    //  m_body.setOrigin(m_body.getLocalBounds().width / 2, m_body.getLocalBounds().height / 2);
}

Game::Bonus::Bonus(Game &game, sf::Texture &texture, int row, int col)
    : GameObject(game, texture, row, col)
{
}

void Game::Bonus::draw()
{
    m_game.m_window.draw(m_body);
}

Game::Hazard::Hazard(Game &game, sf::Texture &texture, int row, int col)
    : GameObject(game, texture, row, col)
{
}

void Game::Hazard::draw()
{
    m_game.m_window.draw(m_body);
}

Game::Fruit::Fruit(Game &game, sf::Texture &texture, int row, int col)
    : GameObject(game, texture, row, col)

{
}

void Game::Fruit::draw()
{
    m_game.m_window.draw(m_body);
}

bool Game::Fruit::move(int dx, int dy)
{
    m_x += dx;
    m_y += dy;
    return true;
}

Game::Ball::Ball(Game &game, sf::Texture &texture, int row, int col)
    : GameObject(game, texture, row, col)
{
}

void Game::Ball::draw()
{
    m_game.m_window.draw(m_body);
}

bool Game::Ball::move(int dx, int dy)
{
    m_x += dx;
    m_y += dy;
    return false;
}

bool Game::load_levels()
{
    std::fstream fs("data/levels.data");

    if (!fs)
    {
        return false;
    }

    for (;;)
    {
        std::string line;

        if (!std::getline(fs, line))
        {
            break;
        }

        m_titles.push_back(line);

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
    }

    return true;
}

void Game::load_field()
{
    float cell_size = std::min(m_window.getSize().x, m_window.getSize().y) / (m_levels[m_curr_level].size() + 2);
    sf::Vector2f block_size(cell_size, cell_size);
    m_player.set_size(cell_size);

    float x = m_window.getSize().x / 2 - ((m_levels[m_curr_level][0].size() / 2.0f) * cell_size);
    float y = m_window.getSize().y / 2 - ((m_levels[m_curr_level].size() / 2.0f) * cell_size);
    sf::Vector2f pos(x, y);

    sf::Texture safe_cell, wall, bonus, fruit, ball, hazard;

    if (!safe_cell.loadFromFile("data/images/safe_cell.png") ||
        !wall.loadFromFile("data/images/wall.png") ||
        !bonus.loadFromFile("data/images/bonus.png") ||
        !fruit.loadFromFile("data/images/fruit.png") ||
        !ball.loadFromFile("data/images/ball.png") ||
        !hazard.loadFromFile("data/images/droodle.png"))
    {
        exit(1);
    }

    std::unordered_map<char, sf::Texture> cell_types = {{'.', safe_cell}, {'w', wall}, {'b', bonus}, {'f', fruit}, {'l', ball}, {'h', hazard}};

    for (size_t i = 0; i < m_levels[m_curr_level].size(); i++)
    {
        for (size_t j = 0; j < m_levels[m_curr_level][i].size(); j++)
        {
            char type = m_levels[m_curr_level][i][j];

            if (type == 'w')
            {
                m_cells.push_back(std::make_unique<Cell>(m_window, block_size, pos, cell_types[type]));
            }
            else
            {
                m_cells.push_back(std::make_unique<Cell>(m_window, block_size, pos, cell_types['.']));

                if (type == 'p')
                {
                    sf::Vector2f coords(x + cell_size / 2, y + cell_size / 2);
                    m_player.set_pos(coords);
                }
                else if (type == 'b')
                {
                    m_objects.push_back(std::make_unique<Bonus>(*this, cell_types[type], i, j));
                    m_objects.back()->set_pos(pos);
                }
                else if (type == 'h')
                {
                    m_objects.push_back(std::make_unique<Hazard>(*this, cell_types[type], i, j));
                    m_objects.back()->set_pos(pos);
                }
                else if (type == 'f')
                {
                    m_objects.push_back(std::make_unique<Fruit>(*this, cell_types[type], i, j));
                    m_objects.back()->set_pos(pos);
                }
                else if (type == 'l')
                {
                    m_objects.push_back(std::make_unique<Ball>(*this, cell_types[type], i, j));
                    m_objects.back()->set_pos(pos);
                }
            }

            pos.x += cell_size;
        }

        pos.x = x;
        pos.y += cell_size;
    }
}

void Game::run()
{
    sf::Vector2f top = m_cells.front()->get_pos();
    sf::Vector2f bottom = m_cells.back()->get_pos();

    float size = m_cells.back()->get_size().x;
    float x = m_window.getSize().x / 2 - ((m_levels[m_curr_level][0].size() / 2.0f) * size);
    float y = m_window.getSize().y / 2 - ((m_levels[m_curr_level].size() / 2.0f) * size);

    while (m_window.isOpen())
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                m_window.close();
            }

            m_player.move(top, bottom, size, m_levels[m_curr_level], x, y);
        }

        m_window.clear();

        for (auto &cell : m_cells)
        {
            cell->draw();
        }

        for (auto &object : m_objects)
        {
            object->draw();
        }

        m_player.draw();

        m_window.display();
    }
}