#include "game.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

Game::Game(sf::RenderWindow &window, sf::Font &font)
    : m_window(window), m_curr_level(0), m_player(*this),
      m_message_level(m_text_level, font), m_message_lives(m_text_lives, font),
      m_message_game_state(m_text_game_won, font),
      m_play_button(window, window.getSize().y / 5, window.getSize().y / 10, m_play_button_text, font)
{
    if (!load_levels())
    {
        std::cout << "levels are not uploaded" << std::endl;
        exit(1);
    }

    load_background();
    load_field();
    load_views();
    load_messages();
}

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

void Game::load_background()
{
    if (!m_menu_texture.loadFromFile("data/images/menu.png") ||
        !m_game_texture.loadFromFile("data/images/bg.png"))
    {
        exit(1);
    }

    m_menu_background.setTexture(m_menu_texture);
    m_menu_background.setScale(m_window.getSize().x / m_menu_background.getTexture()->getSize().x,
                               m_window.getSize().y / m_menu_background.getTexture()->getSize().y);

    m_game_background.setTexture(m_game_texture);
    m_game_background.setScale(m_window.getSize().x / m_game_background.getTexture()->getSize().x,
                               m_window.getSize().y / m_game_background.getTexture()->getSize().y);

    m_play_button.set_pos(m_window.getSize().x / 2 + 150, m_window.getSize().y / 2);
}

void Game::load_field()
{
    m_level = m_levels[m_curr_level];

    sf::Vector2f pos(0, 0);

    if (!m_texture_safe_cell.loadFromFile("data/images/safe_cell.png") ||
        !m_texture_wall.loadFromFile("data/images/wall.png") ||
        !m_texture_enemy.loadFromFile("data/images/enemy.png") ||
        !m_texture_bomb.loadFromFile("data/images/bomb.png") ||
        !m_texture_explosion.loadFromFile("data/images/explosion.png"))
    {
        std::cout << "file for an object is missing" << std::endl;
        exit(1);
    }

    std::unordered_map<char, sf::Texture> cell_types =
        {{'.', m_texture_safe_cell}, {'w', m_texture_wall}, {'e', m_texture_enemy}};

    m_walls.clear();
    m_walls.resize(m_level.size());
    m_cells.clear();
    m_cells.resize(m_level.size());
    m_enemies.clear();
    m_explosions.clear();
    m_explosions.resize(m_level.size());

    float cell_size = std::min(m_window.getSize().x, m_window.getSize().y) / (m_level.size() / 1.7f);
    sf::Vector2f block_size(cell_size, cell_size);

    for (size_t i = 0; i < m_level.size(); i++)
    {
        for (size_t j = 0; j < m_level[i].size(); j++)
        {
            char type = m_level[i][j];

            if (type == 'w')
            {
                m_walls[i].push_back(std::make_unique<Wall>(*this, cell_types[type], block_size, pos, i, j));
                m_cells[i].push_back(nullptr);
            }
            else
            {
                m_cells[i].push_back(std::make_unique<SafeCell>(*this, cell_types['.'], block_size, pos, i, j));
                m_walls[i].push_back(nullptr);

                if (type == 'p')
                {
                    m_player.set_size(block_size);
                    m_player.set_pos(pos, i, j);
                }
                else if (type == 'e')
                {
                    m_enemies.push_back(std::make_unique<Enemy>(*this, cell_types[type], block_size, pos, i, j));
                }
            }

            m_explosions[i].push_back(nullptr);

            pos.x += cell_size;
        }

        pos.x = 0;
        pos.y += cell_size;
    }

    m_total_enemies = m_enemies.size();

    for (auto &enemy : m_enemies)
    {
        if (enemy)
        {
            enemy->set_dir();
        }
    }
}

void Game::load_views()
{
    m_main_view.setSize(m_window.getSize().x, m_window.getSize().y);
    m_main_view.setCenter(m_player.get_pos() + m_player.get_size() / 2.f);

    m_map_view.setSize(m_walls.back().back()->get_size().x * m_level.front().size(), m_walls.back().back()->get_size().y * m_level.size());
    m_map_view.setCenter(m_walls.back().back()->get_pos() / 2.f + m_walls.back().back()->get_size() / 2.f);
    m_map_view.setViewport(sf::FloatRect(0.70f, 0.70f, 0.25f, 0.25f));

    m_hud_view.setSize(m_window.getSize().x, m_window.getSize().y);
    m_hud_view.setCenter(m_window.getSize().x / 2, m_window.getSize().y / 2);
}

void Game::load_messages()
{
    float size = m_window.getSize().x / 47;
    sf::Text::Style style = sf::Text::Style::Regular;
    sf::Color color = sf::Color::White;
    int thickness = 2;

    m_message_level.set_properties(size, style, color, color, thickness);
    m_message_lives.set_properties(size, style, color, color, thickness);

    float size_1 = m_window.getSize().x / 20;
    m_message_game_state.set_properties(size_1, style, color, color, thickness);
}

void Game::set_bomb(int r, int c, int cells)
{
    sf::Vector2f size = m_walls.front().front()->get_size();
    sf::Vector2f pos = m_cells[r][c]->get_pos();

    m_explosions[r][c].reset(new Bomb(*this, m_texture_bomb, size, pos, r, c, cells));

    m_bomb_deployed = true;
}

