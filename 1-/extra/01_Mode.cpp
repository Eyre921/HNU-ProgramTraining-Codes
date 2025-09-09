// 题目：求众数 更快更省空间，无需存储
#include <iostream>
using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    while (cin >> n && n != 0)
    {
        if (n == 0) continue;
        int prev, cur;
        cin >> prev;
        int mode = prev, max_count = 0, cur_count = 1;
        for (int i = 1; i < n; i++)
        {
            cin >> cur;
            if (prev == cur)
            {
                cur_count++;
                if (cur_count > max_count)
                {
                    max_count = cur_count;
                    mode = max(mode, cur);
                }
            } else
            {
                cur_count = 1;
            }
            prev = cur;
        }
        cout << mode << endl;
    }
}
