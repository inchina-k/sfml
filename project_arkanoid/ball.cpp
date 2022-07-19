#include "ball.hpp"
#include "../libs/random.hpp"

using Random = effolkronium::random_static;

Ball::Ball(sf::RenderWindow &window, float x, float y)
    : m_window(window), m_body(m_window.getSize().y / 45), m_x(x), m_y(y), m_missed(false), m_lost(false), m_lives(3), m_lives_remained(3)
{
    m_body.setFillColor(sf::Color::Yellow);
    m_body.setOrigin(m_body.getRadius(), m_body.getRadius());
    m_body.setPosition(m_x, m_y);

    set_speed();
}

void Ball::set_speed()
{
    float speed = std::min(m_window.getSize().x, m_window.getSize().y) / 1.5;
    float angle = 5;

    m_speed.x = speed * cos(angle);
    m_speed.y = speed * sin(angle);
}

float Ball::get_radius() const
{
    return m_body.getRadius();
}

void Ball::move(float time, Player &player, std::vector<std::unique_ptr<Block>> &blocks)
{
    sf::Vector2f curr_pos = m_body.getPosition();
    sf::Vector2f new_pos = curr_pos + m_speed * time;

    if (m_x + m_body.getRadius() >= m_window.getSize().x)
    {
        float excess = m_body.getPosition().x - (m_window.getSize().x - m_body.getRadius());
        m_x = m_window.getSize().x - m_body.getRadius() - excess;
        m_speed.x = -m_speed.x;
    }
    else if (m_x - m_body.getRadius() <= 0)
    {
        float excess = m_body.getRadius() - m_body.getPosition().x;
        m_x = m_body.getRadius() + excess;
        m_speed.x = -m_speed.x;
    }
    else if (m_y + m_body.getRadius() >= m_window.getSize().y + 300)
    {
        if (!m_missed)
        {
            if (--m_lives_remained == 0)
            {
                m_lost = true;
            }
        }

        m_missed = true;
    }
    else if (m_y - m_body.getRadius() <= 0)
    {
        float excess = m_body.getRadius() - m_body.getPosition().y;
        m_y = m_body.getRadius() + excess;
        m_speed.y = -m_speed.y;
    }
    else if (player.is_above(curr_pos.x, curr_pos.y) && player.in_rect(new_pos.x, new_pos.y, m_body.getRadius()))
    {
        float excess = player.get_pos().y - player.get_size().y / 2 - m_body.getRadius() - new_pos.y;
        m_y = player.get_pos().y - player.get_size().y / 2 - excess - m_body.getRadius();
        m_speed.y = -m_speed.y;
    }
    else if (player.in_rect(new_pos.x, new_pos.y, m_body.getRadius()))
    {
        m_speed.x = -m_speed.x + Random::get(-1.5, 1.5);
        m_speed.y = -m_speed.y + Random::get(-1.5, 1.5);
    }
    else
    {
        for (auto &block : blocks)
        {
            if (!block->is_ruined())
            {
                if (block->is_below(curr_pos.x, curr_pos.y) && block->in_rect(new_pos.x, new_pos.y, m_body.getRadius()))
                {
                    float excess = block->get_pos().y + block->get_size().y + m_body.getRadius() - new_pos.y;
                    m_y = block->get_pos().y + block->get_size().y + excess + m_body.getRadius();
                    m_speed.y = -m_speed.y;

                    block->reduce_health();
                }
                else if (block->is_above(curr_pos.x, curr_pos.y) && block->in_rect(new_pos.x, new_pos.y, m_body.getRadius()))
                {
                    float excess = block->get_pos().y - m_body.getRadius() - new_pos.y;
                    m_y = block->get_pos().y - excess - m_body.getRadius();
                    m_speed.y = -m_speed.y;

                    block->reduce_health();
                }
                else if (block->is_left(curr_pos.x, curr_pos.y) && block->in_rect(new_pos.x, new_pos.y, m_body.getRadius()))
                {
                    float excess = block->get_pos().x - m_body.getRadius() - new_pos.x;
                    m_x = block->get_pos().x - excess - m_body.getRadius();
                    m_speed.x = -m_speed.x;

                    block->reduce_health();
                }
                else if (block->is_right(curr_pos.x, curr_pos.y) && block->in_rect(new_pos.x, new_pos.y, m_body.getRadius()))
                {
                    float excess = block->get_pos().x + block->get_size().x + m_body.getRadius() - new_pos.x;
                    m_x = block->get_pos().x + block->get_size().x + excess + m_body.getRadius();
                    m_speed.x = -m_speed.x;

                    block->reduce_health();
                }
                // else if (block->in_rect(new_pos.x, new_pos.y, m_body.getRadius()))
                // {
                //     m_speed.x = -m_speed.x + Random::get(-1.5, 1.5);
                //     m_speed.y = -m_speed.y + Random::get(-1.5, 1.5);
                // }
            }
        }
    }

    m_x += m_speed.x * time;
    m_y += m_speed.y * time;

    m_body.setPosition(m_x, m_y);
}

void Ball::set_pos(float x, float y)
{
    m_x = x;
    m_y = y;

    m_body.setPosition(m_x, m_y);
}

sf::Vector2f Ball::get_pos() const
{
    return m_body.getPosition();
}

bool Ball::is_missed() const
{
    return m_missed;
}

void Ball::set_missed(bool state)
{
    m_missed = state;
}

bool Ball::is_lost()
{
    return m_lost;
}

void Ball::set_lost(bool state)
{
    m_lost = state;
}

void Ball::resume_lives()
{
    m_lives_remained = m_lives;
}

int Ball::get_remained_lives()
{
    return m_lives_remained;
}

void Ball::draw()
{
    m_window.draw(m_body);
}