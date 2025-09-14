/*
 *
4. 公交系统
【问题描述】

        城市公交系统有一个记录仪，用于记录每个站点的乘客人数的变化情况，例如：x表示到站前公交车上的乘客人数，y表示离站时公交车上的乘客人数，则该记录仪记录的该站的数字为y-x。

        对于一辆公交车和n个车站，a1,a2,...,an为该公交车在各站的记录数据。

        假定w为该公交车可容纳的最大乘客人数，编程求出在第一站停靠之前公交车上人数的可能数据有多少种？

【输入形式】

        第一行包含两个数据n和w(1<=n<=1000, 1<=w<=109)，分别表示车站的数目和公交车可容纳的最大乘客人数。

        第二行包含一个序列a1,a2,...,an，表示记录仪记录的各站的数据。

【输出形式】

        输出一个整数，表示公交车在第一站停靠之前可能的乘客人数数据的个数，如果没有，则输出0。

【样例输入1】

3 5
2 1 -3
【样例输出1】

3
【样例输入2】

2 4
-1 1
【样例输出2】

4
【样例输入3】

4 10
2 4 1 2
【样例输出3】

2
【样例说明】

在第一个样例中，乘客数可能有0、1、2，共3种情况
在第二个样例中，乘客数可能有1、2、3、4，共4种情况
在第三个样例种，乘客数可能为0或1，共2种情况
 */

/**
 * @file Bus_System.cpp
 * @brief 计算公交车在始发前可能的乘客数。
 *
 * 核心思路:
 * 这是一个典型的区间约束问题。设公交车在第一站停靠前的初始乘客人数为 p0。
 * 经过第 i 站后，车上的人数将变为 p0 + a1 + a2 + ... + ai。
 * 我们定义前缀和 Si = a1 + a2 + ... + ai (并设 S0 = 0)。
 * 则经过第 i 站后的人数为 p0 + Si。
 *
 * 根据题意，在任何时刻，车上的乘客人数必须在 [0, w] 区间内，w是最大容量。
 * 这意味着对于所有的 i (从0到n)，都必须满足以下不等式：
 * 0 <= p0 + Si <= w
 *
 * 对这个不等式进行变换，我们可以得到对 p0 的约束：
 * -Si <= p0 <= w - Si
 *
 * 为了让 p0 满足所有时刻 (i=0 to n) 的约束，p0 必须大于等于所有下界中的最大值，
 * 并且小于等于所有上界中的最小值。
 * max(-S0, -S1, ..., -Sn) <= p0 <= min(w-S0, w-S1, ..., w-Sn)
 *
 * 这可以进一步简化为：
 * -min(S0, S1, ..., Sn) <= p0 <= w - max(S0, S1, ..., Sn)
 *
 * 我们令 minPrefixSum = min(S0, ..., Sn)，maxPrefixSum = max(S0, ..., Sn)。
 * 则 p0 的有效范围是 [ -minPrefixSum, w - maxPrefixSum ]。
 *
 * 算法流程：
 * 1. 遍历各站的数据 a_i，动态计算前缀和，并记录下前缀和序列中的最小值(minPrefixSum)和最大值(maxPrefixSum)。
 * 2. 根据上述公式计算出 p0 的下界 `lowerBound = -minPrefixSum` 和上界 `upperBound = w - maxPrefixSum`。
 * 3. 如果 `lowerBound > upperBound`，说明不存在有效的 p0，输出0。
 * 4. 否则，有效 p0 的数量就是这个闭区间内的整数个数，即 `upperBound - lowerBound + 1`。
 *
 * 时间复杂度: O(N)
 * 我们只需要一次遍历所有车站数据即可计算出前缀和的最大与最小值。
 *
 * 空间复杂度: O(1)
 * 我们不需要存储整个前缀和数组，只需要常数个变量来记录当前和、最小和与最大和。
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

// 使用标准命名空间，简化代码
using namespace std;

int main() {
    // 加速C++ I/O操作
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // n: 车站数目, w: 公交车最大容量
    int n;
    long long w; // w 很大，使用 long long
    cin >> n >> w;

    long long currentPrefixSum = 0;
    // minPrefixSum 初始化为0，因为 S0 = 0 是前缀和序列的第一个值
    long long minPrefixSum = 0;
    // maxPrefixSum 初始化为0，同上
    long long maxPrefixSum = 0;

    // 遍历n个车站的数据
    for (int i = 0; i < n; ++i) {
        long long passengerChange;
        cin >> passengerChange;

        // 动态计算前缀和
        currentPrefixSum += passengerChange;

        // 在遍历过程中实时更新前缀和的最大值和最小值
        minPrefixSum = min(minPrefixSum, currentPrefixSum);
        maxPrefixSum = max(maxPrefixSum, currentPrefixSum);
    }

    // 根据推导出的公式计算初始乘客数 p0 的下界
    // p0 必须足够大，以保证在乘客数减少最多的站点也不会变为负数
    long long lowerBound = -minPrefixSum;

    // 计算初始乘客数 p0 的上界
    // p0 必须足够小，以保证在乘客数增加最多的站点也不会超过容量 w
    long long upperBound = w - maxPrefixSum;

    // 如果下界大于上界，说明不存在任何有效的初始乘客数
    if (lowerBound > upperBound) {
        cout << 0 << endl;
    } else {
        // 否则，可能的初始乘客数就是闭区间 [lowerBound, upperBound] 内的整数个数
        long long possibleValuesCount = upperBound - lowerBound + 1;
        cout << possibleValuesCount << endl;
    }

    return 0;
}