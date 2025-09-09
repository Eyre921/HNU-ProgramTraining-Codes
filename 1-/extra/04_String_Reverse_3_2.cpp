// Original Filename: 04.字符串反转3.cpp
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

string reversedWords(string &s)
{
    int len = s.length();
    int start = 0;
    for (int i = 0; i <= len; i++)
    {
        if (s[i] == ' ' || i == len)
        {
            reverse(s.begin() + start, s.begin() + i);
            start = i + 1;
        }
    }
    return s;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    cin >> n;
    cin.ignore();
    for (int i = 0; i < n; i++)
    {
        string line;
        getline(cin, line);
        cout << reversedWords(line) << endl;
    }
}
