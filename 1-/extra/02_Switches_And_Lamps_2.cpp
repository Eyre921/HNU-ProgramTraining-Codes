// 开关与灯
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n = 0, m;
    cin >> n >> m;
    vector<string> switches(n);
    vector<int> light(m, 0); // 初始化灯的状态为关闭
    bool can_ignore = 0;
    for (int i = 0; i < n; i++)
    {
        cin >> switches[i];
        for (int j = 0; j < m; j++)
        {
            if (switches[i][j] == '1') light[j]++;
        }
    }
    // 尝试忽略一个灯
    for (int i = 0; i < n; i++)
    {
        can_ignore = true;
        for (int j = 0; j < m; j++)
        {
            if (switches[i][j] == '1')
            {
                // 如果这个开关能打开的灯只有它一个开关能打开
                if (light[j] == 1)
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
