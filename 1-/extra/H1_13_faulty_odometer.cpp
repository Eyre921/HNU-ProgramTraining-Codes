// Original Filename: 13.错误的里程表.cpp
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

int main()
{
    int n;
    cin >> n;
    while (n--)
    {
        string s;
        cin >> s;
        for (auto &c: s)
        {
            if (c - '0' > 2 && c - '0' < 8) c -= 1;
            if (c == '9') c -= 2;
        }
        int res = strtol(s.c_str(),NULL, 8);
        cout << res << endl;
    }

    return 0;
}
