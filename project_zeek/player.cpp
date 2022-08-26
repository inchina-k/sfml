#include "game.hpp"

Game::Player::Player(Game &game)
    : m_game(game), m_counter(0), m_frame_index(0), m_anim_index(0), m_size(0),
      m_row(0), m_col(0), m_num_of_steps(m_max_counter * 4), m_caught(false)
{
    if (!m_texture.loadFromFile("data/images/doodly.png"))
    {
        std::cout << "file for the player is missing" << std::endl;
        exit(1);
    }

    m_w = m_texture.getSize().x / 3;
    m_h = m_texture.getSize().y / 4;

    load();
}

void Game::Player::load()
{
    m_frames.resize(4);

    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            m_frames[i].push_back(std::make_unique<sf::Sprite>(m_texture, sf::IntRect(j * m_w, i * m_h, m_w, m_h)));
        }
    }
}

void Game::Player::set_pos(sf::Vector2f &pos, int row, int col)
{
    m_pos = pos;
    m_row = row;
    m_col = col;
}

sf::Vector2f Game::Player::get_pos() const
{
    return m_pos;
}

sf::Vector2i Game::Player::get_coords() const
{
    return sf::Vector2i(m_col, m_row);
}

void Game::Player::set_size(float size)
{
    m_size = size;

    for (auto &frames : m_frames)
    {
        for (auto &frame : frames)
        {
            frame->setScale(m_size / frame->getLocalBounds().width, m_size / frame->getLocalBounds().height);
            frame->setOrigin(frame->getLocalBounds().width / 2, frame->getLocalBounds().height / 2);
        }
    }
}

sf::Vector2f Game::Player::get_size() const
{
    sf::Vector2f size(m_frames[m_anim_index][m_frame_index]->getGlobalBounds().width,
                      m_frames[m_anim_index][m_frame_index]->getGlobalBounds().height);
    return size;
}

bool Game::Player::can_move(int dr, int dc)
{
    int row = m_row + dr;
    int col = m_col + dc;

    if (m_game.in_field(row, col))
    {
        if (m_game.m_level[row][col] == '.' ||
            m_game.m_level[row][col] == 'p' ||
            m_game.m_level[row][col] == '*')
        {
            return true;
        }
        else if (m_game.m_level[row][col] == 'b')
        {
            m_game.m_bonus_sound.play();
            m_game.m_level[row][col] = '.';
            m_game.m_objects[row][col].release();
            ++m_game.m_collected_bonuses;
            return true;
        }
        else if (m_game.m_level[row][col] == 'k')
        {
            ++m_num_of_keys;
            m_game.m_level[row][col] = '.';
            m_game.m_objects[row][col].release();
            return true;
        }
        else if (m_game.m_level[row][col] == 'g')
        {
            auto object = dynamic_cast<Gates *>(m_game.m_objects[row][col].get());

            if (has_keys())
            {
                object->set_opened(true);
                --m_num_of_keys;
            }
        }
        else if (m_game.m_level[row][col] == 'h')
        {
            return false;
        }
        else if (m_game.m_level[row][col] != 'w' &&
                 m_game.in_field(row + dr, col + dc) && m_game.m_level[row + dr][col + dc] == '.')
        {
            if (auto object = dynamic_cast<Bomb *>(m_game.m_objects[row][col].get()))
            {
                object->set_deployed(true);
            }
            else if (auto object_1 = dynamic_cast<Crystal *>(m_game.m_objects[row][col].get()))
            {
                object_1->set_activated(true);
            }

            m_game.m_objects[row][col]->set_dir(dr, dc);
            std::swap(m_game.m_level[row][col], m_game.m_level[row + dr][col + dc]);
            std::swap(m_game.m_objects[row][col], m_game.m_objects[row + dr][col + dc]);
            return true;
        }
    }

    return false;
}

void Game::Player::switch_command()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
    {
        m_anim_index = 0;

        if (can_move(1, 0))
        {
            m_curr_state = State::Go;
            m_dir.x = 0;
            m_dir.y = m_step;
            ++m_row;
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
    {
        m_anim_index = 1;

        if (can_move(-1, 0))
        {
            m_curr_state = State::Go;
            m_dir.x = 0;
            m_dir.y = -m_step;
            --m_row;
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        m_anim_index = 2;

        if (can_move(0, -1))
        {
            m_curr_state = State::Go;
            m_dir.x = -m_step;
            m_dir.y = 0;
            --m_col;
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        m_anim_index = 3;

        if (can_move(0, 1))
        {
            m_curr_state = State::Go;
            m_dir.x = m_step;
            m_dir.y = 0;
            ++m_col;
        }
    }
    else
    {
        m_frame_index = 0;
    }
}

void Game::Player::move()
{
    if (m_curr_state == State::Go)
    {
        m_pos += m_dir;
        --m_num_of_steps;

        if (m_num_of_steps == 0)
        {
            m_curr_state = State::Stand;
        }
    }
    else
    {
        m_num_of_steps = m_max_counter * 3;
        m_step = get_size().x / m_num_of_steps;

        switch_command();
    }
}

void Game::Player::set_caught(bool b)
{
    m_caught = b;
}

bool Game::Player::is_caught() const
{
    return m_caught;
}

void Game::Player::restore_keys()
{
    m_num_of_keys = 0;
}

bool Game::Player::has_keys() const
{
    return m_num_of_keys;
}

void Game::Player::draw()
{
    move();

    if (!m_caught)
    {
        m_frames[m_anim_index][m_frame_index]->setPosition(m_pos);
        m_game.m_window.draw(*m_frames[m_anim_index][m_frame_index]);
    }

    if (++m_counter == m_max_counter)
    {
        if (++m_frame_index == 3)
        {
            m_frame_index = 1;
        }

        m_counter = 0;
    }
}