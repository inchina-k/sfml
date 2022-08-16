#include <iostream>
#include <vector>
#include <memory>
#include <string>

using namespace std;

int main()
{
    vector<unique_ptr<string>> items;

    items.push_back(make_unique<string>("hello"));
    items.push_back(nullptr);
    items.push_back(make_unique<string>("world"));

    for (const auto &i : items)
    {
        if (i)
        {
            cout << *i << endl;
        }
    }
}