#include "game.hpp"

Game::GameObject::GameObject(Game &game, sf::Texture &texture, sf::Vector2f &pos, int row, int col)
    : m_game(game), m_texture(texture), m_pos(pos), m_row(row), m_col(col),
      m_num_of_steps(m_max_counter * 3)
{
    m_body.setTexture(m_texture);
    m_body.setScale(game.m_cells.front()->get_size().x / m_texture.getSize().x,
                    game.m_cells.front()->get_size().y / m_texture.getSize().y);
    m_body.setPosition(m_pos);
}

void Game::GameObject::set_dir(int dr, int dc)
{
    m_curr_state = State::Go;
    m_dir.x = m_step * dc;
    m_dir.y = m_step * dr;
    m_row += dr;
    m_col += dc;
}

void Game::GameObject::move()
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
        m_step = m_body.getGlobalBounds().width / m_num_of_steps;
    }
}

Game::Bonus::Bonus(Game &game, sf::Texture &texture, sf::Vector2f &pos, int row, int col)
    : GameObject(game, texture, pos, row, col)
{
}

void Game::Bonus::draw()
{
    m_game.m_window.draw(m_body);
}

Game::Hazard::Hazard(Game &game, sf::Texture &texture, sf::Vector2f &pos, int row, int col)
    : GameObject(game, texture, pos, row, col), m_dangerous(true)
{
}

void Game::Hazard::raise_hands(int dir_row, int dir_col)
{
    if (dir_row == -1)
    {
        if (!m_texture.loadFromFile("data/images/droodle_up.png"))
            exit(1);
    }
    else if (dir_row == 1)
    {
        if (!m_texture.loadFromFile("data/images/droodle_down.png"))
            exit(1);
    }
    else if (dir_col == -1)
    {
        if (!m_texture.loadFromFile("data/images/droodle_left.png"))
            exit(1);
    }
    else if (dir_col == 1)
    {
        if (!m_texture.loadFromFile("data/images/droodle_right.png"))
            exit(1);
    }

    m_body.setTexture(m_texture);
}

void Game::Hazard::set_dangerous(bool b)
{
    m_dangerous = b;
}

bool Game::Hazard::is_dangerous() const
{
    return m_dangerous;
}

void Game::Hazard::catch_fruit(int row, int col, int dir_row, int dir_col)
{
    if (++m_counter < m_max_counter * 3)
    {
        raise_hands(dir_row, dir_col);
    }
    else if (m_counter == m_max_counter * 3)
    {
        if (!m_texture.loadFromFile("data/images/fruit_caught.png"))
        {
            exit(1);
        }

        m_game.m_level[row][col] = '.';
        m_game.m_objects[row][col].release();

        m_body.setTexture(m_texture);

        set_dangerous(false);
        m_game.m_chewing_sound.play();
        m_counter = 0;
    }
}

void Game::Hazard::catch_player(int dir_row, int dir_col)
{
    if (is_dangerous())
    {
        if (++m_counter < m_max_counter * 3)
        {
            raise_hands(dir_row, dir_col);
        }
        else if (m_counter == m_max_counter * 3)
        {
            if (!m_texture.loadFromFile("data/images/player_caught.png"))
            {
                exit(1);
            }

            m_body.setTexture(m_texture);
            set_dangerous(false);
            m_game.m_player.set_caught(true);
            m_counter = 0;
        }
    }
}

void Game::Hazard::draw()
{
    std::vector<int> row = {-1, 0, 1, 0};
    std::vector<int> col = {0, -1, 0, 1};

    for (size_t i = 0; i < row.size(); i++)
    {
        if (m_game.in_field(m_row + row[i], m_col + col[i]) &&
            m_game.m_level[m_row + row[i]][m_col + col[i]] == 'f')
        {
            catch_fruit(m_row + row[i], m_col + col[i], row[i], col[i]);
        }
        else if (m_game.in_field(m_row + row[i], m_col + col[i]) &&
                 m_game.m_player.get_coords().y == m_row + row[i] &&
                 m_game.m_player.get_coords().x == m_col + col[i])
        {
            catch_player(row[i], col[i]);
        }
    }

    if (!is_dangerous() && !m_game.m_player.is_caught())
    {
        if (--m_neutralized_counter > 0 && m_neutralized_counter < 5)
        {
            if (!m_texture.loadFromFile("data/images/droodle_swallowing.png"))
                exit(1);

            m_body.setTexture(m_texture);
        }
        else if (m_neutralized_counter == 0)
        {
            m_game.m_swallow_sound.play();

            if (!m_texture.loadFromFile("data/images/droodle.png"))
                exit(1);

            m_body.setTexture(m_texture);
            m_neutralized_counter = 500;
            set_dangerous(true);
        }
    }

    m_game.m_window.draw(m_body);
}

