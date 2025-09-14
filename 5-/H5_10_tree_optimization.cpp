/*
10. 树的优化
【问题描述】

        在一个原始森林里，有人发现了一颗根编号为1的神奇树，它的每个顶点以及每条边上都标有一个数字。

   然而，他发现这颗树上有些顶点有瑕疵，也称为瑕疵点。一个顶点 v 被称为瑕疵点是指在它的子树中存在点 u，使得dist(v, u)>au，这里au是标注在顶点 u 上的数字，而dist(v, u)是所有标注在从顶点 v 到顶点 u 的路径上边的数字之和。

   这人决定删除一些叶子节点，直到整颗树不存在任何瑕疵点。那么，需要删除的叶子节点的最少数是多少？

【输入形式】

     输入的第一行为一个整数 n (1≤ n ≤ 105 )。

     接下来一行为 n 个整数a1、a2、...、an（1≤ ai ≤109），这里 ai 是标注在顶点 i 上的数字。

  接下来的 n - 1描述了树中边的情况，第 i 行有两个整数 pi和ci (1 ≤ pi ≤ n,  - 109 ≤ ci ≤ 109)，这意味着在顶点 i +1 和 pi之间有边相连，其上标有数字ci
【输出形式】

    输出一个整数，表示需要删除的最少节点数。

【样例输入】

9
88 22 83 14 95 91 98 53 11
3 24
7 -8
1 67
1 64
9 65
5 12
6 -80
3 8
【样例输出】

5
【提示】
以下是可能的处理过程

QQ图片20210604150127.png
*/
/*
 * 核心思路:
 * 这是一个在树上剪枝的问题，目标是保留一个最大的、满足特定“无瑕疵”条件的子树。
 * 反向思考，这等价于删除最少的节点，使得留下的所有节点都是“无瑕疵”的。
 *
 * 1. "瑕疵点"条件的转化:
 * - 定义 `d(x)` 为从根节点1到节点 `x` 的路径上所有边权的和。
 * - `dist(v, u)` (v是u的祖先) 可以精确表示为 `d(u) - d(v)`。
 * - 节点 `v` 是“瑕疵点”的条件是：其子树中存在某个节点 `u`，使得 `dist(v, u) > a[u]`。
 * - 这个条件可以转化为 `d(u) - d(v) > a[u]`，即 `d(u) - a[u] > d(v)`。
 *
 * 2. 节点保留条件:
 * - 为了让最终的树没有瑕疵点，我们保留下来的每一个节点 `u` 都必须不让它的任何祖先 `v` 成为瑕疵点。
 * - 也就是说，对于一个被保留的节点 `u` 和它的任意一个祖先 `v`，必须满足 `dist(v, u) <= a[u]`。
 * - 转化为 `d(u) - d(v) <= a[u]`，即 `d(u) - a[u] <= d(v)`。
 * - 这个条件必须对 `u` 的所有祖先 `v` 都成立。
 * - 这等价于一个更强的条件：`d(u) - a[u]` 必须小于等于其所有祖先路径和中的最小值。
 * `d(u) - a[u] <= min_{v is an ancestor of u} (d(v))`
 *
 * 3. 算法设计 (DFS):
 * - 如果一个节点 `u` 不满足上述条件，那么无论我们怎么删除它的后代，`d(u)` 和 `a[u]` 的值
 * 都不会改变，它将永远使其某个祖先成为瑕疵点。因此，唯一的解决办法是删除节点 `u`。
 * - 删除一个非叶子节点 `u` 意味着必须删除其整个子树才能消除它带来的影响。
 * - 这引导我们设计一个自顶向下的深度优先搜索（DFS）算法来识别并“删除”这些不满足条件的子树。
 *
 * 4. 算法流程:
 * a. **预处理**: 首先通过一次DFS，计算出以每个节点为根的子树的大小（`subtree_size[u]`）。
 * 这对于一次性计算需要删除的节点总数至关重要。
 * b. **主DFS**: 设计一个DFS函数 `dfs_check(u, path_sum_u, min_ancestor_sum)`:
 * - `u`: 当前正在处理的节点。
 * - `path_sum_u`: 从根到 `u` 的路径和 (`d(u)`)。
 * - `min_ancestor_sum`: `u` 的所有祖先 `v` 的 `d(v)` 的最小值。
 * c. **在DFS中**:
 * - 检查节点 `u` 是否满足保留条件 `path_sum_u - a[u] <= min_ancestor_sum`。
 * - 如果不满足 (`path_sum_u - a[u] > min_ancestor_sum`)，则节点 `u` 和它的整个子树
 * 都必须被删除。我们将预先计算好的 `subtree_size[u]` 累加到总删除计数中，并停止对该分支的
 * 进一步递归，因为整个分支都已被剪掉。
 * - 如果满足，则节点 `u` 本身可以被保留。我们继续对其子节点进行递归检查。在调用子节点 `c`
 * 时，需要更新 `path_sum` 和 `min_ancestor_sum` (新的 `min_ancestor_sum` 为 `min(原来的, path_sum_u)`)。
 * d. 最终的答案就是累加的删除总数。
 *
 * 时间复杂度: O(N)
 * - N是树的顶点数。我们需要两次独立的DFS遍历。第一次计算子树大小，第二次检查并计数。
 * 每次遍历都访问每个节点和每条边一次，因此是线性时间复杂度。
 *
 * 空间复杂度: O(N)
 * - 需要 O(N) 的空间来存储树的邻接表、子树大小数组以及DFS递归调用产生的栈空间。
 */



