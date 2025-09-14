/*
2. 踩点上课
【问题描述】

       阿迪通常开着闹钟睡觉，这样他才不至于上课迟到。

       他想知道能否赶上第一节课，为了不迟到，他需要知道从家到学校所需要的最少时间是多少。

       阿迪生活的城市是一个 n×m 的矩形区域，其中每个单元( i, j )由一个数字 aij 来表示

数字为 -1 时表示该单元被占用，禁止通行

数字为 0 时表示该单元是空闲的，阿迪可以穿过

数字为 x (1≤x ≤109) 时表示该单元包含入口，需要耗费的时间成本为 x，包含入口的单元也是空闲的，可以自由通行

      从任何包含入口的单元出发，阿迪可以去往任何包含入口的其他单元，从入口( i, j )到入口( x, y ) 的时间成本总和为 aij + axy。

      除了在两个包含入口的单元之间移动，他也可以在具有相邻边的未被占用的单元之间移动，耗费的时间为 w 。实际上，他也可以进入一个包含入口的单元而不使用它。

      开始时，阿迪处在左上角单元 (1, 1)，而学校位于右下角(n, m)。

【输入形式】

       输入的第一行包含三个整数 n、m 和 w (2 ≤ n、m ≤ 2×103，1≤ w ≤109)，此处 n 和 m 是城市的大小，w 是在未被占用的单元之间移动所需要的时间。

       接下来的 n 行每行包含 m 个数 (-1 ≤ ai ≤ 109)，表示对单元的描述。

       输入保证单元(1, 1)和(n, m)是空闲的。

【输出形式】

       输出为一个数，表示阿迪去往学校需要花费的最少时间，如果他不能去到学校，则输出-1。

【样例输入】

5 5 1
0 -1 0 1 -1
0 20 0 0 -1
-1 -1 -1 -1 -1
3 0 0 0 0
-1 0 0 0 0
【样例输出】

14
*/
/*
 * 核心思路:
 * 这是一个在带特殊“传送门”的网格图上的最短路问题。路径可以由两种移动方式构成：
 * 1. 普通移动：在相邻的、非障碍的格子间移动，成本为 w。
 * 2. 传送门移动：从任意一个入口（值 > 0 的格子）P_i，可以瞬间移动到任意另一个入口 P_j，
 * 成本为 `value(P_i) + value(P_j)`。
 *
 * 关键洞察:
 * 任何一条最优路径，最多只会使用一次传送门移动。
 * 理由：如果一条路径使用了多次传送门，例如 S -> ... -> P1 -> P2 -> ... -> P3 -> P4 -> ... -> D，
 * 其传送总成本为 `(val(P1)+val(P2)) + (val(P3)+val(P4))`。我们可以选择直接从 P1 传送到 P4，
 * 成本为 `val(P1)+val(P4)`。由于所有入口值都为正数，很容易证明多次传送不会比单次传送更优。
 *
 * 算法设计:
 * 基于上述洞察，从起点到终点的最短路径只可能是以下两种情况之一：
 * 1. 完全不使用传送门：即从起点 (1,1) 到终点 (n,m) 的纯普通移动最短路。
 * 2. 使用一次传送门：路径形如“起点 -> 普通移动 -> 入口P_i -> 传送 -> 入口P_j -> 普通移动 -> 终点”。
 *
 * 我们可以将问题分解为几个独立的子问题并组合结果：
 * a. 计算起点 (1,1) 到图中所有点的普通移动最短路。我们称之为 `dist_from_S`。
 * b. 计算终点 (n,m) 到图中所有点的普通移动最短路。这等价于从 (n,m) 出发反向寻找最短路。
 * 我们称之为 `dist_from_D`。
 * c. 由于普通移动的成本 `w` 是固定的，我们可以使用广度优先搜索（BFS）来高效地计算(a)和(b)。
 *
 * 最终答案的计算:
 * 1. `Cost_A` (不使用传送门) = `dist_from_S` 中终点 `(n,m)` 的值。
 * 2. `Cost_B` (使用传送门) = (从起点S到任意入口P_i的最低总成本) + (从终点D到任意入口P_j的最低总成本)。
 * - `min_cost_S_to_portal = min(dist_from_S[P_i] + value(P_i))` 对所有入口 `P_i` 取最小值。
 * - `min_cost_D_to_portal = min(dist_from_D[P_j] + value(P_j))` 对所有入口 `P_j` 取最小值。
 * - `Cost_B = min_cost_S_to_portal + min_cost_D_to_portal`。
 * 3. 最终结果 = `min(Cost_A, Cost_B)`。如果某个成本无法计算（路径不通），则视为无穷大。
 *
 * 时间复杂度: O(N*M)
 * - 我们需要运行两次完整的网格BFS，每次的复杂度是 O(节点数 + 边数) = O(N*M)。
 * - 之后遍历一次网格寻找入口并计算最小成本，也是 O(N*M)。
 * - 总体复杂度由BFS主导，对于本题的约束来说非常高效。
 *
 * 空间复杂度: O(N*M)
 * - 需要两个 O(N*M) 的二维数组来存储从起点和终点出发的距离。
 */

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

