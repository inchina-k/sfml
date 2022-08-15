#pragma once

#include <SFML/Graphics.hpp>

#include <vector>
#include <memory>

class Player
{
    enum class State
    {
        GoDown,
        GoUp,
        GoLeft,
        GoRight,
        StandDown,
        StandUp,
        StandLeft,
        StandRight
    };

    sf::RenderWindow &m_window;
    sf::Texture m_texture;
    std::vector<std::vector<std::unique_ptr<sf::Sprite>>> m_frames;
    int m_counter;
    size_t m_frame_index;
    size_t m_anim_index;
    State m_curr_state = State::StandDown;
    sf::Vector2f m_pos;
    float m_size;

    int m_x = 0,
        m_y = 0;

    float m_w, m_h;

    void set_state(State state);
    void load();
    void move_down(float step, sf::Vector2f &bottom, float sz, std::vector<std::string> &field, float x, float y);
    void move_up(float step, sf::Vector2f &top, float sz, std::vector<std::string> &field, float x, float y);
    void move_left(float step, sf::Vector2f &top, float sz, std::vector<std::string> &field, float x, float y);
    void move_right(float step, sf::Vector2f &bottom, float sz, std::vector<std::string> &field, float x, float y);

public:
    Player(sf::RenderWindow &window);

    void set_pos(sf::Vector2f &coords);
    sf::Vector2f get_pos() const;
    void set_size(float size);
    sf::Vector2f get_size() const;
    void move(sf::Vector2f &top, sf::Vector2f &bottom, float sz, std::vector<std::string> &field, float x, float y);
    void draw();
};