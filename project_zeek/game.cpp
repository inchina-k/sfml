#include "game.hpp"

#include <iostream>
#include <unordered_map>

Game::Game(sf::RenderWindow &window, sf::Font &font)
    : m_window(window), m_curr_level(0), m_player(*this),
      m_message_curr_level(m_text_level, font), m_message_points(m_text_points, font),
      m_message_game_state(m_text_game_won, font), m_message_game_name(m_text_game_name, font),
      m_play_button(window, window.getSize().y / 5, window.getSize().y / 10, m_play_button_text, font)
{
    load_background();

    if (!load_levels())
    {
        std::cout << "levels are not uploaded" << std::endl;
        exit(1);
    }

    load_field();
    load_messages();
    load_sounds();
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

    sf::Texture safe_cell, wall, bonus, fruit, ball, hazard, bomb, portal, crystal, key, gates;

    if (!safe_cell.loadFromFile("data/images/safe_cell.png") ||
        !wall.loadFromFile("data/images/wall.png") ||
        !bonus.loadFromFile("data/images/bonus.png") ||
        !fruit.loadFromFile("data/images/fruit.png") ||
        !ball.loadFromFile("data/images/ball.png") ||
        !hazard.loadFromFile("data/images/droodle.png") ||
        !bomb.loadFromFile("data/images/bomb.png") ||
        !portal.loadFromFile("data/images/portal.png") ||
        !crystal.loadFromFile("data/images/crystal.png") ||
        !key.loadFromFile("data/images/key.png") ||
        !gates.loadFromFile("data/images/gates.png"))
    {
        std::cout << "file for an object is missing" << std::endl;
        exit(1);
    }

    std::unordered_map<char, sf::Texture> cell_types =
        {{'.', safe_cell}, {'w', wall}, {'b', bonus}, {'f', fruit}, {'e', ball}, {'h', hazard}, {'m', bomb}, {'*', portal}, {'c', crystal}, {'k', key}, {'g', gates}};

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
                else if (type == 'c')
                {
                    m_objects[i].push_back(std::make_unique<Crystal>(*this, cell_types[type], pos, i, j));
                }
                else if (type == 'k')
                {
                    m_objects[i].push_back(std::make_unique<Key>(*this, cell_types[type], pos, i, j));
                }
                else if (type == 'g')
                {
                    m_objects[i].push_back(std::make_unique<Gates>(*this, cell_types[type], pos, i, j));
                }
            }

            pos.x += cell_size;
        }

        pos.x = x;
        pos.y += cell_size;
    }
}

void Game::load_background()
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
}

void Game::load_messages()
{
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

void Game::load_sounds()
{
    if (!m_bonus_buffer.loadFromFile("data/sounds/bonus.wav") ||
        !m_game_lost_buffer.loadFromFile("data/sounds/game_lost.wav") ||
        !m_game_won_buffer.loadFromFile("data/sounds/game_won.wav"))
    {
        std::cout << "Sound is not uploaded" << std::endl;
        exit(1);
    }

    m_bonus_sound.setBuffer(m_bonus_buffer);
    m_game_lost_sound.setBuffer(m_game_lost_buffer);
    m_game_won_sound.setBuffer(m_game_won_buffer);

    m_bonus_sound.setVolume(50);
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

        if (m_play_sound)
        {
            m_game_won_sound.play();
            m_play_sound = false;
        }
    }
    else if (m_state == State::GameLost)
    {
        m_message_game_state.set_str(m_text_game_lost);

        if (m_play_sound)
        {
            m_game_lost_sound.play();
            m_play_sound = false;
        }
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
                        object->catch_player();
                        m_player.set_caught(true);
                        m_state = State::GameLost;
                    }
                }
            }
        }

        update_messages();
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
    m_player.restore_keys();
    load_field();
    m_play_sound = true;
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
        }

        update_game_state();

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