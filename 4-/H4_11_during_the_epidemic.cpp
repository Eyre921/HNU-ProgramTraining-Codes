/*
11. 疫情期间
【问题描述】

        正值新冠疫情期间，阿迪没法返回学校学习，他希望通过参加一些比赛来提高一下编程技能，同时做做运动。他收集了接下来的 n 天里每一天的信息，包括健身房是否开放，或者互联网上是否有程序设计竞赛。

        第 i 天可以有以下四种情况之一：

该天健身房不开放，互联网上也没有竞赛

该天健身房不开放，但互联网上有竞赛

该天健身房开放，但互联网上没有竞赛

该天健身房开放，互联网上也有竞赛

        每天阿迪要么休息，要么编写程序（如果该天有竞赛），要么做运动（如果该天健身房开放）。

        现在有一个限制条件：不能连续两天都去做运动，或者连续两天都编写程序。阿迪对自己要求很高，希望尽量多写程序或者多做运动，使得休息的天数尽量最少，求出这个天数。

【输入形式】

        输入的第一行为一个正整数 n (1≤ n ≤ 100)，表示接下来的天数。

        第二行为一个用空格分隔的整数序列 a1、a2、…、an（0≤ai≤3），这里

ai=0，第 i 天健身房不开放，互联网上也没有竞赛

ai=1，第 i 天健身房不开放，但互联网上有竞赛

ai=2，第 i 天健身房开放，但互联网上没有竞赛

ai=3，第 i 天健身房开放，互联网上也有竞赛

【输出形式】

        输入阿迪可能休息的最小天数。注意限制条件：

不能连续两天去做运动

不能连续两天编写程序

【样例输入1】

4
1 3 2 0
【样例输出1】

2
【样例输入2】

7
1 3 3 2 1 2 3
【样例输出2】

0
【样例输入3】

2
2 2
【样例输出3】

1
【样例说明】

在第一个样例中，阿迪在第一天编写程序，在第三天做运动，因此他仅有两天可以休息。

在第二个样例中，阿迪可以在第1、3、5、7天编写程序，其他天做运动，因此没有哪天休息。

在第三个样例中，阿迪可以在第1天或第2天做运动，但不能连续两天运动，因此他有一天休息。
【评分标准】
*/
/*
 * 核心思路:
 * 这是一个典型的动态规划(Dynamic Programming)问题。我们需要在满足“不能连续两天进行同一种活动”的约束下，
 * 做出每日的最优决策，以达到全局最优（最小化休息天数）。
 *
 * 1. 状态定义:
 * 某一天的决策（休息、编程、健身）会受到前一天活动的影响。因此，我们的DP状态需要包含“到第i天为止”和
 * “第i天做了什么”这两个维度。
 * 定义 dp[i][j] 为：在前 i 天里，第 i 天选择活动 j 时，所能达到的最少休息天数。
 * - j = 0: 第 i 天休息 (Rest)
 * - j = 1: 第 i 天编程 (Contest)
 * - j = 2: 第 i 天健身 (Gym)
 *
 * 2. 状态转移方程:
 * 对于第 i 天，我们根据第 i-1 天的状态来计算当前状态的最小值：
 * a) dp[i][0] (第i天休息):
 * 由于休息不受前一天活动限制，所以可以从前一天的任何活动状态转移而来。因为今天休息了，所以休息天数+1。
 * dp[i][0] = min(dp[i-1][0], dp[i-1][1], dp[i-1][2]) + 1
 *
 * b) dp[i][1] (第i天编程):
 * 必须当天有竞赛 (输入a[i]为1或3)，且前一天不能是编程。因此，只能从前一天的“休息”或“健身”状态转移。
 * dp[i][1] = min(dp[i-1][0], dp[i-1][2])
 *
 * c) dp[i][2] (第i天健身):
 * 必须当天健身房开放 (输入a[i]为2或3)，且前一天不能是健身。因此，只能从前一天的“休息”或“编程”状态转移。
 * dp[i][2] = min(dp[i-1][0], dp[i-1][1])
 *
 * 3. 空间优化:
 * 我们注意到，计算第 i 天的状态只依赖于第 i-1 天的状态。因此，没有必要存储整个 O(N) 的dp表，
 * 只需3个变量来存储前一天的状态值即可。这可以将空间复杂度从 O(N) 优化到 O(1)，是解决此类问题的标准优化手法。
 *
 * 4. 最终答案:
 * 遍历完所有天数后，最终的答案是最后一天三种可能活动（休息、编程、健身）状态下的最小休息天数。
 * Answer = min(dp[n][0], dp[n][1], dp[n][2])
 *
 * 时间复杂度: O(N)
 * - 我们需要遍历 n 天，每天的状态转移计算是常数时间。
 *
 * 空间复杂度: O(1)
 * - 采用空间优化后，我们只需要常数个变量来存储前一天的状态，与输入规模N无关。
 */

