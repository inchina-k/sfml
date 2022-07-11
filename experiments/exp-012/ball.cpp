#define _USE_MATH_DEFINES

#include "ball.hpp"
#include "../../libs/random.hpp"

#include <math.h>

using Random = effolkronium::random_static;

Ball::Ball(sf::RenderWindow &window, float x, float y)
    : m_window(window), m_body(m_window.getSize().y / 35), m_x(x), m_y(y)
{
    m_body.setFillColor(sf::Color::Yellow);
    m_body.setOrigin(m_body.getRadius(), m_body.getRadius());
    m_body.setPosition(m_x, m_y);

    set_random_speed();
}

void Ball::set_random_speed()
{
    float speed = std::min(m_window.getSize().x, m_window.getSize().y);
    float angle = Random::get(0.0, 2 * M_PI);

    m_speed.x = speed * cos(angle);
    m_speed.y = speed * sin(angle);
}

bool Ball::right_collision(Block &block, float &x_coord)
{
    if (m_x + m_body.getRadius() >= m_window.getSize().x)
    {
        x_coord = m_window.getSize().x;
        return true;
    }
    else if (m_x + m_body.getRadius() >= block.get_pos().x - block.get_size().x / 2 &&
             m_y >= block.get_pos().y - block.get_size().y / 2 &&
             m_y <= block.get_pos().y + block.get_size().y / 2)
    {
        x_coord = block.get_pos().x - block.get_size().x / 2;
        return true;
    }

    return false;
}

bool Ball::left_collision(Block &block, float &x_coord)
{
    if (m_x - m_body.getRadius() <= 0)
    {
        x_coord = 0;
        return true;
    }
    else if (m_x - m_body.getRadius() <= block.get_pos().x + block.get_size().x / 2 &&
             m_y >= block.get_pos().y - block.get_size().y / 2 &&
             m_y <= block.get_pos().y + block.get_size().y / 2)
    {
        x_coord = block.get_pos().x + block.get_size().x / 2;
        return true;
    }

    return false;
}

bool Ball::top_collision(Block &block, float &y_coord)
{
    if (m_y - m_body.getRadius() <= 0)
    {
        y_coord = 0;
        return true;
    }
    else if (m_y - m_body.getRadius() <= block.get_pos().y + block.get_size().y / 2 &&
             m_x >= block.get_pos().x - block.get_size().x / 2 &&
             m_x <= block.get_pos().x + block.get_size().x / 2)
    {
        y_coord = block.get_pos().y + block.get_size().y / 2;
        return true;
    }

    return false;
}

bool Ball::bottom_collision(Player &player, Block &block, float &y_coord)
{
    if (m_y + m_body.getRadius() >= m_window.getSize().y)
    {
        y_coord = m_window.getSize().y;
        return true;
    }
    else if (m_y + m_body.getRadius() >= player.get_pos().y - player.get_size().y / 2 &&
             m_x >= player.get_pos().x - player.get_size().x / 2 &&
             m_x <= player.get_pos().x + player.get_size().x / 2)
    {
        y_coord = player.get_pos().y - player.get_size().y / 2;
        return true;
    }
    else if (m_y + m_body.getRadius() >= block.get_pos().y - block.get_size().y / 2 &&
             m_x >= block.get_pos().x - block.get_size().x / 2 &&
             m_x <= block.get_pos().x + block.get_size().x / 2)
    {
        y_coord = block.get_pos().y - block.get_size().y / 2;
        return true;
    }

    return false;
}

void Ball::move(float time, Player &player, Block &block)
{
    float x_coord, y_coord;

    if (right_collision(block, x_coord))
    {
        float excess = m_body.getPosition().x - (x_coord - m_body.getRadius());
        m_x = x_coord - m_body.getRadius() - excess;
        m_speed.x = -m_speed.x;
    }
    else if (left_collision(block, x_coord))
    {
        float excess = m_body.getRadius() - (x_coord - m_body.getPosition().x);
        m_x = x_coord + m_body.getRadius() + excess;
        m_speed.x = -m_speed.x;
    }
    else if (bottom_collision(player, block, y_coord))
    {
        float excess = m_body.getPosition().y - (y_coord - m_body.getRadius());
        m_y = y_coord - m_body.getRadius() - excess;
        m_speed.y = -m_speed.y;
    }
    else if (top_collision(block, y_coord))
    {
        float excess = m_body.getRadius() - (y_coord - m_body.getPosition().y);
        m_y = y_coord + m_body.getRadius() + excess;
        m_speed.y = -m_speed.y;
    }

    m_x += m_speed.x * time;
    m_y += m_speed.y * time;

    m_body.setPosition(m_x, m_y);
}

void Ball::draw()
{
    m_window.draw(m_body);
}