#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;
// 使用 long long 来防止路径和以及节点值的计算溢出
using ll = long long;

// 将全局状态封装，或在此处作为全局变量以简化DFS函数签名
int n;
vector<ll> a;
vector<vector<pair<int, int>>> adj;
vector<int> subtree_size;
int total_deleted_count;

/**
 * @brief 通过后序遍历计算以每个节点为根的子树大小。
 * @param u 当前节点。
 */
void dfs_size(int u) {
    subtree_size[u] = 1;
    for (auto const& [v, w] : adj[u]) { // C++17 结构化绑定
        dfs_size(v);
        subtree_size[u] += subtree_size[v];
    }
}

/**
 * @brief 主DFS函数，检查节点是否有效，并累加需要删除的节点数。
 * @param u 当前正在处理的节点。
 * @param path_sum 从根到节点u的路径上边权之和。
 * @param min_ancestor_sum 节点u的所有祖先v的path_sum的最小值。
 */
void dfs_check(int u, ll path_sum, ll min_ancestor_sum) {
    // 核心检查：一个节点 u 能被保留，当且仅当它不会让任何祖先 v 成为瑕疵点。
    // 该条件可转化为: path_sum(u) - a[u] <= min(path_sum(v)) 对所有祖先v成立。

    // 检查这个条件是否被违反
    if (path_sum - a[u] > min_ancestor_sum) {
        // 如果条件被违反，则 u 和它的整个子树都必须被删除。
        total_deleted_count += subtree_size[u];
        // 我们不需要再向下遍历这个分支，因为整个子树都被剪枝了。
        return;
    }

    // 如果 u 被保留，我们更新其子节点将要继承的 "min_ancestor_sum"
    // 这个值现在是 u 的祖先路径和与 u 自身路径和中的较小者。
    ll next_min_ancestor_sum = min(min_ancestor_sum, path_sum);
    // 对所有子节点进行递归检查
    for (auto const& [v, w] : adj[u]) {
        dfs_check(v, path_sum + w, next_min_ancestor_sum);
    }
}

void solve() {
    cin >> n;

    a.resize(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }

    adj.assign(n + 1, vector<pair<int, int>>());
    // 输入格式为：节点 i+1 的父节点 p 和边权 c
    for (int i = 1; i < n; ++i) {
        int p, c;
        cin >> p >> c;
        adj[p].push_back({i + 1, c});
    }

    // 步骤 1: 预处理，计算所有节点的子树大小。
    subtree_size.resize(n + 1);
    dfs_size(1); // 树根为节点 1

    // 步骤 2: 遍历树，检查并累加需要删除的节点总数。
    total_deleted_count = 0;
    // 初始调用DFS:
    // - 根节点1的 path_sum 为 0。
    // - 根节点没有祖先，其保留条件是无约束的。我们传递一个极大值作为
    //   min_ancestor_sum，以确保根节点自身永远不会因为这个检查而被删除。
    dfs_check(1, 0, LLONG_MAX);

    cout << total_deleted_count << endl;
}

int main() {
    // 优化C++标准库的I/O性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    solve();

    return 0;
}