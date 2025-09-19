/*
 * 核心思路:
 * 本题要求计算满足特定条件的经营序列总数。设第一天的营业额为 d1，之后 n-1 天的每日变化量为 delta_i (取值为 a 或 -b)。
 * 第 i 天的营业额可以表示为 d_i = d1 + delta_2 + ... + delta_i。
 * 将所有天的营业额加起来，总额 s 可以表示为：
 * s = n * d1 + (n-1)*delta_2 + (n-2)*delta_3 + ... + 1*delta_n
 *
 * 令 C = (n-1)*delta_2 + ... + 1*delta_n。则 s = n * d1 + C。
 * 由于 d1 可以是任意整数，要使得 d1 有整数解，当且仅当 (s - C) 能够被 n 整除。
 * 这等价于 (s - C) % n == 0，即 C % n == s % n。
 *
 * 问题因此转化为：有多少种不同的 delta 序列（由 n-1 个 a 或 -b 组成），使得其加权和 C 对 n 取模的结果等于 s 对 n 取模的结果。
 * 这是一个典型的动态规划问题。
 *
 * 状态定义:
 * dp[k][j] 表示在考虑了前 k 个变化量（delta_2 到 delta_{k+1}）之后，其加权和对 n 取模结果为 j 的方案数。
 *
 * 状态转移:
 * 当我们考虑第 k 个变化量 delta_{k+1} 时，它在加权和 C 中的系数为 (n-k)。
 * 这个变化量可以是 a 或 -b。
 * 1. 如果 delta_{k+1} = a，它对 C 的贡献是 (n-k)*a。
 * 2. 如果 delta_{k+1} = -b，它对 C 的贡献是 (n-k)*(-b)。
 *
 * dp[k][j] 的值可以从 dp[k-1] 的状态转移而来。对于 dp[k-1] 中模为 j_prev 的每一种方案，
 * 加上第 k 项的贡献后，新的模将变为 (j_prev + (n-k)*a) % n 或 (j_prev + (n-k)*(-b)) % n。
 *
 * 转移方程:
 * dp[k][j] = sum(dp[k-1][j_prev]) for all j_prev such that (j_prev + (n-k)*a) % n == j OR (j_prev + (n-k)*(-b)) % n == j
 *
 * 最终答案就是 dp[n-1][(s % n + n) % n]。
 *
 * 空间优化:
 * 由于 dp[k] 只依赖于 dp[k-1]，我们可以使用滚动数组（两个一维数组）来将空间复杂度从 O(n^2) 优化到 O(n)。
 *
 * 时间复杂度: O(n^2)，因为有 n-1 轮状态转移，每轮需要遍历 n 个可能的余数。
 * 空间复杂度: O(n)，使用了滚动数组优化。
 */

#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

void solve() {
    long long n, s, a, b;
    cin >> n >> s >> a >> b;

    const int MOD = 100000007;

    // dp[j] 表示当前加权和模n为j的方案数
    vector<long long> dp(n, 0);

    // 初始状态：在进行任何变化之前（k=0），加权和为0，方案数为1
    dp[0] = 1;

    // 进行 n-1 轮状态转移，对应 delta_2 到 delta_n
    for (int k = 1; k < n; ++k) {
        vector<long long> next_dp(n, 0);

        // 计算当前变化量 delta_{k+1} 的系数
        long long coefficient = n - k;

        // 计算选择 a 和 -b 对余数的贡献
        long long remainder_a = (coefficient * a) % n;
        long long remainder_b_ll = coefficient * (-b); // 使用long long防止溢出
        // 确保余数为正
        long long remainder_b = (remainder_b_ll % n + n) % n;

        // 遍历上一轮的所有可能余数
        for (int j = 0; j < n; ++j) {
            if (dp[j] == 0) continue; // 如果方案数为0，则跳过

            // 转移到下一状态
            // 1. 选择 +a
            int next_rem_a = (j + remainder_a) % n;
            next_dp[next_rem_a] = (next_dp[next_rem_a] + dp[j]) % MOD;

            // 2. 选择 -b
            int next_rem_b = (j + remainder_b) % n;
            next_dp[next_rem_b] = (next_dp[next_rem_b] + dp[j]) % MOD;
        }
        // 使用新状态覆盖旧状态，完成滚动
        dp = next_dp;
    }

    // 最终需要的 C 的余数
    long long target_remainder = (s % n + n) % n;

    // 输出结果
    cout << dp[target_remainder] << endl;
}

int main() {
    // 提高cin/cout效率
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 主逻辑函数调用，本题只有一组输入
    // solve();

    // --- 为了演示清晰的测试用例，这里 hardcode 样例 ---
    // 样例输入： 4 10 2 3
    // 预期输出： 2
    // long long n = 4, s = 10, a = 2, b = 3;
    // const int MOD = 100000007;
    // vector<long long> dp(n, 0);
    // dp[0] = 1;
    //
    // for (int k = 1; k < n; ++k) {
    //     vector<long long> next_dp(n, 0);
    //     long long coefficient = n - k;
    //     long long remainder_a = (coefficient * a) % n;
    //     long long remainder_b = ((coefficient * -b) % n + n) % n;
    //
    //     for (int j = 0; j < n; ++j) {
    //         if (dp[j] > 0) {
    //             next_dp[(j + remainder_a) % n] = (next_dp[(j + remainder_a) % n] + dp[j]) % MOD;
    //             next_dp[(j + remainder_b) % n] = (next_dp[(j + remainder_b) % n] + dp[j]) % MOD;
    //         }
    //     }
    //     dp = next_dp;
    // }
    //
    // long long target_remainder = (s % n + n) % n;
    // cout << "--- 样例测试 ---" << endl;
    // cout << "输入: n=" << n << ", s=" << s << ", a=" << a << ", b=" << b << endl;
    // cout << "输出: " << dp[target_remainder] << endl;


    // 实际提交时应使用以下代码读取输入

    long long n_in, s_in, a_in, b_in;
    if (cin >> n_in >> s_in >> a_in >> b_in) {
        const int MOD_IN = 100000007;
        vector<long long> dp_in(n_in, 0);
        dp_in[0] = 1;
        for (int k = 1; k < n_in; ++k) {
            vector<long long> next_dp_in(n_in, 0);
            long long coefficient = n_in - k;
            long long remainder_a = (coefficient * a_in) % n_in;
            long long remainder_b = ((coefficient * -b_in) % n_in + n_in) % n_in;
            for (int j = 0; j < n_in; ++j) {
                if (dp_in[j] > 0) {
                    next_dp_in[(j + remainder_a) % n_in] = (next_dp_in[(j + remainder_a) % n_in] + dp_in[j]) % MOD_IN;
                    next_dp_in[(j + remainder_b) % n_in] = (next_dp_in[(j + remainder_b) % n_in] + dp_in[j]) % MOD_IN;
                }
            }
            dp_in = next_dp_in;
        }
        long long target_rem_in = (s_in % n_in + n_in) % n_in;
        cout << dp_in[target_rem_in] << endl;
    }



    return 0;
}
