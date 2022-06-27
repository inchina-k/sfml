#include <SFML/Graphics.hpp>

#include <vector>

#include "game_2048.hpp"
#include "renderer.hpp"
#include "field.hpp"
#include "message.hpp"

using namespace std;

using Grid = vector<vector<int>>;

void switch_command(Game &game);

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 900), "SFML app");
    window.setVerticalSyncEnabled(true);

    vector<int> goals = {16, 32, 64, 128, 256, 512, 1024, 2048};

    const int default_goal = 16;
    int goal = default_goal;

    const int n_of_cells = 4;
    const int cell_size = 100;

    double x = window.getSize().x / 2 - cell_size * 2;
    double y = window.getSize().y / 2 - cell_size * 2;

    Field field(x, y, n_of_cells, cell_size);

    sf::Texture nums_texture;
    nums_texture.loadFromFile("data/2-2048 pieces.png");

    Game game(goal);
    Renderer renderer(game, nums_texture, x, y, n_of_cells, cell_size);

    sf::Font font;
    font.loadFromFile("data/AzeretMono-Regular.ttf");

    const int text_size = 70;
    sf::Text::Style text_style(sf::Text::Style::Bold);
    sf::Color text_fill(sf::Color::Cyan);
    sf::Color text_outline(209, 207, 207);
    int outline_thickness = 1;

    string text_default = "Game 2048";
    string text_won = "Game won!";
    string text_lost = "Game lost";
    Message message_title(text_default, font);
    message_title.set_properties(text_size, text_style, text_fill, text_outline, outline_thickness);

    string str_goal = "Goal:\n" + to_string(goal);
    Message message_goal(str_goal, font);
    message_goal.set_properties(text_size, text_style, text_fill, text_outline, outline_thickness);

    string text_score = "Score:\n";
    string curr_score = text_score + to_string(game.get_curr_score());
    Message message_score(curr_score, font);
    message_score.set_properties(text_size, text_style, text_fill, text_outline, outline_thickness);

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }

            message_goal.set_pos(x - 200, window.getSize().y / 2);

            curr_score = text_score + to_string(game.get_curr_score());
            message_score.set_str(curr_score);
            message_score.set_pos(x + cell_size * n_of_cells + 200, window.getSize().y / 2);

            bool won = game.game_won();
            bool lost = game.filled_up() && !game.merge_possible();

            double coord_x = window.getSize().x / 2;
            double coord_y = y - 100;

            if (!won && !(lost))
            {
                message_title.set_pos(coord_x, coord_y);
                switch_command(game);
            }
            else if (won)
            {
                message_title.set_str(text_won);
                message_title.set_pos(coord_x, coord_y);
            }
            else
            {
                message_title.set_str(text_lost);
                message_title.set_pos(coord_x, coord_y);
            }
        }

        window.clear();

        field.draw_field(window);

        renderer.render(window);

        message_goal.show_message(window);

        message_score.show_message(window);

        message_title.show_message(window);

        window.display();
    }
}

void switch_command(Game &game)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        game.move_left();
        game.add_random_number();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        game.move_up();
        game.add_random_number();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        game.move_right();
        game.add_random_number();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        game.move_down();
        game.add_random_number();
    }
}