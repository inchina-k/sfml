#include "game.hpp"
#include "../libs/random.hpp"

using Random = effolkronium::random_static;

Game::Enemy::Enemy(Game &game, sf::Texture &texture, sf::Vector2f &size, sf::Vector2f &pos, int row, int col)
    : GameObject(game, texture, size, pos, row, col), m_dir(1, 0)
{
    m_w = m_texture.getSize().x / 5;
    m_h = m_texture.getSize().y / 2;

    load(size);

    m_row = row;
    m_col = col;

    m_frame_index = Random::get(0, 2);
    m_step = m_game.m_walls.front().front()->get_size().x / 50;

    set_active(true);
}

void Game::Enemy::load(sf::Vector2f &size)
{
    m_frames.resize(2);

    for (size_t i = 0; i < 2; i++)
    {
        for (size_t j = 0; j < 5; j++)
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

sf::Vector2f Game::Enemy::get_size() const
{
    sf::Vector2f size(m_frames[m_anim_index][m_frame_index]->getGlobalBounds().width,
                      m_frames[m_anim_index][m_frame_index]->getGlobalBounds().height);
    return size;
}

sf::FloatRect Game::Enemy::get_bounds() const
{
    return m_frames[m_anim_index][m_frame_index]->getGlobalBounds();
}

bool Game::Enemy::should_be_released() const
{
    return m_should_be_released;
}

void Game::Enemy::set_dir()
{
    std::vector<sf::Vector2i> m_dirs;

    std::vector<int> d_row = {1, 0, 0, -1};
    std::vector<int> d_col = {0, 1, -1, 0};

    for (size_t i = 0; i < d_row.size(); i++)
    {
        if (m_game.m_cells[m_row + d_row[i]][m_col + d_col[i]])
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

void Game::Enemy::check_collision_dir()
{
    if (m_dir.y == 1)
    {
        m_pos.x = get_bounds().left;
        m_pos.y = m_game.m_walls[m_row][m_col]->get_bounds().top - get_bounds().height;
        return;
    }
    else if (m_dir.y == -1)
    {
        m_pos.x = get_bounds().left;
        m_pos.y = m_game.m_walls[m_row][m_col]->get_bounds().top + m_game.m_walls[m_row][m_col]->get_bounds().height;
        return;
    }
    else if (m_dir.x == -1)
    {
        m_pos.x = m_game.m_walls[m_row][m_col]->get_bounds().left + m_game.m_walls[m_row][m_col]->get_bounds().width;
        m_pos.y = get_bounds().top;
        return;
    }
    else if (m_dir.x == 1)
    {
        m_pos.x = m_game.m_walls[m_row][m_col]->get_bounds().left - get_bounds().width;
        m_pos.y = get_bounds().top;
        return;
    }
}

void Game::Enemy::move()
{
    m_pos.x += m_dir.x * m_step;
    m_pos.y += m_dir.y * m_step;

    m_row = m_pos.y / m_game.m_walls.front().front()->get_size().y;
    m_col = m_pos.x / m_game.m_walls.front().front()->get_size().x;

    if (m_dir.y == 1)
    {
        m_row = (m_pos.y + get_size().y) / m_game.m_walls.front().front()->get_size().y;
    }
    else if (m_dir.x == 1)
    {
        m_col = (m_pos.x + get_size().x) / m_game.m_walls.front().front()->get_size().x;
    }

    if (m_game.m_walls[m_row][m_col])
    {
        check_collision_dir();

        m_row = m_pos.y / m_game.m_walls.front().front()->get_size().y;
        m_col = m_pos.x / m_game.m_walls.front().front()->get_size().x;

        set_dir();
    }
}

void Game::Enemy::draw()
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
        if (++m_frame_index == 5)
        {
            if (m_anim_index == 1)
            {
                m_should_be_released = true;
            }

            m_frame_index = 0;
        }

        m_counter = 0;
    }
}