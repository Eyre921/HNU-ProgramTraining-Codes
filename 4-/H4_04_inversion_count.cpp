/*
4. 逆序数
问题描述

在一个排列中，如果一对数的前后位置与大小顺序相反，即前面的数大于后面的数，那么它们就称为一个逆序。一个排列中逆序的总数就称为这个排列的逆序数。也就是说，对于n个不同的元素，先规定各元素之间有一个标准次序（例如n个 不同的自然数，可规定从小到大为标准次序），于是在这n个元素的任一排列中，当某两个元素的先后次序与标准次序不同时，就说有1个逆序。一个排列中所有逆序总数叫做这个排列的逆序数。

比如：

数列 1 7 3 5 4 8 9

其中（7,3），（7,5），（7,4），（5,4）构成逆序，所以其逆序数为4。

对给定的数列，求出其逆序数。

输入格式

有多组测试数据。

每组测试数据第一行是一个正整数N，表示数列中元素个数，接下来一行N个用空格分隔开的正整数，表示数列的N个元素，数列元素值小于32768，并且一个数列中没有两个数值相同。

N=0表示输入结束，并且不需要处理。

40%的数列元素个数N 1 ≤ N≤ 10；

30%的数列元素个数N 1 ≤ N≤ 100；

20%的数列元素个数N 1 ≤ N≤ 1000；

10%的数列元素个数N 1 ≤ N≤ 5000；

输出格式

  　　对于每组测试数据，输出一个整数：数列的逆序数。

样例输入


7
1 7 3 5 4 8 9
4
1 2 3 4
0
样例输出


4
0
*/
/*
 * 核心思路:
 * 本题要求计算一个排列的逆序数。逆序对是指在数组中，下标 i < j，但值 arr[i] > arr[j] 的一对元素。
 * 我们采用“分治法”（Divide and Conquer），这正是归并排序的核心思想。
 *
 * 1. 分解 (Divide):
 * 将待处理的序列递归地一分为二，直到每个子序列只包含一个元素。单个元素的子序列其逆序数为0。
 *
 * 2. 解决 (Conquer):
 * 一个序列的逆序数 = 左子序列的逆序数 + 右子序列的逆序数 + 跨越左右子序列的逆序数。
 * 左右子序列的逆序数可以通过递归调用得到。
 *
 * 3. 合并 (Combine):
 * 最关键的一步是在合并两个已排序的子序列时，计算“跨越”的逆序数。
 * 假设我们有两个已排序的子序列 L 和 R。我们使用双指针 i 和 j 分别指向 L 和 R 的头部。
 * - 当 L[i] <= R[j] 时, L[i] 不可能与 R 中任何剩余元素构成逆序对，我们将 L[i] 放入结果数组。
 * - 当 L[i] > R[j] 时, 这意味着 R[j] 小于 L 中从 i 开始的所有元素。因为 L 是有序的，
 * 所以 L[i], L[i+1], ... 等都大于 R[j]。
 * 这就构成了 (mid - i + 1) 个逆序对。我们累加这个数目，然后将 R[j] 放入结果数组。
 *
 * 通过在归并排序的“合并”阶段增加计数逻辑，我们可以在不增加额外时间复杂度的情况下，高效地计算出总逆序数。
 *
 * 时间复杂度: O(N log N)
 * 归并排序的递归深度是 O(log N)，每层合并操作的时间复杂度是 O(N)。
 *
 * 空间复杂度: O(N)
 * 归并排序需要一个与原数组等大的辅助数组来暂存合并结果。
 */

#include <iostream>
#include <vector>

// 遵循规范，直接使用 std 命名空间
using namespace std;

// 合并两个有序子数组，并计算在合并过程中产生的逆序对数量
long long mergeAndCount(vector<int>& arr, vector<int>& temp, int left, int mid, int right) {
    int i = left;       // 左子数组的起始指针
    int j = mid + 1;    // 右子数组的起始指针
    int k = left;       // 合并后数组（暂存于temp）的起始指针
    long long inversionCount = 0;

    // 当左右子数组都还有元素时，进行比较和合并
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            // 如果左数组元素较小或相等，则不构成逆序对，将其放入临时数组
            temp[k++] = arr[i++];
        } else {
            // 核心步骤：如果右数组元素 arr[j] 较小
            // 这意味着它比左数组中从 arr[i] 到结尾的所有元素都小
            // 这些元素（共 mid - i + 1 个）都和 arr[j] 构成逆序对
            temp[k++] = arr[j++];
            inversionCount += (mid - i + 1);
        }
    }

    // 如果左子数组还有剩余元素，直接复制到临时数组末尾
    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    // 如果右子数组还有剩余元素，直接复制到临时数组末尾
    while (j <= right) {
        temp[k++] = arr[j++];
    }

    // 将排好序的临时数组内容复制回原数组的相应区间
    for (i = left; i <= right; ++i) {
        arr[i] = temp[i];
    }

    return inversionCount;
}

// 归并排序主函数，递归地分割数组并累加逆序数
long long mergeSortAndCount(vector<int>& arr, vector<int>& temp, int left, int right) {
    long long inversionCount = 0;
    // 递归终止条件：当子数组只有一个或零个元素时，逆序数为0
    if (left < right) {
        // 计算中间点，防止 (left + right) 溢出
        int mid = left + (right - left) / 2;

        // 递归计算左半部分的逆序数
        inversionCount += mergeSortAndCount(arr, temp, left, mid);

        // 递归计算右半部分的逆序数
        inversionCount += mergeSortAndCount(arr, temp, mid + 1, right);

        // 计算跨越左右两部分的逆序数，并完成归并操作
        inversionCount += mergeAndCount(arr, temp, left, mid, right);
    }
    return inversionCount;
}

int main() {
    // 提高IO性能，对于大量数据输入是必要的优化
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    // 循环处理多组测试数据，直到输入 N=0 结束
    while (cin >> n && n != 0) {
        // 使用 vector 存储数列元素，更安全、更现代
        vector<int> arr(n);
        for (int i = 0; i < n; ++i) {
            cin >> arr[i];
        }

        // 创建一个临时辅助数组，并将其引用传递给递归函数，避免在递归中反复创建销毁，优化性能
        vector<int> temp(n);

        // 调用归并排序算法计算总逆序数
        // 使用 long long 存储结果，是处理计数问题的良好习惯，可以防止潜在的整数溢出
        long long totalInversions = mergeSortAndCount(arr, temp, 0, n - 1);

        // 输出最终结果
        cout << totalInversions << endl;
    }

    // --- main 函数的测试用例说明 ---
    // 本程序根据题目要求，从标准输入动态读取测试数据。
    //
    // 样例输入1:
    // 7
    // 1 7 3 5 4 8 9
    // 对应输出: 4
    // 解释: 逆序对为 (7,3), (7,5), (7,4), (5,4)
    //
    // 样例输入2:
    // 4
    // 1 2 3 4
    // 对应输出: 0
    // 解释: 数列完全有序，没有逆序对。

    return 0;
}