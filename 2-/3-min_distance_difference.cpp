/*
 *
* 3. 数的距离差
【问题描述】

给定一组正整数，其中最大值和最小值分别为Max和Min, 其中一个数x到Max和Min的距离差定义为：

      abs(abs(x-Max)-(x-Min))

其中abs()为求一个数的绝对值

【输入形式】

包括两行，第一行一个数n，表示第二行有n个正整数

【输出形式】

输出一个数x，该数在所有n个数中的距离差最小；如果有两个数的距离差都是最小，输出较小的哪个

【样例输入1】

5
3 1 7 5 9
【样例输出1】

5
【样例输入2】

3
1 3 2
【样例输出2】

2
 */

/**
 * @file min_distance_difference.cpp
 * @brief 解决方案：寻找距离差最小的数
 *
 * 核心思路:
 * 1.  公式简化:
 * 题目给出的距离差公式为 `abs(abs(x-Max) - abs(x-Min))`。
 * 由于x是数组中的数，必然满足 `Min <= x <= Max`。
 * 因此，`x-Max` <= 0, `x-Min` >= 0。
 * 所以 `abs(x-Max)` 等价于 `Max-x`，`abs(x-Min)` 等价于 `x-Min`。
 * 公式可以简化为 `abs((Max - x) - (x - Min))`
 * 进一步化简为 `abs(Max - x - x + Min)`
 * 最终得到 `abs((Max + Min) - 2 * x)`。
 * 这个简化的公式避免了多次调用`abs`，计算更高效。
 *
 * 2.  算法流程:
 * a. 读取所有输入的整数，并存储在一个 `std::vector` 中。
 * b. 使用STL中的 `*std::max_element` 和 `*std::min_element` 高效地找出数组中的最大值(Max)和最小值(Min)。
 * c. 初始化一个变量 `minDistanceDiff` 记录全局最小距离差（初值为极大值），以及一个变量 `resultNumber` 记录结果。
 * d. 遍历整个数组，对于每个数 `x`，使用简化后的公式计算其距离差 `currentDiff`。
 * e. 比较 `currentDiff` 和 `minDistanceDiff`：
 * - 如果 `currentDiff < minDistanceDiff`，说明找到了一个更优的解，此时更新 `minDistanceDiff = currentDiff` 并且 `resultNumber = x`。
 * - 如果 `currentDiff == minDistanceDiff`，根据题意，需要取较小的那个数，所以更新 `resultNumber = std::min(resultNumber, x)`。
 * f. 遍历结束后，`resultNumber` 中存储的就是最终答案。
 *
 * 时间复杂度: O(N)
 * - 读取N个整数存入vector: O(N)
 * - 寻找最大值和最小值: O(N)
 * - 遍历vector计算距离差: O(N)
 * 整体线性时间复杂度。
 *
 * 空间复杂度: O(N)
 * 需要一个vector来存储N个输入的整数。
 */

#include <iostream>   // 用于标准输入输出 (cin, cout)
#include <vector>     // 用于使用vector容器
#include <algorithm>  // 用于使用 std::min, std::max_element, std::min_element
#include <cmath>      // 用于使用 std::abs
#include <limits>     // 用于使用 std::numeric_limits

// 使用标准命名空间，简化代码
using namespace std;

int main() {
    // 优化C++标准流的IO性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int numCount;
    cin >> numCount;

    // 处理边界情况：如果没有数字或只有一个数字
    if (numCount <= 0) {
        return 0; // 没有输入，直接退出
    }

    // 使用vector存储所有输入的数字
    vector<long long> numbers(numCount);
    for (int i = 0; i < numCount; ++i) {
        cin >> numbers[i];
    }

    if (numCount == 1) {
        cout << numbers[0] << endl; // 只有一个数，它本身就是答案
        return 0;
    }

    // 使用STL算法高效找到最大值和最小值
    // *max_element返回指向最大元素的迭代器，通过*解引用得到其值
    long long maxVal = *max_element(numbers.begin(), numbers.end());
    long long minVal = *min_element(numbers.begin(), numbers.end());

    // 初始化最小距离差为一个理论上的最大值
    long long minDistanceDiff = numeric_limits<long long>::max();
    // 初始化结果数字
    long long resultNumber = -1;

    // 预先计算 Max + Min 的值，避免在循环中重复计算
    long long sumOfMaxMin = maxVal + minVal;

    // 遍历所有数字，寻找距离差最小的那个
    for (long long currentNum : numbers) {
        // 使用简化后的公式计算距离差
        long long currentDiff = abs(sumOfMaxMin - 2 * currentNum);

        // 如果当前距离差更小，则更新最小距离差和结果数字
        if (currentDiff < minDistanceDiff) {
            minDistanceDiff = currentDiff;
            resultNumber = currentNum;
        }
        // 如果距离差相等，根据题意，选择数值较小的那个作为结果
        else if (currentDiff == minDistanceDiff) {
            resultNumber = min(resultNumber, currentNum);
        }
    }

    // 输出最终结果
    cout << resultNumber << endl;

    return 0;
}