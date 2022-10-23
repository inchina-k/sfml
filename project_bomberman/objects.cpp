#include "game.hpp"

#include <iostream>

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

bool Game::GameObject::is_active() const
{
    return m_active;
}

void Game::GameObject::set_active(bool b)
{
    m_active = b;
}

int Game::GameObject::get_curr_anim_index() const
{
    return m_anim_index;
}

int Game::GameObject::get_curr_frame_index() const
{
    return m_frame_index;
}

void Game::GameObject::set_curr_anim_index(int index)
{
    m_anim_index = index;
}

void Game::GameObject::set_curr_frame_index(int index)
{
    m_frame_index = index;
}

int Game::GameObject::get_row() const
{
    return m_row;
}

int Game::GameObject::get_col() const
{
    return m_col;
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

Game::Bomb::Bomb(Game &game, sf::Texture &texture, sf::Vector2f &size, sf::Vector2f &pos, int row, int col, int cells)
    : GameObject(game, texture, size, pos, row, col), m_cells(cells)
{
    m_w = m_texture.getSize().x / 2;
    m_h = m_texture.getSize().y / 2;

    load(size);

    m_row = row;
    m_col = col;
}

void Game::Bomb::load(sf::Vector2f &size)
{
    m_frames.resize(2);

    for (size_t i = 0; i < 2; i++)
    {
        for (size_t j = 0; j < 2; j++)
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

sf::FloatRect Game::Bomb::get_bounds() const
{
    return m_frames[m_anim_index][m_frame_index]->getGlobalBounds();
}

void Game::Bomb::set_explosion(int r, int c, int dr, int dc)
{
    int row = r + dr;
    int col = c + dc;

    sf::Vector2f size = m_game.m_walls.front().front()->get_size();

    for (int i = 0; i < m_cells; i++)
    {
        if (m_game.m_walls[row][col])
        {
            break;
        }

        sf::Vector2f pos = m_game.m_cells[row][col]->get_pos();
        m_game.m_explosions[row][col].reset(new Explosion(m_game, m_game.m_texture_explosion, size, pos, row, col));
        m_game.m_explosions[row][col]->set_active(true);

        row += dr;
        col += dc;
    }
}

void Game::Bomb::handle_explosion()
{
    if (--m_explosion_counter == 0)
    {
        m_anim_index = 1;

        set_explosion(m_row, m_col, 1, 0);
        set_explosion(m_row, m_col, -1, 0);
        set_explosion(m_row, m_col, 0, 1);
        set_explosion(m_row, m_col, 0, -1);

        m_game.m_explosions[m_row][m_col]->set_active(true);
    }
    else if (m_explosion_counter < -70)
    {
        for (auto &explosions : m_game.m_explosions)
        {
            for (auto &explosion : explosions)
            {
                explosion.release();
            }
        }

        m_game.m_bomb_deployed = false;
    }
}

void Game::Bomb::draw()
{
    for (auto &frames : m_frames)
    {
        for (auto &frame : frames)
        {
            frame->setPosition(m_pos);
        }
    }

    m_game.m_window.draw(*m_frames[m_anim_index][m_frame_index]);

    handle_explosion();

    if (++m_counter == m_max_counter)
    {
        if (++m_frame_index == 2)
        {
            m_frame_index = 0;
        }

        m_counter = 0;
    }
}

Game::Explosion::Explosion(Game &game, sf::Texture &texture, sf::Vector2f &size, sf::Vector2f &pos, int row, int col)
    : GameObject(game, texture, size, pos, row, col)
{
    m_w = m_texture.getSize().x / 2;
    m_h = m_texture.getSize().y;

    load(size);

    m_row = row;
    m_col = col;
}

void Game::Explosion::load(sf::Vector2f &size)
{
    m_frames.resize(1);

    for (size_t i = 0; i < 1; i++)
    {
        for (size_t j = 0; j < 2; j++)
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

sf::FloatRect Game::Explosion::get_bounds() const
{
    return m_frames[m_anim_index][m_frame_index]->getGlobalBounds();
}

void Game::Explosion::draw()
{
    for (auto &frames : m_frames)
    {
        for (auto &frame : frames)
        {
            frame->setPosition(m_pos);
        }
    }

    m_game.m_window.draw(*m_frames[m_anim_index][m_frame_index]);

    if (++m_counter == m_max_counter)
    {
        if (++m_frame_index == 2)
        {
            m_frame_index = 0;
        }

        m_counter = 0;
    }
}