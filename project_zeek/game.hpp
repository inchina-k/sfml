#pragma once

#include <SFML/Graphics.hpp>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "player.hpp"
#include "cell.hpp"

class Game
{
    class GameObject
    {
    protected:
        Game &m_game;
        sf::Texture m_texture;
        sf::Sprite m_body;
        int m_row = 0, m_col = 0;
        float m_x = 0, m_y = 0;

    public:
        GameObject(Game &game, sf::Texture &texture, int row, int col);

        virtual void draw() = 0;

        void set_pos(sf::Vector2f &pos)
        {
            m_body.setPosition(pos);
        }
    };

    struct IMovable
    {
        virtual bool move(int dx, int dy) = 0;
    };

    class Bonus : public GameObject
    {
    public:
        Bonus(Game &game, sf::Texture &texture, int row, int col);

        void draw() override;
    };

    class Hazard : public GameObject
    {
    public:
        Hazard(Game &game, sf::Texture &texture, int row, int col);

        void draw() override;
    };

    class Fruit : public GameObject, IMovable
    {
    public:
        Fruit(Game &game, sf::Texture &texture, int row, int col);

        void draw() override;
        bool move(int dx, int dy) override;
    };

    class Ball : public GameObject, IMovable
    {
    public:
        Ball(Game &game, sf::Texture &texture, int row, int col);

        void draw() override;
        bool move(int dx, int dy) override;
    };

    sf::RenderWindow &m_window;

    std::vector<std::string> m_titles;
    std::vector<std::vector<std::string>> m_levels;
    size_t m_curr_level;

    Player m_player;

    std::vector<std::unique_ptr<GameObject>> m_objects;
    std::vector<std::unique_ptr<Cell>> m_cells;

    bool load_levels();
    void load_field();

public:
    Game(sf::RenderWindow &window);

    void run();
};