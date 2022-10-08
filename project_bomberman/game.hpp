#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "message.hpp"
#include "button.hpp"

class Game
{
    /* ---------------GAME_OBJECTS--------------- */

    class GameObject
    {
    protected:
        Game &m_game;
        sf::Texture m_texture;
        sf::Sprite m_body;
        sf::Vector2f m_pos;
        int m_row = 0, m_col = 0;

        static constexpr int m_max_counter = 45;
        int m_frame_index = 0;
        int m_anim_index = 0;
        int m_counter = 0;

    public:
        GameObject(Game &game, sf::Texture &texture, sf::Vector2f &size, sf::Vector2f &pos, int row, int col);

        virtual sf::Vector2f get_size() const;
        sf::Vector2f get_pos() const;
        sf::FloatRect get_bounds() const;
        virtual void draw() = 0;
    };

    class SafeCell : public GameObject
    {
    public:
        SafeCell(Game &game, sf::Texture &texture, sf::Vector2f &size, sf::Vector2f &pos, int row, int col);

        void draw() override;
    };

    class Wall : public GameObject
    {
    public:
        Wall(Game &game, sf::Texture &texture, sf::Vector2f &size, sf::Vector2f &pos, int row, int col);

        void draw() override;
    };

    class Enemy : public GameObject
    {
        std::vector<std::vector<std::unique_ptr<sf::Sprite>>> m_frames;
        float m_w = 0;
        float m_h = 0;
        float m_step = 0;
        sf::Vector2i m_dir;
        int m_change_dir = true;

        // enum class State
        // {
        //     Go,
        //     Stand
        // };
        //
        // State m_curr_state = State::Stand;

        void load(sf::Vector2f &size);
        virtual sf::Vector2f get_size() const override;
        sf::FloatRect get_bounds() const;
        void check_collision_dir();

    public:
        Enemy(Game &game, sf::Texture &texture, sf::Vector2f &size, sf::Vector2f &pos, int row, int col);

        void set_dir();
        void move();
        void draw() override;
    };

    /* ---------------PLAYER--------------- */

    class Player
    {
        Game &m_game;
        sf::Texture m_texture;
        std::vector<std::vector<std::unique_ptr<sf::Sprite>>> m_frames;
        static constexpr int m_max_counter = 45;
        size_t m_frame_index = 0;
        size_t m_anim_index = 0;
        int m_counter = 0;
        sf::Vector2f m_pos;
        sf::Vector2f m_dir;
        float m_w = 0, m_h = 0;
        int m_row = 0, m_col = 0;

        int m_num_of_steps;
        float m_step = 0;

        const size_t m_lives;
        size_t m_lives_left;

        enum class State
        {
            Down,
            Up,
            Left,
            Right,
            Stand
        };

        State m_curr_state = State::Stand;

        void load();
        bool can_move();
        void switch_command();
        sf::FloatRect get_bounds() const;

    public:
        Player(Game &game);

        void set_size(sf::Vector2f &size);
        sf::Vector2f get_size() const;
        void set_pos(sf::Vector2f &pos, int row, int col);
        sf::Vector2f get_pos() const;
        size_t get_lives() const;
        void move();
        void draw();
    };

    /* ---------------GAME--------------- */

    sf::RenderWindow &m_window;

    sf::View m_main_view;
    sf::View m_map_view;
    sf::View m_hud_view;

    std::vector<std::string> m_titles;
    std::vector<std::vector<std::string>> m_levels;
    std::vector<std::string> m_level;
    size_t m_curr_level;

    std::vector<std::unique_ptr<Enemy>> m_objects;
    std::vector<std::vector<std::unique_ptr<Wall>>> m_walls;
    std::vector<std::vector<std::unique_ptr<SafeCell>>> m_cells;

    Player m_player;

    std::string m_text_level = "level: ";
    std::string m_text_lives = "lives: ";
    Message m_message_level;
    Message m_message_lives;

    sf::Texture m_menu_texture;
    sf::Texture m_game_texture;
    sf::Sprite m_menu_background;
    sf::Sprite m_game_background;

    std::string m_play_button_text = "Play";
    Button m_play_button;

    enum class State
    {
        Menu,
        GameStarted,
        GameWon,
        GameLost
    };

    State m_state = State::Menu;
    bool m_show_mini_map = false;

    /* ---------MEMBER_FUNCTIONS/METHODS--------- */

    bool load_levels();
    void load_background();
    void load_field();
    void load_views();
    void load_messages();
    void update_messages();
    void show_messages();
    // void load_sounds();
    // void update_game_state();
    // void change_level();
    void render_entities();

public:
    Game(sf::RenderWindow &window, sf::Font &font);

    void run();
};
