/*
1. Dijkstra?
【问题描述】

       给定一个含权的无向图，顶点编号为1~n，你的任务为找出顶点1到顶点 n 之间的最短路径。

【输入形式】

       输入的第一行为两个整数 n 和 m (2 ≤ n ≤ 105,  0 ≤ m ≤ 105)，其中 n 为顶点数，m 是边数。

       接下来的 m 行包含用形式 ai、bi 和 wi (1≤ai、bi ≤n，1≤ wi ≤ 106)，这里ai、bi 是边的端点，而 wi 是边的长度。

       该图可能包括环，或者一对顶点之间包含多条边。

【输出形式】

       如果无路径，输出-1，否则输出最短路径，如果有多个，则输出字典序最小的路径。

       对于两个整数序列 A (a1、a2、...)和 B (b1、b2、...)，称序列 A 字典序小于序列 B 当且仅当，存在 k ≥ 1，i <k 时，ai = bi，i =k 时，ai < bi 。

【样例输入】

5 6
1 2 2
2 5 5
2 3 4
1 4 1
4 3 3
3 5 1
【样例输出】

1 4 3 5
*/
/*
 * 核心思路:
 * 这是一个带有“字典序最小”附加条件的单源最短路径问题。
 * 一个错误的贪心思路是在Dijkstra松弛操作时，当距离相等时，仅仅比较前驱节点的大小。
 * 这无法保证全局路径的字典序最小。
 *
 * 正确的、健壮的解法分为三步：
 * 1. 正向Dijkstra: 从起点1开始，计算出到所有其他节点的最短距离 `distFromSource`。
 * 由此我们得到1到n的最短路径总长度 `shortestPathLength`。
 *
 * 2. 反向Dijkstra: 从终点n开始，在一个“反向图”上运行Dijkstra算法，
 * 计算出所有节点到终点n的最短距离 `distToDest`。
 * （在反向图上从n出发，等价于在原图上计算所有点到n的距离）。
 *
 * 3. 贪心路径构建: 有了这两个距离数组，我们可以精确地判断任何一条边(u, v)
 * 是否位于某条1->n的最短路径上。
 * 充要条件是：`distFromSource[u] + weight(u, v) + distToDest[v] == shortestPathLength`。
 * 我们从起点1开始，迭代地构建路径：
 * - 在当前节点 `curr`，遍历其所有邻居 `next`。
 * - 找到所有满足上述条件的“有效”邻居。
 * - 在所有“有效”邻居中，选择节点编号最小的一个作为路径的下一个节点。
 * - 重复此过程，直到到达终点n。
 *
 * 这个贪心选择之所以正确，是因为在路径的每一步，我们都选择了能通往终点的、字典序最小的
 * 下一个节点，从而保证了整个路径的字典序是最小的。
 *
 * 时间复杂度: O(M log N)
 * 主要由两次Dijkstra算法决定。路径构建步骤的时间复杂度为 O(N+M)。
 *
 * 空间复杂度: O(N + M)
 * 用于存储邻接表、反向邻接表和两个距离数组。
 */

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

// 遵循规范，直接使用 std 命名空间
using namespace std;

// 定义类型别名，提高代码可读性
using State = pair<long long, int>; // {距离, 节点编号}
using Edge = pair<int, int>;       // {邻居节点, 边权重}

// 定义一个巨大的数表示无穷大
const long long INF = numeric_limits<long long>::max();

// Dijkstra 算法的通用实现，返回从起点到所有点的距离数组
vector<long long> dijkstra(int startNode, int n, const vector<vector<Edge>>& adj) {
    vector<long long> dist(n + 1, INF);
    priority_queue<State, vector<State>, greater<State>> pq;

    dist[startNode] = 0;
    pq.push({0, startNode});

    while (!pq.empty()) {
        long long currentDist = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (currentDist > dist[u]) {
            continue;
        }

        for (const auto& edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

int main() {
    // 提高IO性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    cin >> n >> m;

    // adj: 正向图, adjRev: 反向图
    vector<vector<Edge>> adj(n + 1);
    vector<vector<Edge>> adjRev(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w}); // 无向图
        adjRev[u].push_back({v, w}); // 同样为无向图
        adjRev[v].push_back({u, w});
    }

    // 步骤 1: 运行正向Dijkstra
    vector<long long> distFromSource = dijkstra(1, n, adj);

    // 检查路径是否存在
    long long shortestPathLength = distFromSource[n];
    if (shortestPathLength == INF) {
        cout << -1 << endl;
        return 0;
    }

    // 步骤 2: 运行反向Dijkstra
    vector<long long> distToDest = dijkstra(n, n, adjRev);

    // 步骤 3: 贪心构建字典序最小路径
    vector<int> path;
    int currentNode = 1;
    path.push_back(currentNode);

    while (currentNode != n) {
        int nextNode = -1;
        // 遍历当前节点的所有邻居，寻找最优的下一个节点
        for (const auto& edge : adj[currentNode]) {
            int neighbor = edge.first;
            int weight = edge.second;

            // 检查该邻居是否在某条最短路径上
            if (distFromSource[currentNode] + weight + distToDest[neighbor] == shortestPathLength) {
                // 在所有有效的邻居中，选择编号最小的
                if (nextNode == -1 || neighbor < nextNode) {
                    nextNode = neighbor;
                }
            }
        }
        // 移动到选择的下一个节点
        currentNode = nextNode;
        path.push_back(currentNode);
    }

    // 输出最终路径
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i] << (i == path.size() - 1 ? "" : " ");
    }
    cout << endl;

    return 0;
}