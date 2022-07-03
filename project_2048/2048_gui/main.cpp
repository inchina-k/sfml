#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "game_2048.hpp"
#include "renderer.hpp"
#include "field.hpp"
#include "messages.hpp"

using namespace std;

void switch_command(Game &game, sf::Sound &sound_cell_moved);
void change_goal(Game &game, vector<int> &goals, int &goal_index);

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML app", sf::Style::Fullscreen);
    window.setVerticalSyncEnabled(true);

    const sf::Time frames_per_sec = sf::seconds(0.1f);
    sf::Time total_time = sf::Time::Zero;
    sf::Clock clock;

    // Game
    Game game;

    vector<int> goals = game.get_goals();
    int goal_index = 0;

    // Field
    const int n_of_cells = game.get_puzzle_size();
    const double cell_size = min(window.getSize().x, window.getSize().y) / (2 * n_of_cells);

    double x = window.getSize().x / 2 - cell_size * 2;
    double y = window.getSize().y / 2 - cell_size * 2;

    Field field(x, y, n_of_cells, cell_size);

    // Renderer
    sf::Texture nums_texture;
    nums_texture.loadFromFile("data/2048 pixel pieces.png");
    const int image_size = 100;

    Renderer renderer(game, nums_texture, x, y, n_of_cells, cell_size, image_size);

    // Messages
    sf::Font font;
    font.loadFromFile("data/PressStart2P-Regular.ttf");
    const int text_size = cell_size / 2.5;

    Messages messages(game, font, text_size);
    messages.load_messages();

    // Sounds
    sf::SoundBuffer buff_cell_moved, buff_game_won, buff_game_lost;

    if (!buff_cell_moved.loadFromFile("data/pop.wav") || !buff_game_won.loadFromFile("data/game_won.wav") || !buff_game_lost.loadFromFile("data/game_lost.wav"))
    {
        exit(1);
    }

    sf::Sound sound_cell_moved, sound_game_won, sound_game_lost;

    sound_cell_moved.setBuffer(buff_cell_moved);
    sound_game_won.setBuffer(buff_game_won);
    sound_game_lost.setBuffer(buff_game_lost);

    sound_game_won.setLoop(false);
    sound_game_lost.setLoop(false);

    bool play_sound = true;

    while (window.isOpen())
    {
        total_time += clock.restart();

        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                renderer.restart_game(window);
                messages.set_title_default();
                play_sound = true;
            }

            bool won = game.game_won();
            bool lost = game.game_lost();

            if (!won && !(lost))
            {
                if (!game.game_started())
                {
                    change_goal(game, goals, goal_index);
                    messages.change_message_goal();
                }

                switch_command(game, sound_cell_moved);
            }
            else if (won)
            {
                change_goal(game, goals, goal_index);
                messages.change_message_goal();

                messages.set_title_game_won();

                if (play_sound)
                {
                    sound_game_won.play();
                }

                play_sound = false;
            }
            else
            {
                change_goal(game, goals, goal_index);
                messages.change_message_goal();

                messages.set_title_game_lost();

                if (play_sound)
                {
                    sound_game_lost.play();
                }

                play_sound = false;
            }

            messages.place_messages(window, x, y, cell_size);

            messages.change_message_curr_score();

            game.update_best_score();
            messages.change_message_best_score();
        }

        if (total_time > frames_per_sec)
        {
            total_time -= frames_per_sec;

            window.clear(sf::Color(3, 19, 43));

            field.draw_field(window);

            renderer.render(window);

            messages.show_messages(window);

            window.display();
        }
    }
}

void switch_command(Game &game, sf::Sound &sound_cell_moved)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && game.frames_empty())
    {
        game.move_left();
        sound_cell_moved.play();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && game.frames_empty())
    {
        game.move_up();
        sound_cell_moved.play();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && game.frames_empty())
    {
        game.move_right();
        sound_cell_moved.play();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && game.frames_empty())
    {
        game.move_down();
        sound_cell_moved.play();
    }
}

void change_goal(Game &game, vector<int> &goals, int &goal_index)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp) && goal_index < int(goals.size() - 1))
    {
        game.set_goal(goals[++goal_index]);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown) && goal_index > 0)
    {
        game.set_goal(goals[--goal_index]);
    }
}