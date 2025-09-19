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
/*
 *
16. 合法的括号串
【问题描述】

一个合法的括号串，是指只包含括号的串，如果满足如下条件：

（1）<> () [] {} 这四对括号是合法的；

（2）如果r是合法括号串，则<r> (r) [r] {r}也是；

（3）如果r，s是合法括号串，则rs也是；

所以<<>> , [<>{}(())],[({<>})]是合法的括号串，而)(,[( ])就不是。

【输入形式】

输入第一行正整数t (10 ≤ n ≤ 100)，表示有多少组测试数据。

后面有t行，每行一个只包含8种括号符号的括号串。

40%的括号串的长度L 2 ≤ L≤ 20；

30%的括号串的长度L 2 ≤ L≤ 200；

20%的括号串的长度L 2 ≤ L≤ 2000；

10%的括号串的长度L 2 ≤ L≤ 20000；

【输出形式】

对于每组测试数据，如果括号串是合法的，输出“Yes”（输出没有引号）占一行，否则，输出“No”（输出没有引号）占一行。

【样例输入】


6
<<>>
)(
[<>{}(())]
[({<>})]
[(])
<([{
【样例输出】


Yes
No
Yes
Yes
No
No
 */