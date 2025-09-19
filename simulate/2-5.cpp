/*5. 代码是一门艺术
【问题背景】

  代码是一门艺术，作为现代代码艺术抽象风格大师，你正打算打造你的绝世之作--“创世原码”！你已经写好了你所需要的所有code，并且code之间有调用关系。你非常珍重自己的成果，所以你决定让“创世之作”可以调用所有的code。但是作为顶级代码艺术大师，你决定使用数量最少的“直接”调用code来“间接”调用所有的code。

比如，1可以调用2，2可以调用3，那么直接调用1就可以间接调用2、3。

【问题描述】

    现在你将编写一个程序来计算这个最少数量，并输出具体的方案数。

    如果存在多个最小数量的方案，请输出字典序最小的一种方案

【输入形式】

    第一行包含两个正整数 n、m 分别表示code的数量和调用关系的数量

    接下来 m 行，每行包含两个正整数 u、v 表示调用 u 可以间接调用 v ，且 u ≠ v.

    保证不会给出重复的调用关系。

【输出形式】

    输出为若干行，每行包含一个要调用的code的编号，且按编号从小到大输出。

【样例输入1】


5 6
2 1
2 4
3 1
3 2
5 1
5 2
【样例输出1】


3
5
【样例输入2】


9 11
1 3
2 4
2 6
4 1
5 3
5 6
5 8
6 8
7 4
8 1
8 2
【样例输出2】


5
7
9


【样例说明】

无
【评分标准】

对于50%的数据，给出的调用关系关系中不会出现环

对于100%的数据，1≤ n ≤30000，1≤ m ≤500000


【出题人】

    拔尖班2022级林新晨


*/
/*
 * 核心思路:
 * 本题要求找到一个最小的节点集合，从这个集合出发可以遍历整个有向图。如果存在多个大小相同的最小集合，则输出字典序最小的那个。
 * 这个问题可以被抽象为在图的“缩点”后的有向无环图（DAG）上寻找所有入度为0的节点。
 * 1.  **强连通分量 (SCC)**: 首先，我们将原图中的所有强连通分量（SCC）缩成一个点。在一个SCC内部，任何一个节点都可以到达该SCC内的所有其他节点。因此，我们只需要进入一个SCC一次，就可以覆盖其中的所有节点。
 *
 * 2.  **Tarjan算法**: 我们使用Tarjan算法来高效地找出所有的强连通分量。在算法执行过程中，为每个节点分配一个它所属的SCC的唯一ID。
 *
 * 3.  **构建缩点图 (Condensation Graph)**: 找出SCC之后，我们可以构建一个缩点图（一个DAG）。在这个图中，每个节点代表一个SCC。如果原图中存在从节点u到节点v的边，且u和v不属于同一个SCC，那么在缩点图中就存在一条从u所属的SCC到v所属的SCC的边。
 *
 * 4.  **寻找根节点**: 问题的核心就变成了在缩点图这个DAG中，找到所有入度为0的SCC。这些SCC是整个调用链的起点，只要我们“直接调用”这些SCC中的任意一个节点，就可以通过依赖关系间接触及到所有从它们出发可以到达的SCC。因此，要覆盖全图，我们必须且只需从每一个入度为0的SCC中选取一个代表节点。
 *
 * 5.  **计算入度**: 我们无需显式地构建缩点图。在找到所有SCC后，我们遍历原图的每一条边(u, v)。如果u和v属于不同的SCC（即 sccId[u] != sccId[v]），那么就在缩点图中存在一条从sccId[u]到sccId[v]的边。我们据此增加 sccId[v] 的入度计数。
 *
 * 6.  **输出字典序最小解**: 题目要求输出字典序最小的方案。这意味着对于每一个入度为0的SCC，我们都应该选择其中编号最小的节点作为代表。我们遍历从1到n的所有节点，如果当前节点i所属的SCC是一个入度为0的SCC，并且我们还未为该SCC选择过代表节点，那么就选择i，并将其加入最终结果。这样可以保证我们总是为每个源SCC选择最小的节点，且最终输出的结果自然是升序的。
 *
 * 时间复杂度: O(N + M)，其中N是节点数，M是边数。这主要是Tarjan算法和后续遍历图计算入度的时间开销。
 * 空间复杂度: O(N + M)，用于存储图的邻接表以及Tarjan算法所需的辅助数组。
 */

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

// 图和Tarjan算法所需的全局变量
vector<vector<int>> adj;
vector<int> dfn, low, sccId;
vector<bool> onStack;
stack<int> s;
int timer = 0, sccCount = 0;

// Tarjan算法实现
void tarjan(int u) {
    dfn[u] = low[u] = ++timer;
    s.push(u);
    onStack[u] = true;

    for (int v : adj[u]) {
        if (dfn[v] == 0) { // v未被访问
            tarjan(v);
            low[u] = min(low[u], low[v]);
        } else if (onStack[v]) { // v已被访问且在当前DFS路径的栈中
            low[u] = min(low[u], dfn[v]);
        }
    }

    // 如果u是其所在SCC的根节点
    if (dfn[u] == low[u]) {
        ++sccCount;
        int node;
        do {
            node = s.top();
            s.pop();
            onStack[node] = false;
            sccId[node] = sccCount;
        } while (node != u);
    }
}

int main() {
    // 禁用 C++ 标准流与 C 标准流的同步，提高 IO 效率
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    cin >> n >> m;

    // 初始化
    adj.resize(n + 1);
    dfn.assign(n + 1, 0);
    low.assign(n + 1, 0);
    sccId.assign(n + 1, 0);
    onStack.assign(n + 1, false);

    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
    }

    // --- 步骤 1 & 2: 运行Tarjan算法找出所有SCC ---
    for (int i = 1; i <= n; ++i) {
        if (dfn[i] == 0) {
            tarjan(i);
        }
    }

    // --- 步骤 3 & 4: 计算每个SCC的入度 ---
    vector<int> inDegree(sccCount + 1, 0);
    for (int u = 1; u <= n; ++u) {
        for (int v : adj[u]) {
            if (sccId[u] != sccId[v]) {
                inDegree[sccId[v]]++;
            }
        }
    }

    // --- 步骤 5: 找出字典序最小的解 ---
    vector<int> result;
    vector<bool> sccChosen(sccCount + 1, false);
    // 遍历所有节点以保证字典序
    for (int i = 1; i <= n; ++i) {
        int currentSccId = sccId[i];
        // 如果节点i所在的SCC入度为0，且我们还未为这个SCC选择代表节点
        if (inDegree[currentSccId] == 0 && !sccChosen[currentSccId]) {
            result.push_back(i);
            sccChosen[currentSccId] = true;
        }
    }

    // 输出结果
    for (int node : result) {
        cout << node << "\n";
    }

    return 0;
}
