#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <string>

#include "game_2048.hpp"
#include "renderer.hpp"

using namespace std;

void switch_command(Game &game, Renderer &renderer);
void show_result(Game &game, Renderer &renderer);

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

    Game game(goal);
    Renderer renderer(game);

    cout << "---Game 2048---" << endl;
    cout << "rules: join numbers to achieve the goal " << game.get_goal() << endl;
    cout << "allowed commands: {left, up, right, down} to move the numbers" << endl;
    cout << "                  {exit} to exit the game" << endl;

    cout << "\ngoal: " << game.get_goal() << endl;
    cout << "current score: " << game.get_curr_score() << endl;
    renderer.render();

    cout << "\ntype command: ";
    switch_command(game, renderer);
}

void switch_command(Game &game, Renderer &renderer)
{
    string command;

    while (cin >> command)
    {
        if (command == "left")
        {
            game.move_left();
            game.add_random_number();
        }
        else if (command == "up")
        {
            game.move_up();
            game.add_random_number();
        }
        else if (command == "right")
        {
            game.move_right();
            game.add_random_number();
        }
        else if (command == "down")
        {
            game.move_down();
            game.add_random_number();
        }
        else if (command == "exit")
        {
            cout << "\ngame exited" << endl;
            exit(0);
        }
        else
        {
            cout << "\nunknown command: " << command << endl;
            cout << "allowed commands are {left, up, right, down, exit}" << endl;
        }

        show_result(game, renderer);
    }
}

void show_result(Game &game, Renderer &renderer)
{
    cout << "\ngoal: " << game.get_goal() << endl;
    cout << "current score: " << game.get_curr_score() << endl;
    renderer.render();

    if (game.game_won())
    {
        cout << "\nthe goal " << game.get_goal() << " achieved!" << endl;
        exit(0);
    }
    else if (game.filled_up() && !game.merge_possible())
    {
        cout << "\ngame lost" << endl;
        exit(0);
    }

    cout << "\ntype command: ";
}