Game::Fruit::Fruit(Game &game, sf::Texture &texture, sf::Vector2f &pos, int row, int col)
    : GameObject(game, texture, pos, row, col)

{
}

void Game::Fruit::draw()
{
    move();
    m_body.setPosition(m_pos);
    m_game.m_window.draw(m_body);
}

Game::Ball::Ball(Game &game, sf::Texture &texture, sf::Vector2f &pos, int row, int col)
    : GameObject(game, texture, pos, row, col)
{
}

void Game::Ball::draw()
{
    move();

    m_body.setPosition(m_pos);
    m_game.m_window.draw(m_body);
}

Game::Portal::Portal(Game &game, sf::Texture &texture, sf::Vector2f &pos, int row, int col)
    : GameObject(game, texture, pos, row, col)
{
}

void Game::Portal::draw()
{
    m_game.m_window.draw(m_body);
}

Game::Bomb::Bomb(Game &game, sf::Texture &texture, sf::Vector2f &pos, int row, int col)
    : GameObject(game, texture, pos, row, col), m_deployed(false)
{
    m_w = m_texture.getSize().x / 2;
    m_h = m_texture.getSize().y / 3;

    load();
}

void Game::Bomb::load()
{
    m_frames.resize(3);

    for (size_t i = 0; i < 3; i++)
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
            float cell_size = m_game.m_cells.front()->get_size().x;
            frame->setScale(cell_size / frame->getLocalBounds().width, cell_size / frame->getLocalBounds().height);
        }
    }
}

void Game::Bomb::set_deployed(bool b)
{
    m_deployed = b;
}

bool Game::Bomb::is_deployed() const
{
    return m_deployed;
}

void Game::Bomb::explode()
{
    if (m_play_sound)
    {
        m_game.m_bomb_deployed_sound.play();
        m_play_sound = false;
    }

    if (--m_explosion_counter > 0)
    {
        m_anim_index = 1;
    }
    else if (m_explosion_counter == 0)
    {
        m_anim_index = 2;

        m_game.m_bomb_deployed_sound.stop();
        m_game.m_bomb_exploded_sound.play();
    }
    else if (m_explosion_counter < -5)
    {
        std::vector<int> row = {-1, 0, 1, 0, -1, 1, -1, 1};
        std::vector<int> col = {0, -1, 0, 1, -1, 1, 1, -1};

        for (size_t i = 0; i < row.size(); i++)
        {
            if (m_game.in_field(m_row + row[i], m_col + col[i]) &&
                m_game.m_level[m_row + row[i]][m_col + col[i]] != 'w' &&
                m_game.m_level[m_row + row[i]][m_col + col[i]] != 'g')
            {
                if (m_game.m_level[m_row + row[i]][m_col + col[i]] == 'b')
                {
                    m_game.m_state = Game::State::GameLost;
                }
                else if (m_game.m_player.get_coords().x == m_col + col[i] &&
                         m_game.m_player.get_coords().y == m_row + row[i])
                {
                    m_game.m_player.set_caught(true);
                    m_game.m_state = Game::State::GameLost;
                }

                m_game.m_level[m_row + row[i]][m_col + col[i]] = '.';
                m_game.m_objects[m_row + row[i]][m_col + col[i]].release();
            }
        }

        m_game.m_level[m_row][m_col] = '.';
        m_game.m_objects[m_row][m_col].release();
    }
}

void Game::Bomb::draw()
{
    move();

    m_frames[m_anim_index][m_frame_index]->setPosition(m_pos);
    m_game.m_window.draw(*m_frames[m_anim_index][m_frame_index]);

    if (m_curr_state == State::Stand && is_deployed())
    {
        explode();
    }

    if (++m_counter == m_max_counter)
    {
        if (++m_frame_index == 2)
        {
            m_frame_index = 0;
        }

        m_counter = 0;
    }
}

