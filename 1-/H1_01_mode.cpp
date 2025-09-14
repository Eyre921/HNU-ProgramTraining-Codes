// 众数
#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int n;
    while (cin >> n && n != 0)
    {
        vector<int> data(n);
        for (int i = 0; i < n; i++)
        {
            cin >> data[i];
        }
        int mode = data[0]; // 当前众数
        int max_count = 1; // 最大出现次数
        int current_count = 1; // 当前数字的出现次数
        // 遍历数组，统计每个数字的连续出现次数
        for (int i = 1; i < n; i++)
        {
            if (data[i] == data[i - 1])
            {
                // 如果当前数字与前一个相同，增加计数
                current_count++;
            } else
            {
                // 如果当前数字与前一个不同，检查是否需要更新众数

                if (current_count > max_count)
                {
                    max_count = current_count;
                    mode = data[i - 1];
                }
                current_count = 1; // 重置计数
            }
        }
        // 检查最后一组数字
        if (current_count > max_count)
        {
            mode = data[n - 1];
        }
        cout << mode << endl;
    }
    return 0;
}

/*
    * 这段代码实现了众数的计算。它首先读取一个整数n，表示数据的个数，然后读取n个整数存储在一个向量中。
    * 接着，它遍历这个向量，统计每个数字的连续出现次数，并更新当前众数和最大出现次数。
    * 最后，它输出众数的值。
 */
/*
【问题描述】

一组数据中出现最多的数，称为众数。比如

1 2 3 3

众数为3。一组数据中也可能有多个众数，以最先出现的作为众数。比如

2 2 3 3

众数为2。

   问题是一组按升序排好的数据，指出它的众数。

【输入形式】

有多组测试数据（不超过100组测试数据）。

每组测试数据占两行，第一行是正整数N：表示这组测试数据中数据项数。

第二行是N个用空格隔开的正整数，表示这组测试数据的数据元素。每个数据元素都不大于10000。

N=0，表示输入结束，并且不需要处理。

测试数据N 1 < N≤ 10000；

【输出形式】

对于每组测试数据，输出一行包含一个正整数：对应的众数。

【样例输入】


4
1 2 3 3
4
2 2 3 3
0
【样例输出】


3
2
 */