#include <iostream>
#include <vector>
#include <algorithm>

// 使用 std 命名空间，可以省略 `std::` 前缀，使代码更简洁
using namespace std;

// 定义一个足够大的数作为无穷大，用于表示不可达或非法的状态
const int INF = 1e9;

void solve() {
    int n;
    cin >> n;

    vector<int> activities(n);
    for (int i = 0; i < n; ++i) {
        cin >> activities[i];
    }

    // DP状态变量，使用空间优化。分别表示到“前一天”为止，以休息、编程、健身结束的最小休息天数。
    // 初始状态：在第0天（开始前），可以认为我们累积了0个休息日，并且可以自由选择第一天的任何活动。
    int prevMinRest_endedInRest = 0;
    int prevMinRest_endedInContest = 0;
    int prevMinRest_endedInGym = 0;

    // 遍历每一天，根据前一天的状态计算当天的最优状态
    for (int i = 0; i < n; ++i) {
        int todayActivityType = activities[i];

        // --- 计算当天的三种可能状态 ---

        // 状态1：今天选择“休息”
        // 可以从前一天的任何状态转移而来，总休息天数+1
        int currentMinRest_endingInRest = min({prevMinRest_endedInRest, prevMinRest_endedInContest, prevMinRest_endedInGym}) + 1;

        // 状态2：今天选择“编程”
        int currentMinRest_endingInContest = INF; // 默认为不可能
        // 条件：今天必须有竞赛 (1 或 3)，且昨天不能编程
        if (todayActivityType == 1 || todayActivityType == 3) {
            currentMinRest_endingInContest = min(prevMinRest_endedInRest, prevMinRest_endedInGym);
        }

        // 状态3：今天选择“健身”
        int currentMinRest_endingInGym = INF; // 默认为不可能
        // 条件：今天健身房必须开放 (2 或 3)，且昨天不能健身
        if (todayActivityType == 2 || todayActivityType == 3) {
            currentMinRest_endingInGym = min(prevMinRest_endedInRest, prevMinRest_endedInContest);
        }

        // --- 更新状态，为下一天的计算做准备 ---
        prevMinRest_endedInRest = currentMinRest_endingInRest;
        prevMinRest_endedInContest = currentMinRest_endingInContest;
        prevMinRest_endedInGym = currentMinRest_endingInGym;
    }

    // 最终答案是最后一天所有可能活动状态（休息、编程、健身）下的最小值
    cout << min({prevMinRest_endedInRest, prevMinRest_endedInContest, prevMinRest_endedInGym}) << endl;
}

int main() {
    // 禁用 C++ 标准流与 C 标准流的同步，大幅提高 I/O 效率
    ios_base::sync_with_stdio(false);
    // 解除 cin 与 cout 的绑定，避免不必要的刷新，进一步提速
    cin.tie(NULL);

    // 调用解决问题的函数
    solve();

    return 0;
}