Game::Crystal::Crystal(Game &game, sf::Texture &texture, sf::Vector2f &pos, int row, int col)
    : GameObject(game, texture, pos, row, col), m_activated(false)
{
    m_w = m_texture.getSize().x / 2;
    m_h = m_texture.getSize().y / 3;

    load();
}

void Game::Crystal::load()
{
    m_frames.resize(3);

    for (size_t i = 0; i < 3; i++)
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
            float cell_size = m_game.m_cells.front()->get_size().x;
            frame->setScale(cell_size / frame->getLocalBounds().width, cell_size / frame->getLocalBounds().height);
        }
    }
}

void Game::Crystal::set_activated(bool b)
{
    m_activated = b;
}

bool Game::Crystal::is_activated() const
{
    return m_activated;
}

void Game::Crystal::explode()
{
    if (m_play_sound)
    {
        m_game.m_crystal_activated_sound.play();
        m_play_sound = false;
    }

    if (--m_explosion_counter > 0)
    {
        m_anim_index = 1;
    }
    else if (m_explosion_counter == 0)
    {
        m_anim_index = 2;

        m_game.m_crystal_activated_sound.stop();
        m_game.m_crystal_exploded_sound.play();
    }
    else if (m_explosion_counter < -5)
    {
        m_game.m_level[m_row][m_col] = '.';
        m_game.m_objects[m_row][m_col].release();
    }
}

void Game::Crystal::draw()
{
    move();

    m_frames[m_anim_index][m_frame_index]->setPosition(m_pos);
    m_game.m_window.draw(*m_frames[m_anim_index][m_frame_index]);

    if (m_curr_state == State::Stand && is_activated())
    {
        std::vector<int> row = {-1, 0, 1, 0};
        std::vector<int> col = {0, -1, 0, 1};

        for (size_t i = 0; i < row.size(); i++)
        {
            if (m_game.in_field(m_row + row[i], m_col + col[i]) &&
                m_game.m_level[m_row + row[i]][m_col + col[i]] == 'c')
            {
                auto object = dynamic_cast<Crystal *>(m_game.m_objects[m_row + row[i]][m_col + col[i]].get());
                object->explode();
                explode();
            }
        }
    }

    if (++m_counter == m_max_counter)
    {
        if (++m_frame_index == 2)
        {
            m_frame_index = 0;
        }

        m_counter = 0;
    }
}

Game::Gates::Gates(Game &game, sf::Texture &texture, sf::Vector2f &pos, int row, int col)
    : GameObject(game, texture, pos, row, col), m_opened(false)
{
    m_w = m_texture.getSize().x / 2;
    m_h = m_texture.getSize().y / 3;

    load();
}

void Game::Gates::load()
{
    m_frames.resize(3);

    for (size_t i = 0; i < 3; i++)
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
            float cell_size = m_game.m_cells.front()->get_size().x;
            frame->setScale(cell_size / frame->getLocalBounds().width, cell_size / frame->getLocalBounds().height);
        }
    }
}

void Game::Gates::set_opened(bool b)
{
    m_opened = b;
}

bool Game::Gates::is_opened() const
{
    return m_opened;
}

void Game::Gates::open()
{
    if (--m_open_counter > 0)
    {
        m_anim_index = 1;
    }
    else if (m_open_counter == 0)
    {
        m_anim_index = 2;
    }
    else if (m_open_counter < -5)
    {
        m_game.m_level[m_row][m_col] = '.';
        m_game.m_objects[m_row][m_col].release();
    }
}

void Game::Gates::draw()
{
    m_frames[m_anim_index][m_frame_index]->setPosition(m_pos);
    m_game.m_window.draw(*m_frames[m_anim_index][m_frame_index]);

    if (is_opened())
    {
        open();
    }

    if (++m_counter == m_max_counter)
    {
        if (++m_frame_index == 2)
        {
            m_frame_index = 0;
        }

        m_counter = 0;
    }
}

Game::Key::Key(Game &game, sf::Texture &texture, sf::Vector2f &pos, int row, int col)
    : GameObject(game, texture, pos, row, col)
{
}

void Game::Key::draw()
{
    m_game.m_window.draw(m_body);
}
