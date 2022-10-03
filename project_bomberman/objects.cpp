#include "game.hpp"

#include "../libs/random.hpp"
#include <iostream>

using Random = effolkronium::random_static;

Game::GameObject::GameObject(Game &game, sf::Texture &texture, sf::Vector2f &size, sf::Vector2f &pos, int row, int col)
    : m_game(game), m_texture(texture), m_pos(pos)
{
    m_body.setTexture(m_texture);
    m_body.setScale(size.x / m_texture.getSize().x,
                    size.y / m_texture.getSize().y);
    m_body.setPosition(m_pos);

    m_row = row;
    m_col = col;
}

sf::Vector2f Game::GameObject::get_size() const
{
    return sf::Vector2f(m_body.getGlobalBounds().width, m_body.getGlobalBounds().height);
}

sf::Vector2f Game::GameObject::get_pos() const
{
    return m_body.getPosition();
}

sf::FloatRect Game::GameObject::get_bounds() const
{
    return m_body.getGlobalBounds();
}

Game::SafeCell::SafeCell(Game &game, sf::Texture &texture, sf::Vector2f &size, sf::Vector2f &pos, int row, int col)
    : GameObject(game, texture, size, pos, row, col)
{
}

void Game::SafeCell::draw()
{
    m_game.m_window.draw(m_body);
}

Game::Wall::Wall(Game &game, sf::Texture &texture, sf::Vector2f &size, sf::Vector2f &pos, int row, int col)
    : GameObject(game, texture, size, pos, row, col)
{
}

void Game::Wall::draw()
{
    m_game.m_window.draw(m_body);
}

Game::Enemy::Enemy(Game &game, sf::Texture &texture, sf::Vector2f &size, sf::Vector2f &pos, int row, int col)
    : GameObject(game, texture, size, pos, row, col), m_dir(1, 0)
{
    m_w = m_texture.getSize().x / 3;
    m_h = m_texture.getSize().y;

    load(size);

    m_row = row;
    m_col = col;

    m_step = m_game.m_cells.front().front()->get_size().x / 50;
}

void Game::Enemy::load(sf::Vector2f &size)
{
    m_frames.resize(1);

    for (size_t i = 0; i < 1; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            m_frames[i].push_back(std::make_unique<sf::Sprite>(m_texture, sf::IntRect(j * m_w, i * m_h, m_w, m_h)));
        }
    }

    for (auto &frames : m_frames)
    {
        for (auto &frame : frames)
        {
            frame->setScale(size.x / frame->getLocalBounds().width, size.y / frame->getLocalBounds().height);
        }
    }
}

// bool Game::Enemy::can_move()
// {
//     int new_row = m_row;
//     int new_col = m_col;

//     if (m_dir.x == 1 || m_dir.y == 1)
//     {
//         new_row += m_dir.y;
//         new_col += m_dir.x;
//     }

//     if (new_row > int(m_game.m_cells.size() - 1) || new_col > int(m_game.m_cells.front().size() - 1) ||
//         new_row < 0 || new_col < 0)
//     {
//         return false;
//     }
//     else if (auto obj = dynamic_cast<Wall *>(m_game.m_cells[new_row][new_col].get()))
//     {
//         return false;
//     }

//     return true;
// }

void Game::Enemy::set_dir()
{
    std::vector<sf::Vector2i> m_dirs;

    std::vector<int> d_row = {1, 0, 0, -1};
    std::vector<int> d_col = {0, 1, -1, 0};

    for (size_t i = 0; i < d_row.size(); i++)
    {
        if (auto obj = dynamic_cast<SafeCell *>(m_game.m_cells[m_row + d_row[i]][m_col + d_col[i]].get()))
        {
            m_dirs.emplace_back(d_col[i], d_row[i]);
        }
    }

    if (!m_dirs.empty())
    {
        int index = Random::get(0, int(m_dirs.size() - 1));
        m_dir = m_dirs[index];
    }
}

void Game::Enemy::move()
{
    std::cout << "move: " << m_row << ' ' << m_col << std::endl;

    m_pos.x += m_dir.x * m_step;
    m_pos.y += m_dir.y * m_step;

    m_row = (m_pos.y) / m_game.m_cells.front().front()->get_size().y;
    m_col = (m_pos.x) / m_game.m_cells.front().front()->get_size().x;

    int new_row = m_row;
    int new_col = m_col;

    if (m_dir.x == 1 || m_dir.y == 1)
    {
        new_row += m_dir.y;
        new_col += m_dir.x;
    }

    if (auto obj = dynamic_cast<Wall *>(m_game.m_cells[new_row][new_col].get()))
    {
        m_pos.x -= m_dir.x * m_step;
        m_pos.y -= m_dir.y * m_step;

        m_row = (m_pos.y) / m_game.m_cells.front().front()->get_size().y;
        m_col = (m_pos.x) / m_game.m_cells.front().front()->get_size().x;

        set_dir();
    }
}

void Game::Enemy::draw()
{
    m_frames[m_anim_index][m_frame_index]->setPosition(m_pos);
    m_game.m_window.draw(*m_frames[m_anim_index][m_frame_index]);

    if (++m_counter == m_max_counter)
    {
        if (++m_frame_index == 3)
        {
            m_frame_index = 0;
        }

        m_counter = 0;
    }
}