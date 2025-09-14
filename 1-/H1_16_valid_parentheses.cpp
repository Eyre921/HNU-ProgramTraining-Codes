// Original Filename: 16.合法的括号串.cpp
#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    cin >> n;
    while (n--)
    {
        stack<char> s; // 存储开括号
        stack<char> s1; // 存储无法匹配的闭括号
        string str;
        cin >> str;
        int len = str.length();

        for (int i = 0; i < len; i++)
        {
            if (str[i] == '<' || str[i] == '(' || str[i] == '[' || str[i] == '{')
            {
                s.push(str[i]);
            } else if (str[i] == '>')
            {
                if (!s.empty() && s.top() == '<')
                {
                    s.pop();
                } else
                {
                    s1.push(str[i]);
                }
            } else if (str[i] == ')')
            {
                if (!s.empty() && s.top() == '(')
                {
                    s.pop();
                } else
                {
                    s1.push(str[i]);
                }
            } else if (str[i] == ']')
            {
                if (!s.empty() && s.top() == '[')
                {
                    s.pop();
                } else
                {
                    s1.push(str[i]);
                }
            } else if (str[i] == '}')
            {
                if (!s.empty() && s.top() == '{')
                {
                    s.pop();
                } else
                {
                    s1.push(str[i]);
                }
            }
        }

        bool b = s.empty(), d = s1.empty();
        if (b && d)
        {
            cout << "Yes" << endl;
        } else
        {
            cout << "No" << endl;
        }
    }
    return 0;
}
