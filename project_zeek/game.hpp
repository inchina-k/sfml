#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "cell.hpp"
#include "message.hpp"
#include "button.hpp"

class Game
{
    /* ---------------PLAYER--------------- */

    class Player
    {
        enum class State
        {
            Go,
            Stand
        };

        Game &m_game;
        sf::Texture m_texture;
        std::vector<std::vector<std::unique_ptr<sf::Sprite>>> m_frames;
        int m_counter;
        size_t m_frame_index;
        size_t m_anim_index;
        State m_curr_state = State::Stand;
        sf::Vector2f m_pos;
        sf::Vector2f m_dir;
        float m_size;

        int m_row, m_col;
        float m_w = 0, m_h = 0;

        static constexpr int m_max_counter = 15;
        int m_num_of_steps;
        float m_step = 0;

        bool m_caught;
        int m_num_of_keys = 0;

        void load();
        void switch_command();
        bool can_move(int dr, int dc);
        void move();

    public:
        Player(Game &game);

        void set_pos(sf::Vector2f &pos, int row, int col);
        sf::Vector2f get_pos() const;
        sf::Vector2i get_coords() const;
        void set_size(float size);
        sf::Vector2f get_size() const;
        void set_caught(bool b);
        bool is_caught() const;
        void restore_keys();
        bool has_keys() const;
        void draw();
    };

    /* ---------------GAME_OBJECTS--------------- */

    class GameObject
    {
    protected:
        Game &m_game;
        sf::Texture m_texture;
        sf::Sprite m_body;
        sf::Vector2f m_pos;
        int m_row = 0, m_col = 0;

        /* ---------MOVE--------- */
        enum class State
        {
            Go,
            Stand
        };

        State m_curr_state = State::Stand;
        static constexpr int m_max_counter = 15;
        int m_num_of_steps;
        float m_step = 0;
        int m_frame_index = 0;
        int m_anim_index = 0;
        int m_counter = 0;
        sf::Vector2f m_dir;

    public:
        GameObject(Game &game, sf::Texture &texture, sf::Vector2f &pos, int row, int col);

        void set_dir(int dr, int dc);
        void move();
        virtual void draw() = 0;
    };

    class Bonus : public GameObject
    {
    public:
        Bonus(Game &game, sf::Texture &texture, sf::Vector2f &pos, int row, int col);

        void draw() override;
    };

    class Hazard : public GameObject
    {
        bool m_dangerous;
        int m_neutralized_counter = 500;

    public:
        Hazard(Game &game, sf::Texture &texture, sf::Vector2f &pos, int row, int col);

        void raise_hands(int dir_row, int dir_col);
        void set_dangerous(bool b);
        bool is_dangerous() const;
        void catch_fruit(int row, int col, int dir_row, int dir_col);
        void catch_player(int dir_row, int dir_col);
        void draw() override;
    };

    class Fruit : public GameObject
    {
    public:
        Fruit(Game &game, sf::Texture &texture, sf::Vector2f &pos, int row, int col);

        void draw() override;
    };

    class Ball : public GameObject
    {
    public:
        Ball(Game &game, sf::Texture &texture, sf::Vector2f &pos, int row, int col);

        void draw() override;
    };

    class Portal : public GameObject
    {
    public:
        Portal(Game &game, sf::Texture &texture, sf::Vector2f &pos, int row, int col);

        void draw() override;
    };

    class Bomb : public GameObject
    {
        std::vector<std::vector<std::unique_ptr<sf::Sprite>>> m_frames;
        float m_w = 0;
        float m_h = 0;
        int m_explosion_counter = 150;
        bool m_deployed;

        void load();

    public:
        Bomb(Game &game, sf::Texture &texture, sf::Vector2f &pos, int row, int col);

        void set_deployed(bool b);
        bool is_deployed() const;
        void explode();
        void draw() override;
    };

    class Crystal : public GameObject
    {
        std::vector<std::vector<std::unique_ptr<sf::Sprite>>> m_frames;
        float m_w = 0;
        float m_h = 0;
        int m_explosion_counter = 150;
        bool m_activated;

        void load();

    public:
        Crystal(Game &game, sf::Texture &texture, sf::Vector2f &pos, int row, int col);

        void set_activated(bool b);
        bool is_activated() const;
        void explode();
        void draw() override;
    };

    class Gates : public GameObject
    {
        std::vector<std::vector<std::unique_ptr<sf::Sprite>>> m_frames;
        float m_w = 0;
        float m_h = 0;
        int m_open_counter = 15;
        bool m_opened;

        void load();

    public:
        Gates(Game &game, sf::Texture &texture, sf::Vector2f &pos, int row, int col);

        void set_opened(bool b);
        bool is_opened() const;
        void open();
        void draw() override;
    };

    class Key : public GameObject
    {
    public:
        Key(Game &game, sf::Texture &texture, sf::Vector2f &pos, int row, int col);

        void draw() override;
    };

    /* ---------------GAME--------------- */

    sf::RenderWindow &m_window;

    std::vector<std::string> m_titles;
    std::vector<std::vector<std::string>> m_levels;
    std::vector<std::string> m_level;
    size_t m_curr_level;

    Player m_player;

    std::vector<std::vector<std::unique_ptr<GameObject>>> m_objects;
    std::vector<std::unique_ptr<Cell>> m_cells;
    sf::RectangleShape m_boundaries;

    int m_total_bonuses;
    int m_collected_bonuses;

    std::string m_text_level = "level: ";
    std::string m_text_points = "points: ";
    Message m_message_curr_level;
    Message m_message_points;
    std::string m_text_game_won = "Mission passed";
    std::string m_text_game_lost = "Mission failed";
    Message m_message_game_state;

    sf::Texture m_menu_background_texture;
    sf::Texture m_game_background_texture;
    sf::Sprite m_background;
    std::string m_text_game_name = "d & d";
    Message m_message_game_name;
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

    /* ---------SOUNDS--------- */

    sf::SoundBuffer m_bonus_buffer, m_game_lost_buffer, m_game_won_buffer, m_key_buffer, m_gates_opening_buffer,
        m_chewing_buffer, m_bomb_deployed_buffer, m_bomb_exploded_buffer, m_crystal_activated_buffer, m_crystal_exploded_buffer,
        m_pushing_buffer;
    sf::Sound m_bonus_sound, m_game_lost_sound, m_game_won_sound, m_key_sound, m_gates_opening_sound,
        m_chewing_sound, m_bomb_deployed_sound, m_bomb_exploded_sound, m_crystal_activated_sound, m_crystal_exploded_sound,
        m_pushing_sound;
    bool m_play_sound = true;

    /* ---------MEMBER_FUNCTIONS/METHODS--------- */

    bool load_levels();
    void load_field();
    void load_background();
    void load_messages();
    void load_sounds();
    bool in_field(int row, int col) const;
    void update_messages();
    void update_game_state();
    void change_level();
    void render_entities();

public:
    Game(sf::RenderWindow &window, sf::Font &font);

    void run();
};