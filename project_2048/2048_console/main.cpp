#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

int main(int argc, char *argv[])
{
    vector<int> nums = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};

    const int default_goal = 16;

    int goal;

    if (argc == 1)
    {
        goal = default_goal;
        cout << goal << endl;
    }
    else if (argc == 2)
    {
        istringstream iss(argv[1]);

        char zapretnye_symvoly;
        if (iss >> goal && !(iss >> zapretnye_symvoly))
        {
            if (count(nums.begin(), nums.end(), goal))
            {
                cout << goal << endl;
            }
            else
            {
                cout << "incorrect number" << endl;
                exit(1);
            }
        }
    }
    else
    {
        cout << "you screwed up" << endl;
    }
}
