/*
2. 数列再分段
【问题描述】

对于给定的一个长度为 N 的正整数数列,现要将其分成 M 段,并要求每段连续，且每段和的最大值最小。

【输入形式】

第 1 行包含两个正整数 N、M；

第 2 行包含 N 个空格隔开的非负整数,为数列的元素。

【输出形式】

仅包含一个正整数，即每段和最大值最小为多少。

对于20%的数据，有 N<=10；


对于40%的数据，有 N<=1000；


对于100%的数据，有 N<=105, M<=N，数列元素之和不超过109。

【样例输入】


5 3
4 2 4 5 1
【样例输出】

6
【出题人】

拔尖班2022级周新城
 */
/*
 * 核心思路:
 * 这个问题要求我们将一个连续的数列分成M段，并使得各段之和的最大值最小。这是一个典型的“最大值最小化”问题，
 * 通常采用二分答案的策略来解决。
 *
 * 1. 二分答案:
 * 我们可以对“每段和的最大值”进行二分查找。假设我们猜测一个最大值 `x`，然后我们需要验证是否能将
 * 数列分成不超过 M 段，且每一段的和都不超过 `x`。
 *
 * 2. 验证函数 (check):
 * 验证过程可以采用贪心策略。我们遍历数列，尽可能地将元素放入当前段，直到再加入下一个元素就会
 * 超过 `x`。当无法再加入元素时，就结束当前段，并开启一个新段。
 * - 维护一个 `currentSum` 记录当前段的和，以及一个 `segmentCount` 记录已经分了多少段。
 * - 遍历数组，对于每个元素 `a[i]`：
 * - 如果 `currentSum + a[i] <= x`，则将其加入当前段，即 `currentSum += a[i]`。
 * - 否则，说明当前段已满，必须开启新段。令 `segmentCount++`，并让新段从 `a[i]` 开始，
 * 即 `currentSum = a[i]`。
 * - 遍历结束后，得到的 `segmentCount` 就是在最大和为 `x` 的限制下，最少需要分成的段数。
 * - 如果 `segmentCount <= M`，说明猜测的 `x` 是可行的（或者太大了），我们可以尝试一个更小的 `x`。
 * - 如果 `segmentCount > M`，说明猜测的 `x` 太小了，导致分出了太多的段，需要增大 `x`。
 *
 * 3. 确定二分范围:
 * - 搜索下界 (left): 数组中单个元素的最大值。因为每个元素至少要自成一段，所以最大和不能小于任何一个元素。
 * - 搜索上界 (right): 数组所有元素的总和。这是最宽松的情况，即只分成一段（M=1）。
 *
 * 最终，二分查找会收敛于满足条件的最小的那个 `x`。
 *
 * 时间复杂度: O(N * log(Sum))
 * - 其中 N 是数列的长度，Sum 是数列所有元素的总和。
 * - 每次 `check` 函数需要 O(N) 的时间。二分查找需要 O(log(Sum)) 次。
 *
 * 空间复杂度: O(N)
 * - 主要用于存储输入的数列。
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

// 验证函数：检查在给定的最大段和 `maxSum` 下，是否可以将数列分成不超过 `maxSegments` 段
bool canPartition(long long maxSum, int maxSegments, const vector<int>& nums) {
    int segmentsNeeded = 1; // 至少需要1段
    long long currentSegmentSum = 0;

    // 遍历数列中的每个数
    for (int num : nums) {
        // 单个元素如果就大于maxSum，则此maxSum不可行
        if (num > maxSum) {
            return false;
        }

        // 如果当前数加入后，当前段的和会超过限制
        if (currentSegmentSum + num > maxSum) {
            // 需要开启一个新段
            segmentsNeeded++;
            // 新段的和从当前数开始
            currentSegmentSum = num;
        } else {
            // 否则，将当前数加入当前段
            currentSegmentSum += num;
        }
    }

    // 返回需要的段数是否在允许范围内
    return segmentsNeeded <= maxSegments;
}

int main() {
    // 优化输入输出，提高效率
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n; // 数列长度
    int m; // 要分成的段数
    cin >> n >> m;

    vector<int> nums(n);
    long long totalSum = 0; // 数列总和，作为二分上界
    int maxElement = 0;     // 数列中最大元素，作为二分下界

    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
        totalSum += nums[i];
        maxElement = max(maxElement, nums[i]);
    }

    // --- 二分查找答案 ---
    long long left = maxElement;  // 搜索区间的左边界
    long long right = totalSum;   // 搜索区间的右边界
    long long minMaxSum = right; // 存储最终结果，初始化为最大可能值

    while (left <= right) {
        // 计算中间值，防止溢出
        long long mid = left + (right - left) / 2;

        // 检查以 mid 作为每段最大和，是否可行
        if (canPartition(mid, m, nums)) {
            // 如果可行，说明 mid 可能就是答案，或者答案在更小的范围里
            minMaxSum = mid;       // 记录下这个可行的答案
            right = mid - 1;     // 尝试在左半部分寻找更小的可行值
        } else {
            // 如果不可行，说明 mid 太小了，需要增大
            left = mid + 1;      // 在右半部分寻找
        }
    }

    // 输出找到的最小的最大和
    cout << minMaxSum << endl;

    return 0;
}

