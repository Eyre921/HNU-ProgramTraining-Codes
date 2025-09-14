/*
16. 小希的数表
【问题描述】

Gardon 昨天给小希布置了一道作业，即根据一张由不超过 5000 的 N(3<=N<=100)个正整数组成的数表两两相加得到 N*(N-1)/2 个和，然后再将它们排序。例如，如果数表里含有四个数 1，3，4，9，那么正确答案是 4，5，7，10，12，13。小希做完作业以后出去玩了一阵，可是下午回家时发现原来的那张数表不见了，好在她做出的答案还在，你能帮助她根据她的答案计算出原来的数表么？

【输入形式】

包含多组数据，每组数据以一个 N 开头，接下来的一行有按照大小顺序排列的 N*(N-1)/2 个数，是小希完成的答案。文件最后以一个 0 结束。
假设输入保证解的存在性和唯一性。

【输出形式】

对于每组数据，输出原来的数表。它们也应当是按照顺序排列的。

【样例输入】

4
4 5 7 10 12 13
4
5 6 7 8 9 10
0
【样例输出】

1 3 4 9
2 3 4 6
*/
/*
 * 核心思路:
 * 这是一个典型的搜索问题，需要通过给定的两两之和反推出原始的N个数。由于输入保证解的存在性和唯一性，
 * 我们可以使用带高效剪枝的回溯搜索（Backtracking/DFS）来找到这组唯一的数。
 *
 * 1. 关键洞察与约束:
 * a. 设原始数表为 a_1, a_2, ..., a_N，并已按升序排列 (a_1 < a_2 < ... < a_N)。
 * b. 给定的和数组中，最小的和 S[0] 必然是 a_1 + a_2。
 * c. 从 (b) 可以推断出 a_1 的一个严格搜索范围。因为 a_1, a_2 都是正整数且 a_1 < a_2，
 * 所以 2*a_1 < a_1 + a_2 = S[0]，即 a_1 < S[0] / 2。这为我们提供了搜索的起点。
 *
 * 2. 回溯搜索策略:
 * a. 首先，我们对 a_1 进行猜测。遍历所有可能的 a_1（从 1 到 S[0]/2 - 1）。对于每一个
 * 猜测的 a_1，我们可以立刻计算出 a_2 = S[0] - a_1。这构成了我们搜索的初始状态。
 * b. 有了 a_1, a_2 的候选值后，我们开始递归地寻找 a_3, a_4, ..., a_N。
 * c. 在寻找第 k 个数 a_k 时 (k >= 3)，我们利用一个至关重要的性质进行剪枝：
 * 在所有尚未被 {a_1, ..., a_{k-1}} 的配对和所“解释”的和中，最小的那个和必然是 a_1 + a_k。
 * 这是因为 a_1 是所有数中最小的，它与下一个未知的、最小的数 a_k 的和，一定会小于
 * 其他任何包含 a_k 的配对和 (例如 a_2 + a_k)。
 * d. 因此，我们找到当前剩余和中的最小值 S_min，就可以唯一地推断出 a_k 的候选值：
 * a_k = S_min - a_1。
 * e. **验证与剪枝**:
 * - 我们必须验证推断出的 a_k 是否大于 a_{k-1}，否则序列不是升序，此路不通，立即剪枝。
 * - 我们需要验证由 a_k 新产生的所有和 (a_1+a_k, a_2+a_k, ..., a_{k-1}+a_k)
 * 是否都能在“剩余和”的集合中找到。如果都能找到，我们就从剩余和中“划掉”它们，然后
 * 继续递归寻找 a_{k+1}。如果有一个找不到，说明我们最初对 a_1 的猜测是错误的，
 * 或者之前的某个推断是错误的，需要回溯。
 *
 * 3. 数据结构:
 * 为了高效地处理“剩余的和”（需要支持快速查找、插入和删除），使用频次图
 * (std::map<int, int>) 是一个非常理想的选择。它的键是和的值，值是该和出现的次数。
 *
 * 时间复杂度: 比较复杂，因为它是一个剪枝搜索。大致为 O(S[0] * N^2 * logM)，其中M是和的数量。
 * 在N<=100的约束下，基于 S_min = a_1 + a_k 的强力剪枝是高效解决问题的关键。
 * 空间复杂度: O(M + N)，主要用于存储和的频次图以及递归调用的栈空间。
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>

// 使用 std 命名空间，可以省略 `std::` 前缀，使代码更简洁
using namespace std;

/**
 * @brief 递归回溯函数，用于寻找数表中的第 k 个数
 * @param k 当前正在尝试确定的数字的索引 (例如，k=3 表示正在寻找 a_3)
 * @param n 原始数表的总大小
 * @param solution 当前已经找到的部分解 {a_1, a_2, ..., a_{k-1}}
 * @param sums_freq 剩余和的频次图
 * @return 如果能找到完整解则返回 true，否则返回 false
 */
