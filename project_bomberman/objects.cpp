#include "game.hpp"

Game::GameObject::GameObject(Game &game, sf::Texture &texture, sf::Vector2f &size, sf::Vector2f &pos)
    : m_game(game), m_texture(texture), m_pos(pos)
{
    m_body.setTexture(m_texture);
    m_body.setScale(size.x / m_texture.getSize().x,
                    size.y / m_texture.getSize().y);
    m_body.setPosition(m_pos);
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

Game::SafeCell::SafeCell(Game &game, sf::Texture &texture, sf::Vector2f &size, sf::Vector2f &pos)
    : GameObject(game, texture, size, pos)
{
}

void Game::SafeCell::draw()
{
    m_game.m_window.draw(m_body);
}

Game::Wall::Wall(Game &game, sf::Texture &texture, sf::Vector2f &size, sf::Vector2f &pos)
    : GameObject(game, texture, size, pos)
{
}

void Game::Wall::draw()
{
    m_game.m_window.draw(m_body);
}

Game::Enemy::Enemy(Game &game, sf::Texture &texture, sf::Vector2f &size, sf::Vector2f &pos)
    : GameObject(game, texture, size, pos)
{
    m_w = m_texture.getSize().x / 3;
    m_h = m_texture.getSize().y;

    load(size);
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