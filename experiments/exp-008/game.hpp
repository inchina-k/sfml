#pragma once

#include <SFML/Graphics.hpp>

using namespace std;

class Game
{
    enum class Dirs
    {
        None,
        Up,
        Right,
        Down,
        Left
    };

    const sf::Time m_frames_per_sec;
    sf::Time m_total_time;
    sf::Clock m_clock;

    int m_field_size;
    int m_width;
    int m_height;
    int m_cell_size;

    sf::RectangleShape m_cell;
    int m_rect_x;
    int m_rect_y;

    sf::CircleShape circle;
    int m_circle_x;
    int m_circle_y;

    Dirs m_dir;
    int m_col;
    int m_row;

public:
    Game(int size, int width, int height);

    void process_events();

    void update();

    void draw(sf::RenderWindow &window);

    void run(sf::RenderWindow &window);
};