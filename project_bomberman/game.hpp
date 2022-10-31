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
        bool m_active = false;

        static constexpr int m_max_counter = 45;
        int m_frame_index = 0;
        int m_anim_index = 0;
        int m_counter = 0;

    public:
        GameObject(Game &game, sf::Texture &texture, sf::Vector2f &size, sf::Vector2f &pos, int row, int col);

        virtual sf::Vector2f get_size() const;
        sf::Vector2f get_pos() const;
        virtual sf::FloatRect get_bounds() const;
        bool is_active() const;
        void set_active(bool b);
        int get_curr_anim_index() const;
        int get_curr_frame_index() const;
        void set_curr_anim_index(int index);
        void set_curr_frame_index(int index);
        int get_row() const;
        int get_col() const;
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

    class Bomb : public GameObject
    {
        std::vector<std::vector<std::unique_ptr<sf::Sprite>>> m_frames;
        float m_w = 0;
        float m_h = 0;
        int m_explosion_counter = 150;
        int m_cells;

        void load(sf::Vector2f &size);
        void set_explosion(int r, int c, int dr, int dc);
        void handle_explosion();

    public:
        Bomb(Game &game, sf::Texture &texture, sf::Vector2f &size, sf::Vector2f &pos, int row, int col, int cells);

        sf::FloatRect get_bounds() const override;
        bool is_exploded() const;
        void draw() override;
    };

    class Explosion : public GameObject
    {
        std::vector<std::vector<std::unique_ptr<sf::Sprite>>> m_frames;
        float m_w = 0;
        float m_h = 0;

        void load(sf::Vector2f &size);

    public:
        Explosion(Game &game, sf::Texture &texture, sf::Vector2f &size, sf::Vector2f &pos, int row, int col);

        sf::FloatRect get_bounds() const override;
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
        bool m_should_be_released = false;

        void load(sf::Vector2f &size);
        sf::Vector2f get_size() const override;
        void check_collision_dir();

    public:
        Enemy(Game &game, sf::Texture &texture, sf::Vector2f &size, sf::Vector2f &pos, int row, int col);

        void set_dir();
        sf::FloatRect get_bounds() const override;
        bool should_be_released() const;
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

    public:
        Player(Game &game);

        sf::FloatRect get_bounds() const;
        void set_size(sf::Vector2f &size);
        sf::Vector2f get_size() const;
        void set_pos(sf::Vector2f &pos, int row, int col);
        sf::Vector2f get_pos() const;
        size_t get_lives() const;
        void reduce_lives();
        void heal();
        int get_row() const;
        int get_col() const;
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

    sf::Texture m_texture_safe_cell, m_texture_wall, m_texture_enemy, m_texture_bomb, m_texture_explosion;

    int m_total_enemies = 0;
    std::vector<std::unique_ptr<Enemy>> m_enemies;
    std::vector<std::vector<std::unique_ptr<Wall>>> m_walls;
    std::vector<std::vector<std::unique_ptr<SafeCell>>> m_cells;
    std::vector<std::vector<std::unique_ptr<GameObject>>> m_explosions;

    Player m_player;

    std::string m_text_level = "level: ";
    std::string m_text_lives = "lives: ";
    Message m_message_level;
    Message m_message_lives;

    std::string m_text_game_won = "Mission completed";
    std::string m_text_game_lost = "Mission failed";
    std::string m_text_restart_level = "Life lost";
    Message m_message_game_state;

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
        GameLost,
        RestartLevel
    };

    State m_state = State::Menu;
    bool m_show_mini_map = false;
    bool m_bomb_deployed = false;
    bool m_reduce_player_lives = false;

    /* ---------MEMBER_FUNCTIONS/METHODS--------- */

    bool load_levels();
    void load_background();
    void load_field();
    void load_views();
    void load_messages();
    void set_bomb(int r, int c, int cells);
    void handle_explosions();
    void update_game_state();
    void update_messages();
    void update_entities();
    void show_messages();
    // void load_sounds();
    void change_level();
    void reload_level();
    void render_entities();

public:
    Game(sf::RenderWindow &window, sf::Font &font);

    void run();
};
