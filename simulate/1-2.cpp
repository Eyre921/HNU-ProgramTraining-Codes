/*
2. 熊二与蹦蹦菇的故事
【问题描述】

一个清朗的早晨，熊二来到提瓦特大陆，他受到了草神纳西妲热情的接见，为了让熊二感受须弥当地的风情，纳西妲给熊二准备了一个游戏，首先有 n 个蹦蹦菇，前 n-1 个蹦蹦菇有一个跳跃的距离 ki，当熊二在某一个蹦蹦菇上时，他可以选择向前或者向后跳 ki 个距离，刚好跳到第 n 个蹦蹦菇上时游戏胜利，熊二想知道最少需要多少次跳跃他可以获得游戏胜利，特别的，熊二一开始站在第一个蹦蹦菇之前，他自己可以跳 p 个距离。

【输入形式】

第一行两个整数 n p

第二行 n-1个数，按顺序为 ki

【输出形式】

一个数为最少次数，若无法到达，输出-1

【样例输入】

7  1
1 2 4 1 1 1
【样例输出】

5
【样例说明】

熊二亲身实践，得出最优路线1-2-4-3-7

【评分标准】

40% n、k≤1000

70% n、k≤100000

100% n、k≤1000000(虽然熊二好像没有那么能跳）

【出题人】

拔尖班2022级邹俊豪
*/
/*
 * 【算法核心思路】
 * 这是一个图论中的最短路问题。我们将每个蹦蹦菇看作图中的节点，
 * 如果从一个蹦蹦菇可以跳到另一个蹦蹦菇，就在它们之间建立一条边。
 * 关键点：熊二初始可以跳1~p范围内的任意距离，而不是固定跳p个距离。
 *
 * 【解题方法】
 * 1. 使用BFS(广度优先搜索)来求最短路径
 * 2. 初始状态：将前p个蹦蹦菇都设为可达点（距离为1）
 * 3. 对于每个可达的蹦蹦菇，尝试向前或向后跳ki个位置
 *
 * 【时间复杂度】O(N) - 每个节点最多被访问一次
 * 【空间复杂度】O(N) - 需要队列和visited数组存储状态
 */

#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

int main() {
    int n, p;
    cin >> n >> p;

    // 存储每个蹦蹦菇的跳跃距离
    vector<int> jumps(n);
    for (int i = 0; i < n - 1; ++i) {
        cin >> jumps[i];
    }

    // 存储到达每个蹦蹦菇需要的最少跳跃次数，初始化为无穷大
    vector<int> distance(n + 1, INT_MAX);

    // BFS使用的队列
    queue<int> q;

    // 初始状态：熊二可以直接跳到前p个蹦蹦菇（注意索引从1开始）
    for (int i = 1; i <= min(p, n); ++i) {
        distance[i] = 1;  // 从起点跳到第i个蹦蹦菇需要1次跳跃
        q.push(i);
    }

    // BFS寻找最短路径
    while (!q.empty()) {
        int current = q.front();
        q.pop();

        // 如果已经到达目标，直接返回结果
        if (current == n) {
            cout << distance[n] << endl;
            return 0;
        }

        // 向前或向后跳跃
        int jumpDistance = jumps[current - 1];  // 当前蹦蹦菇的跳跃距离

        // 向前跳
        int forward = current + jumpDistance;
        if (forward <= n && distance[forward] == INT_MAX) {
            distance[forward] = distance[current] + 1;
            q.push(forward);
        }

        // 向后跳
        int backward = current - jumpDistance;
        if (backward >= 1 && distance[backward] == INT_MAX) {
            distance[backward] = distance[current] + 1;
            q.push(backward);
        }
    }

    // 如果BFS结束后仍然无法到达终点，输出-1
    if (distance[n] == INT_MAX) {
        cout << -1 << endl;
    } else {
        cout << distance[n] << endl;
    }

    return 0;
}
