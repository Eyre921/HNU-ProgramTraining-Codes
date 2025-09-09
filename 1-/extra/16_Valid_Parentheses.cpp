// Original Filename: 16.合法的括号串.cpp
#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    cin >> n;
    while (n--)
    {
        stack<char> s, s1;
        string str;
        cin >> str;
        if (str.size() % 2 != 0)
        {
            cout << "No" << endl;
            continue;
        }
        bool flag = true;
        for (int i = 0; i < int(str.length()); i++)
        {
            if (str[i] == '(' || str[i] == '[' || str[i] == '{' || str[i] == '<')
            {
                s.push(str[i]);
            } else if (str[i] == ')' && !s.empty())
            {
                if (s.top() == '(') s.pop();
                else { s1.push(str[i]); }
            } else if (str[i] == ']' && !s.empty())
            {
                if (s.top() == '[') s.pop();
                else { s1.push(str[i]); }
            } else if (str[i] == '}' && !s.empty())
            {
                if (s.top() == '{') s.pop();
                else { s1.push(str[i]); }
            } else if (str[i] == '>' && !s.empty())
            {
                if (s.top() == '<') s.pop();
                else { s1.push(str[i]); }
            } else
            {
                flag = false;
            }
        }
        if (s.empty() && s1.empty() && flag) cout << "Yes" << endl;
        else cout << "No" << endl;
    }
}
