// 02.开关与灯.cpp
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    int n, m;
    cin >> n >> m;

    vector<string> switches(n);
    for (int i = 0; i < n; i++)
    {
        cin >> switches[i];
    }

    // 统计每个灯有多少个开关可以打开它
    vector<int> light_count(m, 0);
    for (int j = 0; j < m; j++)
    {
        for (int i = 0; i < n; i++)
        {
            if (switches[i][j] == '1')
            {
                light_count[j]++;
            }
        }
    }

    // 检查每个开关是否可以被忽略
    for (int i = 0; i < n; i++)
    {
        bool can_ignore = true;

        // 检查这个开关能打开的每个灯
        for (int j = 0; j < m; j++)
        {
            if (switches[i][j] == '1')
            {
                // 如果这个灯只有当前开关能打开，那么不能忽略这个开关
                if (light_count[j] == 1)
                {
                    can_ignore = false;
                    break;
                }
            }
        }

        if (can_ignore)
        {
            cout << "YES" << endl;
            return 0;
        }
    }

    cout << "NO" << endl;
    return 0;
}

/*
*【问题描述】

给定n个开关和m个灯，第i个开关只能打开部分灯。矩阵a包含n行m列，当aij=1时表示开关i可以打开灯j，否则aij=0。

开始时所有的m个灯都是关着的。

开关只能从状态"关"到"开"。这意味着，对于每个可以打开某个灯的开关，无论你按多少次，这个灯都是开的。

确保当你按下所有开关时，所有的灯都能打开，考虑是否可以忽略其中某个开关也能打开所有的灯。

你的任务是确定是否存在这样的开关可以忽略，而使用其余的n-1个开关来打开所有m个灯。

【输入形式】

输入第1行包含两个整数n和m(1<=n, m<=2000)，表示开关的数量和灯的数量。

接下来的n行，每行包含m个字符，字符aij=1时表示开关i可以打开灯j，否则aij=0。

【输出形式】

如果存在这样的可以忽略的开关，而使用其他n-1个开关打开所有的m个灯，输出"YES"，否则输出"NO"。

【样例输入】

4 5
10101
01000
00111
10000

【样例输出】

YES*/
