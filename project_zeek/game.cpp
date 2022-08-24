#include "game.hpp"

#include <iostream>
#include <unordered_map>

Game::Game(sf::RenderWindow &window, sf::Font &font)
    : m_window(window), m_curr_level(0), m_player(*this),
      m_message_curr_level(m_text_level, font), m_message_points(m_text_points, font),
      m_message_game_state(m_text_game_won, font), m_message_game_name(m_text_game_name, font),
      m_play_button(window, window.getSize().y / 5, window.getSize().y / 10, m_play_button_text, font)
{
    if (!m_menu_background_texture.loadFromFile("data/images/menu_background.png") ||
        !m_game_background_texture.loadFromFile("data/images/game_background.png"))
    {
        std::cout << "background image is missing" << std::endl;
        exit(1);
    }

    m_play_button.set_pos(m_window.getSize().x / 2, m_window.getSize().y / 2 + 70);

    m_background.setTexture(m_menu_background_texture);
    m_background.setScale(m_window.getSize().x / m_menu_background_texture.getSize().x,
                          m_window.getSize().y / m_menu_background_texture.getSize().y);

    if (!load_levels())
    {
        std::cout << "levels are not uploaded" << std::endl;
        exit(1);
    }

    load_field();

    float size = m_window.getSize().x / 47;
    sf::Text::Style style = sf::Text::Style::Regular;
    sf::Color color = sf::Color::White;
    int thickness = 2;

    m_message_curr_level.set_properties(size, style, color, color, thickness);
    m_message_points.set_properties(size, style, color, color, thickness);
    m_message_game_state.set_properties(m_window.getSize().x / 20, style, color, color, thickness);
    m_message_game_name.set_properties(m_window.getSize().x / 20, style, color, color, thickness);
    m_message_game_name.set_pos(m_window.getSize().x / 2, m_window.getSize().y / 2 - 70);
}

/* ---------------PLAYER--------------- */

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
            m_game.m_level[row][col] = '.';
            m_game.m_objects[row][col].release();
            ++m_game.m_collected_bonuses;
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

