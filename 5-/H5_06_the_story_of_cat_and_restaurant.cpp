/*
6. 猫与餐厅的故事
【问题描述】

      公司今天发薪，阿迪想与朋友们去餐厅庆祝一下。

       他住在一个非常神奇的公园里，这个公园是一个根在顶点1，且由 n 个顶点组成的有根树，顶点1也就是他的住所。然而不幸的是，公园也有许多的猫，阿迪已经找出了所有包含猫的顶点。

       公园的叶子顶点都有餐厅，阿迪想选择一家他可以去的餐厅，但很不幸，他非常害怕猫，因而如果从餐厅去往他家的路径上有连续包含猫的数量超过 m 时，他将不能去往这家餐厅。

       你的任务是帮助他确认他能去的餐厅的数量。

【输入形式】

       输入的第一行包含两个整数 n 和 m （2≤ n ≤ 105,  1≤ m ≤ n），分别表示树的顶点数以及对于阿迪来说可以忍受的最大的包含猫的连续顶点数。

        第二行包含 n 个整数 a1、a2、...、an，这里的每个 ai 或者为0（顶点 i 无猫），或者为1（顶点 i 有猫）。

       接下来的 n - 1 行包含用形式“xi  yi”（1≤ xi , yi ≤ n，xi ≠ yi）表示的树的边，表示顶点xi和顶点yi之间有边相连。

【输出形式】

       输出为一个整数，表示从阿迪家去往叶子顶点的路径上至多包含 m 只猫的叶子顶点的数量。

【样例输入1】

4 1
1 1 0 0
1 2
1 3
1 4
【样例输出1】

2
【样例输入2】

7 1
1 0 1 1 0 0 0
1 2
1 3
2 4
2 5
3 6
3 7
【样例输出2】

2
【样例说明】

很显然，树是具有个 n 顶点 n - 1 条边的连通图，有根树是有一个称为根的特殊顶点的树。

在样例一中

QQ图片20210630152715.png

包含猫的顶点变为红色，餐厅在顶点2、3、4，阿迪不能去到在顶点2的餐厅。

在样例二中

QQ图片20210630152926.png

餐厅在顶点4、5、6、7，阿迪不能去到6和7。
*/
/*
 * 核心思路:
 * 这个问题是在一棵树上进行的带条件的路径搜索，非常适合使用深度优先搜索（DFS）或广度优先搜索（BFS）。
 * 这里我们选择DFS，因为它能以非常自然的方式模拟“从家走到餐厅”的过程。
 *
 * 1. 数据结构:
 * - 图使用邻接表 `vector<vector<int>> adj` 来存储树的结构。
 * - 猫的位置使用一个布尔型或整型数组 `hasCat` 存储。
 *
 * 2. 深度优先搜索 (DFS) 函数:
 * - 我们设计一个递归函数 `dfs(u, parent, consecutiveCats)`。
 * - `u`: 当前所在的顶点。
 * - `parent`: 当前顶点的父节点，用于防止在树中走回头路。
 * - `consecutiveCats`: 从根节点到当前顶点 `u` 的路径上，末尾连续的有猫顶点的数量。
 *
 * 3. DFS 过程:
 * a. 进入 `dfs(u, ...)` 函数时，首先根据 `hasCat[u]` 更新当前路径的连续猫咪数。
 * - 如果 `u` 有猫，`currentConsecutiveCats = consecutiveCats + 1`。
 * - 如果 `u` 没猫，`currentConsecutiveCats = 0` (连续性中断)。
 *
 * b. 检查是否违反猫咪数量限制。
 * - 如果 `currentConsecutiveCats > m`，说明这条路已经无法继续走下去了，直接从当前递归返回，
 * 不再探索其子树。
 *
 * c. 判断是否到达餐厅（叶子节点）。
 * - 一个节点是叶子节点的充要条件是：它的度数为1，且它不是根节点（根节点的度数可能也为1）。
 * - 如果当前节点 `u` 是一个叶子节点，并且我们成功到达了这里（没有违反猫咪限制），
 * 那么我们就找到了一个可以去的餐厅，将结果计数器加一。
 *
 * d. 递归探索。
 * - 遍历 `u` 的所有邻居 `v`。
 * - 如果 `v` 不是父节点 `parent`，则对其调用 `dfs(v, u, currentConsecutiveCats)`。
 *
 * 4. 初始化调用:
 * - 从根节点1开始调用 `dfs(1, 0, 0)`。父节点设为0（一个不存在的节点），初始连续猫咪数为0。
 *
 * 时间复杂度: O(N)
 * 我们需要访问树中的每个顶点和每条边一次。
 *
 * 空间复杂度: O(N)
 * 主要由邻接表和递归调用栈的深度决定。在最坏的情况下（链状树），递归深度可达 O(N)。
 */

#include <iostream>
#include <vector>
#include <functional>

// 遵循规范，直接使用 std 命名空间
using namespace std;

int n, m;
vector<int> hasCat;
vector<vector<int>> adj;
int reachableRestaurants = 0;

// 深度优先搜索函数
void dfs(int u, int parent, int consecutiveCats) {
    // 步骤 a: 更新当前路径的连续猫咪数
    int currentConsecutiveCats = hasCat[u] ? consecutiveCats + 1 : 0;

    // 步骤 b: 检查是否违反猫咪数量限制
    if (currentConsecutiveCats > m) {
        return; // 此路不通，返回
    }

    // 步骤 c: 判断是否是叶子节点
    bool isLeaf = true;
    for (int v : adj[u]) {
        if (v != parent) {
            isLeaf = false; // 只要有非父节点的邻居，就不是叶子
            // 步骤 d: 递归探索
            dfs(v, u, currentConsecutiveCats);
        }
    }

    // 如果是叶子节点 (且不是根节点自身作为叶子的情况，通过parent!=0判断)
    // 且我们成功到达这里，说明找到一个可行的餐厅
    if (isLeaf) {
        reachableRestaurants++;
    }
}


int main() {
    // 提高IO性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m;

    // 读取猫的位置，数组下标从1开始，所以大小为 n+1
    hasCat.resize(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> hasCat[i];
    }

    // 构建邻接表
    adj.resize(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // 特殊情况处理：如果根节点只有一个邻居，dfs中isLeaf的判断逻辑对于根节点会出错，
    // 需要修正。更简单的逻辑是在dfs结束后再判断。
    // 一个更简洁的isLeaf判断是：adj[u].size() == 1 && u != 1。
    // 但是，在dfs递归中判断更自然。我们稍微修改一下DFS的逻辑。

    function<void(int, int, int)> solve_dfs =
        [&](int u, int parent, int consecutiveCats) {

        int currentConsecutiveCats = hasCat[u] ? consecutiveCats + 1 : 0;

        if (currentConsecutiveCats > m) {
            return;
        }

        // 检查u是否是叶子节点 (不是根节点1，且只有一个邻居)
        // 根节点1的父节点是0, adj[1]的size可能是1
        bool is_leaf = true;
        for (int v : adj[u]) {
            if (v != parent) {
                is_leaf = false;
                solve_dfs(v, u, currentConsecutiveCats);
            }
        }

        if (is_leaf) {
            reachableRestaurants++;
        }
    };


    // 步骤 4: 初始化调用
    // 从根节点1开始，其父节点设为0，初始连续猫咪数为0
    solve_dfs(1, 0, 0);

    cout << reachableRestaurants << endl;

    return 0;
}