bool backtrack(int k, int n, vector<int>& solution, map<int, int>& sums_freq) {
    // 基线条件：如果已经找到了 N 个数 (k > n)，说明搜索成功
    if (k > n) {
        return true;
    }

    // 在剩余的和中找到最小值 s_min。根据我们的核心洞察，这必然是 a_1 + a_k
    int s_min = sums_freq.begin()->first;

    // 由 s_min = a_1 + a_k，推断出 a_k 的唯一候选值
    int a_k_candidate = s_min - solution[0]; // solution[0] is a_1

    // 剪枝条件1：如果候选值不满足升序要求 (a_k <= a_{k-1})，则此路不通
    if (a_k_candidate <= solution.back()) {
        return false;
    }

    // 收集由 a_k 新产生的所有和
    vector<int> new_sums;
    for (int i = 0; i < k - 1; ++i) {
        new_sums.push_back(solution[i] + a_k_candidate);
    }

    // 剪枝条件2：检查这些新产生的和是否都存在于 sums_freq 中
    for (int sum_val : new_sums) {
        if (sums_freq.find(sum_val) == sums_freq.end() || sums_freq[sum_val] == 0) {
            // 如果某个需要的和不存在，则此路不通
            return false;
        }
    }

    // 验证通过，做出选择：将 a_k 加入解，并从频次图中“临时”移除新产生的和
    solution.push_back(a_k_candidate);
    for (int sum_val : new_sums) {
        sums_freq[sum_val]--;
        if (sums_freq[sum_val] == 0) {
            sums_freq.erase(sum_val);
        }
    }

    // 进入下一层递归，寻找 a_{k+1}
    if (backtrack(k + 1, n, solution, sums_freq)) {
        return true; // 成功找到完整解，一路返回true
    }

    // 回溯：如果后续的搜索失败，说明当前的选择是错误的，需要撤销
    // 1. 将移除的和加回去
    for (int sum_val : new_sums) {
        sums_freq[sum_val]++;
    }
    // 2. 将 a_k 从解中移除
    solution.pop_back();

    return false; // 明确返回此路不通
}

// 主解决函数
void solve(int n, const vector<int>& initial_sums) {
    // 获取最小的和 S[0] = a_1 + a_2
    int s0 = initial_sums[0];

    // 遍历 a_1 的所有可能性，这是整个搜索的起点
    for (int a1_candidate = 1; a1_candidate * 2 < s0; ++a1_candidate) {
        vector<int> solution;
        solution.push_back(a1_candidate);

        int a2_candidate = s0 - a1_candidate;
        solution.push_back(a2_candidate);

        // 将所有和存入频次图，方便后续查询和删除
        map<int, int> sums_freq;
        for (int s : initial_sums) {
            sums_freq[s]++;
        }

        // 从频次图中移除已被解释的和: a_1 + a_2
        sums_freq[s0]--;
        if (sums_freq[s0] == 0) {
            sums_freq.erase(s0);
        }

        // 从 a_3 开始进行回溯搜索
        if (backtrack(3, n, solution, sums_freq)) {
            // 由于解唯一，找到后即可输出并结束
            for (int i = 0; i < n; ++i) {
                cout << solution[i] << (i == n - 1 ? "" : " ");
            }
            cout << endl;
            return;
        }
    }
}

int main() {
    // 优化 I/O 性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    while (cin >> n && n != 0) {
        int m = n * (n - 1) / 2;
        vector<int> sums(m);
        for (int i = 0; i < m; ++i) {
            cin >> sums[i];
        }
        solve(n, sums);
    }

    return 0;
}