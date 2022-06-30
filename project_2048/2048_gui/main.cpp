#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "game_2048.hpp"
#include "renderer.hpp"
#include "field.hpp"
#include "message.hpp"

using namespace std;

using Grid = vector<vector<int>>;

void switch_command(Game &game, sf::Sound &sound_cell_moved);
void change_goal(int &goal, vector<int> &goals, int &goal_index, string &str_goal, Message &message_goal);

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML app", sf::Style::Fullscreen);
    window.setVerticalSyncEnabled(true);

    const sf::Time frames_per_sec = sf::seconds(0.1f);
    sf::Time total_time = sf::Time::Zero;
    sf::Clock clock;

    vector<int> goals = {16, 32, 64, 128, 256, 512, 1024, 2048};
    int goal_index = 0;

    const int default_goal = 16;
    int goal = default_goal;

    const int n_of_cells = 4;
    const int image_size = 100;
    const double cell_size = min(window.getSize().x, window.getSize().y) / (2 * n_of_cells);

    double x = window.getSize().x / 2 - cell_size * 2;
    double y = window.getSize().y / 2 - cell_size * 2;

    Field field(x, y, n_of_cells, cell_size);

    sf::Texture nums_texture;
    nums_texture.loadFromFile("data/2048 pixel pieces.png");

    Game game(goal);
    Renderer renderer(game, nums_texture, x, y, n_of_cells, cell_size, image_size);

    sf::Font font;
    font.loadFromFile("data/PressStart2P-Regular.ttf");

    const int text_size = cell_size / 2.5;
    sf::Text::Style text_style(sf::Text::Style::Bold);
    sf::Color text_fill(sf::Color::Cyan);
    sf::Color text_outline(209, 207, 207);
    int outline_thickness = 1;

    string text_default = "Game 2048";
    string text_won = "Game won!";
    string text_lost = "Game lost";
    Message message_title(text_default, font);
    message_title.set_properties(text_size, text_style, text_fill, text_outline, outline_thickness);

    string str_goal = "Goal:\n\n" + to_string(game.get_goal());
    Message message_goal(str_goal, font);
    message_goal.set_properties(text_size, text_style, text_fill, text_outline, outline_thickness);

    string text_score = "Score:\n\n";
    string curr_score = text_score + to_string(game.get_curr_score());
    Message message_score(curr_score, font);
    message_score.set_properties(text_size, text_style, text_fill, text_outline, outline_thickness);

    string text_best_score = "Best:\n\n";
    string best_score = text_best_score + to_string(game.get_best_score(game.get_goal()));
    Message message_best_score(best_score, font);
    message_best_score.set_properties(text_size, text_style, text_fill, text_outline, outline_thickness);

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
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
            {
                renderer.restart_game(goal, window);
                message_title.set_str(text_default);
                play_sound = true;
            }

            message_goal.set_pos(x / 2, window.getSize().y / 2);

            curr_score = text_score + to_string(game.get_curr_score());
            message_score.set_str(curr_score);
            message_score.set_pos(window.getSize().x - x / 2, window.getSize().y / 2 - 100);

            best_score = text_best_score + to_string(game.get_best_score(game.get_goal()));
            message_best_score.set_str(best_score);
            message_best_score.set_pos(window.getSize().x - x / 2, window.getSize().y / 2 + 100);

            bool won = game.game_won();
            bool lost = game.filled_up() && !game.merge_possible();

            double coord_x = window.getSize().x / 2;
            double coord_y = y / 2;

            game.update_best_score();

            if (!won && !(lost))
            {
                if (!game.get_curr_score())
                {
                    change_goal(goal, goals, goal_index, str_goal, message_goal);
                    game.set_goal(goal);
                }

                switch_command(game, sound_cell_moved);
            }
            else if (won)
            {
                message_title.set_str(text_won);
                change_goal(goal, goals, goal_index, str_goal, message_goal);

                if (play_sound)
                {
                    sound_game_won.play();
                }

                play_sound = false;
            }
            else
            {
                message_title.set_str(text_lost);
                change_goal(goal, goals, goal_index, str_goal, message_goal);

                if (play_sound)
                {
                    sound_game_lost.play();
                }

                play_sound = false;
            }

            message_title.set_pos(coord_x, coord_y);
        }

        if (total_time > frames_per_sec)
        {
            total_time -= frames_per_sec;

            window.clear(sf::Color(3, 19, 43));

            field.draw_field(window);

            renderer.render(window);

            message_goal.show_message(window);

            message_score.show_message(window);

            message_best_score.show_message(window);

            message_title.show_message(window);

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

void change_goal(int &goal, vector<int> &goals, int &goal_index, string &str_goal, Message &message_goal)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp) && goal_index < int(goals.size() - 1))
    {
        goal = goals[++goal_index];
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown) && goal_index > 0)
    {
        goal = goals[--goal_index];
    }

    str_goal = "Goal:\n\n" + to_string(goal);
    message_goal.set_str(str_goal);
}