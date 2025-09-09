// Original Filename: 04.字符串反转3.cpp
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

string reverseWords(string str)
{
    string result = str;
    int n = str.length();
    int start = 0;

    for (int i = 0; i <= n; i++)
    {
        // 当遇到空格或到达字符串末尾时，反转当前单词
        if (i == n || str[i] == ' ')
        {
            if (i > start)
            {
                // 反转从start到i-1的字符
                reverse(result.begin() + start, result.begin() + i);
            }
            start = i + 1; // 下一个单词的开始位置
        }
    }

    return result;
}

int main()
{
    int n;
    cin >> n;
    cin.ignore(); // 忽略换行符

    for (int i = 0; i < n; i++)
    {
        string line;
        getline(cin, line);
        cout << reverseWords(line) << endl;
    }

    return 0;
}

/*
*【问题描述】

       给出一个字符串，请将其每个单词反转后输出。

【输入形式】

      输入第一行为一个正整数N，表示测试用例数，接下来的N行，每行一个字符串。

【输出形式】

      输出N行，每行对应一个反转后的字符串。

【样例输入】

3
olleh !dlrow
m'I morf .unh
I ekil .tae
【样例输出】

hello world!
I'm from hnu.
I like eat.

 */
