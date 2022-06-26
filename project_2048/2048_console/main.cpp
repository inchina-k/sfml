#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <string>

#include "game_2048.hpp"
#include "renderer.hpp"

using namespace std;

void count_curr_score(Game &game);
void switch_command(Game &game, int goal, Renderer &renderer);
void show_result(Game &game, int goal, Renderer &renderer, string &command);

int main(int argc, char *argv[])
{
    vector<int> nums = {8, 16, 32, 64, 128, 256, 512, 1024, 2048};

    const int default_goal = 16;

    int goal;

    if (argc == 1)
    {
        goal = default_goal;
    }
    else if (argc == 2)
    {
        istringstream iss(argv[1]);

        char prohibited_symbols;
        if (!(iss >> goal) || iss >> prohibited_symbols || !count(nums.begin(), nums.end(), goal))
        {
            cout << "incorrect goal: " << argv[1] << endl;
            cout << "allowed goals are {8, 16, 32, 64, 128, 256, 512, 1024, 2048}" << endl;
            exit(1);
        }
    }
    else
    {
        cout << "too many arguments" << endl;
        exit(1);
    }

    cout << "---Game 2048---" << endl;
    cout << "rules: join numbers to achieve the goal " << goal << endl;
    cout << "       allowed commands are {left, up, right, down}" << endl;

    Game game(goal);
    Renderer renderer(game);

    cout << "\ncurrent score: " << game.get_curr_score() << endl;
    cout << "numbers sum: " << game.get_numbers_sum() << endl;
    renderer.render();

    cout << "\ntype command: ";
    switch_command(game, goal, renderer);
}

void switch_command(Game &game, int goal, Renderer &renderer)
{
    string command;

    while (cin >> command)
    {
        if (command == "left")
        {
            game.move_left();
        }
        else if (command == "up")
        {
            game.move_up();
        }
        else if (command == "right")
        {
            game.move_right();
        }
        else if (command == "down")
        {
            game.move_down();
        }
        else if (command == "exit")
        {
            cout << "\ngame exited" << endl;
            exit(1);
        }
        else
        {
            cout << "\nunknown command: " << command << endl;
            cout << "allowed commands are {left, up, right, down}" << endl;
        }

        show_result(game, goal, renderer, command);
    }
}

void show_result(Game &game, int goal, Renderer &renderer, string &command)
{
    if (command == "left" || command == "up" || command == "right" || command == "down")
    {
        count_curr_score(game);
        game.add_random_number();
    }

    cout << "\ncurrent score: " << game.get_curr_score() << endl;
    cout << "numbers sum: " << game.get_numbers_sum() << endl;
    renderer.render();

    if (game.get_curr_score() == goal)
    {
        cout << "\nthe goal " << goal << " achieved!" << endl;
        exit(0);
    }
    else if (game.filled_up())
    {
        cout << "\ngame lost" << endl;
        exit(0);
    }

    cout << "\ntype command: ";
}

void count_curr_score(Game &game)
{
    int max_num = 0;

    for (const auto &row : game.get_puzzle())
    {
        for (auto num : row)
        {
            max_num = max(max_num, num);
        }
    }

    game.set_curr_score(max_num);
}