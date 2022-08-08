#include "block.hpp"

#include "../libs/random.hpp"

using Random = effolkronium::random_static;

Block::Block(void *game, sf::RenderWindow &window,
             sf::Vector2f &size, sf::Vector2f &pos, sf::Texture &texture, int health,
             bool is_bonus, void start_magic(void *, void *), void end_magic(void *))
    : m_game(game), m_window(window),
      m_body(size), m_pos(pos), m_initial_texture(texture), m_damaged_textures(3), m_initial_health(health), m_current_health(health),
      m_is_bonus(is_bonus), ptr_to_magic_start(start_magic), ptr_to_magic_end(end_magic)
{
    m_body.setTexture(&m_initial_texture);
    m_body.setPosition(m_pos);

    if (!m_damaged_textures[0].loadFromFile("data/images/b_block_damaged_0.png") ||
        !m_damaged_textures[1].loadFromFile("data/images/b_block_damaged_1.png") ||
        !m_damaged_textures[2].loadFromFile("data/images/b_block_damaged_2.png"))
    {
        exit(1);
    }
}

sf::Vector2f Block::get_pos() const
{
    return m_body.getPosition();
}

sf::Vector2f Block::get_size() const
{
    return m_body.getSize();
}

bool Block::is_below(float x, float y) const
{
    return x >= m_body.getPosition().x &&
           x <= m_body.getPosition().x + m_body.getSize().x &&
           y >= m_body.getPosition().y + m_body.getSize().y;
}

bool Block::is_above(float x, float y) const
{
    return x >= m_body.getPosition().x &&
           x <= m_body.getPosition().x + m_body.getSize().x &&
           y <= m_body.getPosition().y;
}

bool Block::is_left(float x, float y) const
{
    return y >= m_body.getPosition().y &&
           y <= m_body.getPosition().y + m_body.getSize().y &&
           x <= m_body.getPosition().x;
}

bool Block::is_right(float x, float y) const
{
    return y >= m_body.getPosition().y &&
           y <= m_body.getPosition().y + m_body.getSize().y &&
           x >= m_body.getPosition().x + m_body.getSize().x;
}

bool Block::in_rect(float tx, float ty, float radius) const
{
    return tx >= m_body.getPosition().x - radius &&
           tx <= m_body.getPosition().x + m_body.getSize().x + radius &&
           ty <= m_body.getPosition().y + m_body.getSize().y + radius &&
           ty >= m_body.getPosition().y - radius;
}

bool Block::is_ruined() const
{
    return !m_current_health;
}

void Block::heal()
{
    m_current_health = m_initial_health;
    m_body.setTexture(&m_initial_texture);
}

void Block::reduce_health()
{
    if (--m_current_health > 0 && m_current_health < m_initial_health)
    {
        m_body.setTexture(&m_damaged_textures[Random::get(0, int(m_damaged_textures.size() - 1))]);
    }

    if (m_current_health == 0)
    {
        if (ptr_to_magic_start)
        {
            ptr_to_magic_start(m_game, reinterpret_cast<void *>(ptr_to_magic_end));
        }
    }
}

int Block::get_initial_health() const
{
    return m_initial_health;
}

void Block::set_health(int health)
{
    m_current_health = health;
}

void Block::set_texture(sf::Texture &texture)
{
    m_initial_texture = texture;
    m_body.setTexture(&m_initial_texture);
}

bool Block::is_bonus() const
{
    return m_is_bonus;
}

void Block::draw()
{
    m_window.draw(m_body);
}