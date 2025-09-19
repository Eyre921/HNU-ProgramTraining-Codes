/*
 *
1. 博弈论
CF1842A

【问题描述】

    Alice 和 Bob 在玩游戏，Alice 有 n 个怪兽，能力值为 a[1]~a[n]；Bob有 m 个怪兽，能力值为 b[1]~b[m]。

    Alice 与 Bob 轮流操作，Alice 先手。

    一次操作定义为选择一个 Alice 的怪兽和一个 Bob 的怪兽，若这两个怪兽的能力值为 x 和 y，则经过这次操作后能力值变为 x-y 和 y-x，若能力值小于等于 0，则这个怪兽会死亡。

若某玩家没有活着的怪兽，则这位玩家会输掉这场游戏，若两位玩家都没有活着的怪兽，则为平局。

【输入形式】

第一行一个整数 t ( t  ≤ 20)，表示测试数据的组数。

每组数据包含 3 行，第一行两个正整数 n、m(n、m ≤ 50)

第二行 n 个正整数，第 i 个数表示 a[i]( a[i] ≤ 109)。

第三行 m 个正整数，第 i 个数表示 b[i]( b[i] ≤ 109)。

【输出形式】

对于每组测试数据，输出胜利的玩家，若平局则输出 ‘Draw’。
【样例输入】

6
1 3
9
1 2 3
2 3
1 2
1 1 1
3 2
1 2 3
1 1
3 3
1 1 1
2 2 2
10 10
1 2 3 3 2 2 1 1 2 2
3 3 3 3 2 1 1 1 1 1
10 10
1 2 3 4 5 6 7 8 9 10
6 7 8 9 10 11 1 1 1 1
【样例输出】

Alice
Draw
Alice
Bob
Draw
Draw
【出题人】

拔尖班2022级朱名扬
 */
/*
 * 核心思路:
 * 这个问题是一个看似复杂的博弈论问题，但其本质非常简单。
 * 每次操作，Alice选择一个自己的怪兽（能力x）和Bob的一个怪兽（能力y），
 * 它们的能力分别变为 x-y 和 y-x。
 * 我们来分析一下双方总能力值的变化：
 * Alice的总能力值 S_a' = (S_a - x) + (x - y) = S_a - y
 * Bob的总能力值 S_b' = (S_b - y) + (y - x) = S_b - x
 * 这意味着，每次操作，Alice的总能力值会减少Bob出战怪兽的能力值，而Bob的总能力值会减少Alice出战怪兽的能力值。
 * 游戏结束的条件是某一方没有怪兽，这等价于其总能力值消耗殆尽。
 * 因此，整个游戏可以看作是双方在消耗对方的总能力值。
 * 策略和先后手顺序在这种模型下是无关紧要的，因为无论如何选择，最终的结果只取决于双方初始的总能力值之和。
 * 谁的总能力值高，谁就能在消耗战中存活下来。
 *
 * 时间复杂度: O(N + M)
 * 对于每组测试数据，我们需要读取 n 个 Alice 的怪兽能力值和 m 个 Bob 的怪兽能力值，
 * 并将它们分别累加。因此，时间复杂度是线性的，与怪兽总数成正比。
 *
 * 空间复杂度: O(1)
 * 我们只需要两个变量来存储双方的总能力值，不需要额外的存储空间。
 * 因此，空间复杂度是常数级别的。
 */

#include <iostream> // 用于标准输入输出
#include <vector>   // 虽然不是必须，但包含了常用的容器
#include <numeric>  // 包含 std::accumulate，但我们这里手动累加更直观

// 使用标准命名空间，简化代码
using namespace std;

// 解决单个测试用例的函数
void solve() {
    int n, m; // n: Alice的怪兽数量, m: Bob的怪兽数量
    cin >> n >> m;

    // 使用 long long 防止整数溢出
    // 题目中单个怪兽能力值可达 10^9，数量可达 50，总和会超过 32 位整数范围
    long long aliceTotalPower = 0;
    for (int i = 0; i < n; ++i) {
        int power;
        cin >> power;
        aliceTotalPower += power;
    }

    long long bobTotalPower = 0;
    for (int i = 0; i < m; ++i) {
        int power;
        cin >> power;
        bobTotalPower += power;
    }

    // 比较双方的总能力值来决定胜负
    if (aliceTotalPower > bobTotalPower) {
        cout << "Alice" << endl;
    } else if (bobTotalPower > aliceTotalPower) {
        cout << "Bob" << endl;
    } else {
        cout << "Draw" << endl;
    }
}

int main() {
    // 优化C++标准流的输入输出速度
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int testCases; // 测试数据组数
    cin >> testCases;

    // 循环处理每组测试数据
    while (testCases--) {
        solve();
    }

    return 0;
}
