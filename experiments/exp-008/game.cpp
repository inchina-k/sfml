#include "game.hpp"

Game::Game(int size, int width, int height)
    : m_frames_per_sec(sf::seconds(0.2f)), m_total_time(sf::Time::Zero), m_field_size(size), m_width(width), m_height(height), m_cell_size(min(m_width, m_height) / (2 * m_field_size)), m_cell(sf::Vector2f(m_cell_size, m_cell_size)), m_dir(Dirs::None), m_col(0), m_row(0)
{
    m_rect_x = m_width / 2 - m_cell_size * (m_field_size / 2);
    m_rect_y = m_height / 2 - m_cell_size * (m_field_size / 2);

    m_cell.setFillColor(sf::Color(189, 203, 219));
    m_cell.setOutlineThickness(1);
    m_cell.setOutlineColor(sf::Color(225, 237, 252));

    m_circle_x = m_width / 2 - m_cell_size * (m_field_size / 2);
    m_circle_y = m_height / 2 - m_cell_size * (m_field_size / 2);

    circle.setRadius(m_cell_size / 2);
    circle.setFillColor(sf::Color::Blue);
    circle.setPosition(m_circle_x, m_circle_y);
}

void Game::process_events()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && m_dir == Dirs::None)
    {
        m_dir = Dirs::Right;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && m_dir == Dirs::None)
    {
        m_dir = Dirs::Down;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && m_dir == Dirs::None)
    {
        m_dir = Dirs::Left;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && m_dir == Dirs::None)
    {
        m_dir = Dirs::Up;
    }
}

void Game::update()
{
    m_total_time += m_clock.restart();

    if (m_total_time > m_frames_per_sec)
    {
        m_total_time -= m_frames_per_sec;

        if (m_dir == Dirs::Right)
        {
            m_circle_x += m_cell_size;
            ++m_col;

            if (m_col == m_field_size)
            {
                m_dir = Dirs::None;

                m_col = m_field_size - 1;
                m_circle_x -= m_cell_size;
            }
        }
        else if (m_dir == Dirs::Down)
        {
            m_circle_y += m_cell_size;
            ++m_row;

            if (m_row == m_field_size)
            {
                m_dir = Dirs::None;

                m_row = m_field_size - 1;
                m_circle_y -= m_cell_size;
            }
        }
        else if (m_dir == Dirs::Left)
        {
            m_circle_x -= m_cell_size;
            --m_col;

            if (m_col == -1)
            {
                m_dir = Dirs::None;

                m_col = 0;
                m_circle_x += m_cell_size;
            }
        }
        else if (m_dir == Dirs::Up)
        {
            m_circle_y -= m_cell_size;
            --m_row;

            if (m_row == -1)
            {
                m_dir = Dirs::None;

                m_row = 0;
                m_circle_y += m_cell_size;
            }
        }
    }
}

void Game::draw(sf::RenderWindow &window)
{
    sf::Vector2f pos(m_rect_x, m_rect_y);

    for (int i = 0; i < m_field_size; i++)
    {
        for (int j = 0; j < m_field_size; j++)
        {
            m_cell.setPosition(pos.x, pos.y);
            window.draw(m_cell);

            pos.x += m_cell.getSize().x;
        }

        pos.x = m_rect_x;
        pos.y += m_cell.getSize().x;
    }

    circle.setPosition(m_circle_x, m_circle_y);
    window.draw(circle);
}

void Game::run(sf::RenderWindow &window)
{
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }

            process_events();
        }

        update();

        window.clear();

        draw(window);

        window.display();
    }
}