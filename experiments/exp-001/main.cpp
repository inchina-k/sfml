#include <iostream>
#include <vector>
#include "../../libs/random.hpp"

using Random = effolkronium::random_static;

using namespace std;

int main()
{
    vector<int> v;

    for (int i = 0; i < 10; i++)
    {
        int num = Random::get(0, 15);
        v.push_back(num);
    }

    for (auto e : v)
    {
        cout << e << ' ';
    }
    cout << endl;
}