bool Game::Player::get_caught() const
{
    return m_caught;
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

/* ---------------GAME_OBJECTS--------------- */

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

void Game::Hazard::set_dangerous(bool b)
{
    m_dangerous = b;
}

bool Game::Hazard::is_dangerous() const
{
    return m_dangerous;
}

void Game::Hazard::catch_player()
{
    if (!m_texture.loadFromFile("data/images/player_caught.png"))
    {
        exit(1);
    }

    m_body.setTexture(m_texture);
}

void Game::Hazard::draw()
{
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
    if (--m_explosion_counter > 0)
    {
        m_anim_index = 1;
    }
    else if (m_explosion_counter == 0)
    {
        m_anim_index = 2;
    }
    else if (m_explosion_counter < -5)
    {
        std::vector<int> row = {-1, 0, 1, 0, -1, 1, -1, 1};
        std::vector<int> col = {0, -1, 0, 1, -1, 1, 1, -1};

        for (size_t i = 0; i < row.size(); i++)
        {
            if (m_game.in_field(m_row + row[i], m_col + col[i]) &&
                m_game.m_level[m_row + row[i]][m_col + col[i]] != 'w')
            {
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

/* ---------------GAME--------------- */

bool Game::load_levels()
{
    std::fstream fs("data/levels.data");

    if (!fs)
    {
        return false;
    }

    for (;;)
    {
        std::string line;

        if (!std::getline(fs, line))
        {
            break;
        }

        m_titles.push_back(line);

        if (!std::getline(fs, line))
        {
            std::cout << "height is not found" << std::endl;
            return false;
        }

        int height;
        std::istringstream ss(line);

        if (!(ss >> height))
        {
            std::cout << "height is not an int" << std::endl;
            return false;
        }

        std::vector<std::string> level;

        for (int i = 0; i < height; i++)
        {
            if (!std::getline(fs, line))
            {
                std::cout << "level is not found" << std::endl;
                return false;
            }

            level.push_back(line);
        }

        m_levels.push_back(level);
    }

    return true;
}

void Game::load_field()
{
    m_level = m_levels[m_curr_level];
    m_total_bonuses = m_collected_bonuses = 0;

    float cell_size = std::min(m_window.getSize().x, m_window.getSize().y) / (m_level.size() + 2);
    sf::Vector2f block_size(cell_size, cell_size);
    m_player.set_size(cell_size);

    float x = m_window.getSize().x / 2 - ((m_level.front().size() / 2.0f) * cell_size);
    float y = m_window.getSize().y / 2 - ((m_level.size() / 2.0f) * cell_size);
    sf::Vector2f pos(x, y);

    m_boundaries.setFillColor(sf::Color::Transparent);
    m_boundaries.setOutlineColor(sf::Color::White);
    m_boundaries.setOutlineThickness(cell_size / 20);
    m_boundaries.setPosition(x, y);
    m_boundaries.setSize(sf::Vector2f(cell_size * m_level.front().size(), cell_size * m_level.size()));

    sf::Texture safe_cell, wall, bonus, fruit, ball, hazard, bomb, portal;

    if (!safe_cell.loadFromFile("data/images/safe_cell.png") ||
        !wall.loadFromFile("data/images/wall.png") ||
        !bonus.loadFromFile("data/images/bonus.png") ||
        !fruit.loadFromFile("data/images/fruit.png") ||
        !ball.loadFromFile("data/images/ball.png") ||
        !hazard.loadFromFile("data/images/droodle.png") ||
        !bomb.loadFromFile("data/images/bomb.png") ||
        !portal.loadFromFile("data/images/portal.png"))
    {
        std::cout << "file for an object is missing" << std::endl;
        exit(1);
    }

    std::unordered_map<char, sf::Texture> cell_types =
        {{'.', safe_cell}, {'w', wall}, {'b', bonus}, {'f', fruit}, {'e', ball}, {'h', hazard}, {'m', bomb}, {'*', portal}};

    m_cells.clear();
    m_objects.clear();
    m_objects.resize(m_level.size());

    for (size_t i = 0; i < m_level.size(); i++)
    {
        for (size_t j = 0; j < m_level[i].size(); j++)
        {
            char type = m_level[i][j];

            if (type == 'w')
            {
                m_cells.push_back(std::make_unique<Cell>(m_window, block_size, pos, cell_types[type]));
                m_objects[i].push_back(nullptr);
            }
            else
            {
                m_cells.push_back(std::make_unique<Cell>(m_window, block_size, pos, cell_types['.']));

                if (type == 'p')
                {
                    m_objects[i].push_back(std::make_unique<Portal>(*this, cell_types['*'], pos, i, j));
                    sf::Vector2f coords(pos.x + cell_size / 2, pos.y + cell_size / 2);
                    m_player.set_pos(coords, i, j);
                }
                else if (type == '.')
                {
                    m_objects[i].push_back(nullptr);
                }
                else if (type == 'b')
                {
                    m_objects[i].push_back(std::make_unique<Bonus>(*this, cell_types[type], pos, i, j));
                    ++m_total_bonuses;
                }
                else if (type == 'h')
                {
                    m_objects[i].push_back(std::make_unique<Hazard>(*this, cell_types[type], pos, i, j));
                }
                else if (type == 'f')
                {
                    m_objects[i].push_back(std::make_unique<Fruit>(*this, cell_types[type], pos, i, j));
                }
                else if (type == 'e')
                {
                    m_objects[i].push_back(std::make_unique<Ball>(*this, cell_types[type], pos, i, j));
                }
                else if (type == 'm')
                {
                    m_objects[i].push_back(std::make_unique<Bomb>(*this, cell_types[type], pos, i, j));
                }
                else if (type == '*')
                {
                    m_objects[i].push_back(std::make_unique<Portal>(*this, cell_types[type], pos, i, j));
                }
            }

            pos.x += cell_size;
        }

        pos.x = x;
        pos.y += cell_size;
    }
}

bool Game::in_field(int row, int col) const
{
    return row >= 0 && row < int(m_level.size()) &&
           col >= 0 && col < int(m_level.front().size());
}

void Game::update_messages()
{
    float size = m_cells.back()->get_size().x;

    std::string level = m_text_level + m_titles[m_curr_level];
    m_message_curr_level.set_str(level);
    m_message_curr_level.set_pos(size * 2, size / 1.5);

    std::string points = m_text_points + std::to_string(m_collected_bonuses);
    m_message_points.set_str(points);
    m_message_points.set_pos(m_window.getSize().x - size * 2, size / 1.5);

    if (m_state == State::GameWon)
    {
        m_message_game_state.set_str(m_text_game_won);
    }
    else if (m_state == State::GameLost)
    {
        m_message_game_state.set_str(m_text_game_lost);
    }

    m_message_game_state.set_pos(m_window.getSize().x / 2, m_window.getSize().y / 2);
}

void Game::update_game_state()
{
    if (m_state != State::Menu)
    {
        m_window.setMouseCursorVisible(false);

        if (m_collected_bonuses == m_total_bonuses &&
            m_level[m_player.get_coords().y][m_player.get_coords().x] == '*')
        {
            m_message_game_state.set_str(m_text_game_won);
            m_state = State::GameWon;
        }
        else
        {
            std::vector<int> row = {1, 0, -1, 0};
            std::vector<int> col = {0, -1, 0, 1};

            for (size_t i = 0; i < row.size(); i++)
            {
                int r = m_player.get_coords().y + row[i];
                int c = m_player.get_coords().x + col[i];

                if (in_field(r, c) && m_level[r][c] == 'h')
                {
                    auto object = dynamic_cast<Hazard *>(m_objects[r][c].get());

                    if (object->is_dangerous())
                    {
                        m_message_game_state.set_str(m_text_game_lost);
                        object->catch_player();
                        m_player.set_caught(true);
                        m_state = State::GameLost;
                    }
                }
            }
        }
    }
}

void Game::change_level()
{
    if (m_state == State::GameWon)
    {
        if (++m_curr_level == m_levels.size())
        {
            m_curr_level = 0;
        }
    }

    m_player.set_caught(false);
    load_field();
    m_state = State::GameStarted;
}

void Game::render_entities()
{
    m_window.draw(m_boundaries);

    for (auto &cell : m_cells)
    {
        cell->draw();
    }

    for (auto &objects : m_objects)
    {
        for (auto &object : objects)
        {
            if (object)
            {
                object->draw();
            }
        }
    }

    m_player.draw();

    m_message_curr_level.show_message(m_window);
    m_message_points.show_message(m_window);

    if (m_state == State::GameWon || m_state == State::GameLost)
    {
        m_message_game_state.show_message(m_window);
    }
}

void Game::run()
{
    while (m_window.isOpen())
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                m_window.close();
            }
            else if ((m_state == State::GameWon || m_state == State::GameLost) &&
                     sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                change_level();
            }
            else if (m_play_button.is_pressed())
            {
                m_state = State::GameStarted;
                m_background.setTexture(m_game_background_texture);
            }

            update_game_state();
            update_messages();
        }

        m_window.clear();

        m_window.draw(m_background);

        if (m_state == State::Menu)
        {
            m_message_game_name.show_message(m_window);
            m_play_button.draw();
        }
        else
        {
            render_entities();
        }

        m_window.display();
    }
}