// 使用 long long 防止路径成本在计算过程中溢出
using ll = long long;
// 定义一个足够大的数作为无穷大，用于表示不可达状态
const ll INF = 4e18;

using namespace std;

/**
 * @brief 使用广度优先搜索(BFS)计算从指定起点到网格所有点的最短距离。
 * @param start 起点坐标 {row, col} (0-indexed)
 * @param n 网格行数
 * @param m 网格列数
 * @param w 普通移动的成本
 * @param grid 网格布局
 * @return 一个二维向量，包含从起点到每个点的最短距离
 */
vector<vector<ll>> bfs(pair<int, int> start, int n, int m, ll w, const vector<vector<int>>& grid) {
    vector<vector<ll>> dist(n, vector<ll>(m, INF));
    queue<pair<int, int>> q;

    int start_r = start.first;
    int start_c = start.second;

    // 如果起点本身是障碍物，则无法从这里出发
    if (grid[start_r][start_c] == -1) {
        return dist;
    }

    dist[start_r][start_c] = 0;
    q.push({start_r, start_c});

    // 定义四个移动方向：上、下、左、右
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (!q.empty()) {
        pair<int, int> curr = q.front();
        q.pop();

        int r = curr.first;
        int c = curr.second;

        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            // 检查新坐标是否在边界内、是否可达（非障碍且未访问过）
            if (nr >= 0 && nr < n && nc >= 0 && nc < m && grid[nr][nc] != -1 && dist[nr][nc] == INF) {
                dist[nr][nc] = dist[r][c] + w;
                q.push({nr, nc});
            }
        }
    }
    return dist;
}


void solve() {
    int n, m;
    ll w;
    cin >> n >> m >> w;

    vector<vector<int>> grid(n, vector<int>(m));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> grid[i][j];
        }
    }

    // 1. 从起点 (0,0) 开始运行 BFS，计算到所有点的最短普通距离
    vector<vector<ll>> dist_from_S = bfs({0, 0}, n, m, w, grid);

    // 2. 从终点 (n-1, m-1) 开始运行 BFS，计算到所有点的最短普通距离
    vector<vector<ll>> dist_from_D = bfs({n - 1, m - 1}, n, m, w, grid);

    // 3. 计算两种可能路径的最低成本

    // 情况A: 完全不使用传送门的路径成本
    ll cost_no_portal = dist_from_S[n - 1][m - 1];

    // 情况B: 使用一次传送门的路径成本
    // 首先计算从起点S出发，通过普通移动到达任一入口并“激活”它的最低成本
    ll min_S_to_portal = INF;
    // 接着计算从终点D出发，通过普通移动到达任一入口并“激活”它的最低成本
    ll min_D_to_portal = INF;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            // 如果当前格子是入口
            if (grid[i][j] > 0) {
                // 只有当该入口能从起点到达时，才计算其成本
                if (dist_from_S[i][j] != INF) {
                    min_S_to_portal = min(min_S_to_portal, dist_from_S[i][j] + grid[i][j]);
                }
                // 只有当该入口能从终点到达时，才计算其成本
                if (dist_from_D[i][j] != INF) {
                    min_D_to_portal = min(min_D_to_portal, dist_from_D[i][j] + grid[i][j]);
                }
            }
        }
    }

    ll cost_with_portal = INF;
    // 只有当起点和终点都能连接到传送网络时，此路径才可能存在
    if (min_S_to_portal != INF && min_D_to_portal != INF) {
        cost_with_portal = min_S_to_portal + min_D_to_portal;
    }

    // 4. 最终答案是两种情况成本的较小值
    ll final_ans = min(cost_no_portal, cost_with_portal);

    if (final_ans >= INF) { // 使用 >= INF 更安全
        cout << -1 << endl;
    } else {
        cout << final_ans << endl;
    }
}

int main() {
    // 优化C++标准库的I/O性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}