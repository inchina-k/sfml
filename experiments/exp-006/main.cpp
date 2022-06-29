#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<vector<int>> puzzle = {{0, 0, 4, 2}, {0, 8, 0, 0}, {0, 64, 0, 32}, {0, 2, 2, 4}};

    vector<vector<bool>> merged(puzzle.size(), vector<bool>(puzzle.size()));

    for (int step = 0; step < 4; step++)
    {
        bool is_changed = false;

        for (int row = 0; row < 4; row++)
        {
            for (int col = 1; col < 4; col++)
            {
                if (puzzle[row][col] == puzzle[row][col - 1] && puzzle[row][col] && !merged[row][col - 1] && !merged[row][col])
                {
                    puzzle[row][col - 1] *= 2;
                    puzzle[row][col] = 0;
                    merged[row][col - 1] = true;
                    is_changed = true;
                }
                else if (!puzzle[row][col - 1] && puzzle[row][col])
                {
                    puzzle[row][col - 1] = puzzle[row][col];
                    puzzle[row][col] = 0;
                    is_changed = true;
                }
            }
        }

        if (is_changed)
        {
            for (const auto &r : puzzle)
            {
                for (auto num : r)
                {
                    cout << num << ' ';
                }
                cout << endl;
            }

            cout << endl;
        }
    }
}