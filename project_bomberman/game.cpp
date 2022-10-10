#include "game.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

Game::Game(sf::RenderWindow &window, sf::Font &font)
    : m_window(window), m_curr_level(0), m_player(*this),
      m_message_level(m_text_level, font), m_message_lives(m_text_lives, font),
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

    for (auto &enemy : m_enemies)
    {
        enemy->set_dir();
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
}

void Game::set_explosion(int r, int c, int dr, int dc)
{
    int row = r + dr;
    int col = c + dc;

    sf::Vector2f size = m_walls.front().front()->get_size();

    for (int i = 0; i < 3; i++)
    {
        if (m_walls[row][col])
        {
            break;
        }

        sf::Vector2f pos = m_cells[row][col]->get_pos();
        m_explosions[row][col].reset(new Explosion(*this, m_texture_explosion, size, pos, row, col));

        row += dr;
        col += dc;
    }
}

void Game::set_bomb(int r, int c)
{
    sf::Vector2f size = m_walls.front().front()->get_size();
    sf::Vector2f pos = m_cells[r][c]->get_pos();

    m_explosions[r][c].reset(new Bomb(*this, m_texture_bomb, size, pos, r, c));

    set_explosion(r, c, 1, 0);
    set_explosion(r, c, -1, 0);
    set_explosion(r, c, 0, 1);
    set_explosion(r, c, 0, -1);

    m_bomb_deployed = true;
}

void Game::update_messages()
{
    std::string str_level = m_text_level + m_titles[m_curr_level];
    m_message_level.set_str(str_level);
    m_message_level.set_pos(m_message_level.get_char_size() * 5, m_message_level.get_char_size() * 1.5f);

    std::string str_lives = m_text_lives + std::to_string(m_player.get_lives());
    m_message_lives.set_str(str_lives);
    m_message_lives.set_pos(m_window.getSize().x - m_message_lives.get_char_size() * 5, m_message_lives.get_char_size() * 1.5f);
}

void Game::show_messages()
{
    m_message_level.show_message(m_window);
    m_message_lives.show_message(m_window);
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
        enemy->draw();
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
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::M)
            {
                m_show_mini_map = !m_show_mini_map;
            }
            else if (m_state == State::Menu && m_play_button.is_pressed())
            {
                m_state = State::GameStarted;
                m_window.setMouseCursorVisible(false);
            }
            else if (m_state == State::GameStarted && !m_bomb_deployed &&
                     event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
            {
                int r = m_player.get_row();
                int c = m_player.get_col();

                set_bomb(r, c);
            }
        }

        if (m_state == State::GameStarted)
        {
            m_player.move();
            for (auto &enemy : m_enemies)
            {
                enemy->move();
            }
            update_messages();
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