/*
16. 最大报销额
【问题描述】现有一笔经费可以报销一定额度的发票。允许报销的发票类型包括买图书（A类）、文具（B类）、差旅（C类），要求每张发票的总额不得超过1000元，每张发票上，单项物品的价值不得超过600元。现请你编写程序，在给出的一堆发票中找出可以报销的、不超过给定额度的最大报销额。

【输入形式】测试输入包含若干测试用例。每个测试用例的第1行包含两个正数 Q 和 N，其中 Q 是给定的报销额度，N（N<=30）是发票张数。随后是 N 行输入，每行的格式为：
      m Type_1:price_1 Type_2:price_2 … Type_m:price_m
      其中正整数 m 是这张发票上所开物品的件数，Type_i 和 price_i 是第 i 项物品的种类和价值。物品种类用一个大写英文字母表示。当N为0时，全部输入结束，相应的结果不要输出。

【输出形式】对每个测试用例输出1行，即可以报销的最大数额，精确到小数点后2位。

【样例输入】

200.00 3
2 A:23.50 B:100.00
1 C:650.00
3 A:59.99 A:120.00 X:10.00
1200.00 2
2 B:600.00 A:400.00
1 C:200.50
1200.50 3
2 B:600.00 A:400.00
1 C:200.50
1 A:100.00
100.00 0

【样例输出】

123.50
1000.00
1200.50
 */
/**
 * @file Maximum_Reimbursement.cpp
 * @brief 在满足一系列规则的发票中，找出不超过报销额度的最大总金额。
 *
 * 核心思路:
 * 这个问题可以分解为两个主要步骤：
 * 1.  发票筛选：首先，我们需要遍历所有N张发票，根据题目给出的三条规则筛选出所有“有效”的发票，
 * 并计算它们的总金额。
 * - 规则a: 单张发票总额不超过1000元。
 * - 规则b: 单项物品价值不超过600元。
 * - 规则c: 物品种类必须是 'A', 'B', 或 'C'。
 *
 * 2.  0/1背包问题：筛选出有效发票及其金额后，问题转化为一个经典的0/1背包问题。
 * - 背包容量 (W): 给定的报销额度 Q。
 * - 物品 (Items): 每一张有效的发票。
 * - 物品的“重量” (weight): 发票的金额。
 * - 物品的“价值” (value): 也是发票的金额（因为目标是最大化报销总额）。
 * 我们需要从这些“物品”（有效发票）中挑选一部分，使得它们的“重量”之和（总金额）不超过
 * 背包容量Q，同时“价值”之和（总金额）最大。
 *
 * 技术细节：
 * -   处理金额：由于金额涉及小数，直接使用`double`进行DP状态转移可能会有精度问题。
 * 标准做法是将所有金额乘以100，转换为整数“分”来处理，可以完全避免精度误差。
 * -   DP状态定义：我们定义 `dp[j]` 为报销额度为 `j` 分时，可以达到的最大报销金额。
 * -   DP状态转移方程：`dp[j] = max(dp[j], dp[j - amount] + amount)`，其中 `amount`
 * 是当前遍历到的有效发票的金额（以分为单位）。为了保证每张发票只被用一次，内层循环需要从
 * 总额度`Q`向`amount`倒序遍历。
 *
 * 时间复杂度: O(N * M + N * Q')
 * 其中 N 是发票张数，M 是单张发票上物品的最大件数，Q' 是以“分”为单位的报销额度。
 * O(N*M) 用于处理N张发票的有效性检查。O(N*Q') 是0/1背包算法的时间复杂度。
 *
 * 空间复杂度: O(Q')
 * 主要由DP数组 `dp` 决定。
 */

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <algorithm>

// 使用标准命名空间，简化代码
using namespace std;

int main() {
    // 加速C++ I/O操作
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    double Q; // Q: 报销额度 (元)
    int N;    // N: 发票张数

    // 持续读取测试用例，直到N为0
    while (cin >> Q >> N && N != 0) {

        vector<long long> validInvoiceAmounts; // 存储所有有效发票的金额 (分)

        // 步骤1: 筛选有效发票
        for (int i = 0; i < N; ++i) {
            int m; // m: 当前发票的物品件数
            cin >> m;

            bool isInvoiceValid = true;
            long long currentInvoiceTotal = 0;
            vector<string> itemStrs(m);

            // 先一次性读取该发票的所有物品描述，避免输入流混乱
            for (int j = 0; j < m; ++j) {
                cin >> itemStrs[j];
            }

            // 逐项检查物品的有效性
            for (const string& itemStr : itemStrs) {
                size_t colonPos = itemStr.find(':');
                // 格式检查
                if (colonPos == string::npos || itemStr.length() < 3) {
                    isInvoiceValid = false;
                    break;
                }

                char type = itemStr[0];
                double price = stod(itemStr.substr(colonPos + 1));

                // 规则c: 检查物品类型
                if (type != 'A' && type != 'B' && type != 'C') {
                    isInvoiceValid = false;
                    break;
                }
                // 规则b: 检查单项价值
                if (price > 600.00) {
                    isInvoiceValid = false;
                    break;
                }
                // 累加金额（分）
                currentInvoiceTotal += round(price * 100);
            }

            // 如果物品检查已确定无效，则跳过后续检查
            if (!isInvoiceValid) {
                continue;
            }

            // 规则a: 检查发票总额
            if (currentInvoiceTotal > 100000) { // 1000元 = 100000分
                isInvoiceValid = false;
            }

            // 如果所有检查都通过，将发票金额存入有效列表
            if (isInvoiceValid) {
                validInvoiceAmounts.push_back(currentInvoiceTotal);
            }
        }

        // 步骤2: 0/1背包求解
        long long capacity = round(Q * 100); // 背包容量 (分)
        vector<long long> dp(capacity + 1, 0);

        // 遍历每张有效发票（物品）
        for (long long amount : validInvoiceAmounts) {
            // 从总容量开始向下遍历，确保每个物品只用一次
            for (long long j = capacity; j >= amount; --j) {
                dp[j] = max(dp[j], dp[j - amount] + amount);
            }
        }

        // 输出结果，将分转回元，并保留两位小数
        cout << fixed << setprecision(2) << dp[capacity] / 100.0 << endl;
    }

    return 0;
}