void Game::handle_explosions()
{
    for (const auto &explosions : m_explosions)
    {
        for (const auto &explosion : explosions)
        {
            if (explosion && explosion->is_active())
            {
                for (auto &enemy : m_enemies)
                {
                    if (enemy &&
                        enemy->get_row() == explosion->get_row() &&
                        enemy->get_col() == explosion->get_col())
                    {
                        if (enemy->is_active())
                        {
                            enemy->set_active(false);
                            enemy->set_curr_anim_index(1);
                            enemy->set_curr_frame_index(0);

                            --m_total_enemies;
                        }
                    }
                }

                if (explosion->get_row() == m_player.get_row() &&
                    explosion->get_col() == m_player.get_col() &&
                    m_state == State::GameStarted)
                {
                    m_state = State::RestartLevel;
                    m_reduce_player_lives = true;
                }
            }
        }
    }
}

void Game::update_game_state()
{
    if (m_state == State::RestartLevel && m_reduce_player_lives)
    {
        m_player.reduce_lives();
        m_reduce_player_lives = false;
    }

    if (m_player.get_lives() == 0)
    {
        m_state = State::GameLost;
    }
    else if (m_total_enemies == 0)
    {
        m_state = State::GameWon;
    }
}

void Game::update_messages()
{
    std::string str_level = m_text_level + m_titles[m_curr_level];
    m_message_level.set_str(str_level);
    m_message_level.set_pos(m_message_level.get_char_size() * 5, m_message_level.get_char_size() * 1.5f);

    std::string str_lives = m_text_lives + std::to_string(m_player.get_lives());
    m_message_lives.set_str(str_lives);
    m_message_lives.set_pos(m_window.getSize().x - m_message_lives.get_char_size() * 5, m_message_lives.get_char_size() * 1.5f);

    if (m_state == State::GameLost)
    {
        m_message_game_state.set_str(m_text_game_lost);
    }
    else if (m_state == State::GameWon)
    {
        m_message_game_state.set_str(m_text_game_won);
    }
    else if (m_state == State::RestartLevel)
    {
        m_message_game_state.set_str(m_text_restart_level);
    }

    m_message_game_state.set_pos(m_window.getSize().x / 2.f, m_window.getSize().y / 2.f);
}

void Game::update_entities()
{
    m_player.move();

    for (auto &enemy : m_enemies)
    {
        if (enemy && enemy->is_active())
        {
            enemy->move();

            if (enemy->get_row() == m_player.get_row() &&
                enemy->get_col() == m_player.get_col() &&
                m_state == State::GameStarted)
            {
                m_state = State::RestartLevel;
                m_reduce_player_lives = true;
            }
        }
        else if (enemy && !enemy->is_active() && enemy->should_be_released())
        {
            enemy.release();
        }
    }

    handle_explosions();
    update_game_state();
    update_messages();
}

void Game::show_messages()
{
    m_message_level.show_message(m_window);
    m_message_lives.show_message(m_window);

    if (m_state == State::GameWon || m_state == State::GameLost || m_state == State::RestartLevel)
    {
        m_message_game_state.show_message(m_window);
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

    m_player.heal();

    load_field();
    load_views();
    update_messages();

    m_state = State::GameStarted;

    m_bomb_deployed = false;
}

void Game::reload_level()
{
    load_field();
    load_views();
    update_messages();

    m_state = State::GameStarted;

    m_bomb_deployed = false;
}

void Game::render_entities()
{
    for (const auto &cells : m_cells)
    {
        for (const auto &cell : cells)
        {
            if (cell)
            {
                cell->draw();
            }
        }
    }

    for (const auto &walls : m_walls)
    {
        for (const auto &wall : walls)
        {
            if (wall)
            {
                wall->draw();
            }
        }
    }

    for (const auto &explosions : m_explosions)
    {
        for (const auto &explosion : explosions)
        {
            if (explosion)
            {
                explosion->draw();
            }
        }
    }

    for (const auto &enemy : m_enemies)
    {
        if (enemy)
        {
            enemy->draw();
        }
    }

    m_player.draw();
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
            else if (m_state == State::Menu && m_play_button.is_pressed())
            {
                m_state = State::GameStarted;
                m_window.setMouseCursorVisible(false);
            }
            else if (m_state == State::GameStarted &&
                     event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::M)
            {
                m_show_mini_map = !m_show_mini_map;
            }
            else if (m_state == State::GameStarted && !m_bomb_deployed &&
                     event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
            {
                int r = m_player.get_row();
                int c = m_player.get_col();

                set_bomb(r, c, 3);
            }
            else if ((m_state == State::GameWon || m_state == State::GameLost) &&
                     event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
            {
                change_level();
            }
            else if (m_state == State::RestartLevel &&
                     event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
            {
                reload_level();
            }
        }

        if (m_state != State::Menu)
        {
            update_entities();
        }

        m_window.clear();

        if (m_state == State::Menu)
        {
            m_window.draw(m_menu_background);
            m_play_button.draw();
        }
        else
        {
            m_window.draw(m_game_background);

            m_window.setView(m_main_view);
            render_entities();

            if (m_show_mini_map)
            {
                m_window.setView(m_map_view);
                render_entities();
            }

            m_window.setView(m_hud_view);
            show_messages();
        }

        m_window.display();
    }
}