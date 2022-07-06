#include <iostream>
#include <cstdint>

using namespace std;

string to_binary(int32_t x)
{
    uint32_t u = x;

    string res(32, '0');

    uint32_t mask = 1;

    for (size_t i = 0; i < 32; i++, mask <<= 1)
    {
        if (u & mask)
        {
            res[i] = '1';
        }
    }

    return res;
}

int main()
{
    uint32_t flags = 3;
    flags <<= 2;
    cout << flags << endl;

    cout << to_binary(-1) << endl;

#ifdef AUCA_DEBUG
    std::clog << "ok" << std::endl;
#endif
}