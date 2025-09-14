/*
4. 有效的BFS
【问题描述】

       在图的BFS（广度优先搜索）中，通常采用队列来保存当前顶点的邻接点，但对对应邻接点的存入顺序没有要求，因此对于一个图的BFS结果可以有多个，在本问题中，从顶点1开始，请验证一个给定的顶点序列是否为一个有效的BFS序列？

【输入形式】

       输入的第一行为一个整数 n (1≤ n ≤ 2 ×105)，表示树中节点的数量。

       接下来 n-1 行描述了树的边，每行包含两个整数 x 和 y (1 ≤ x ,  y ≤ n)，表示对应边的两个端点，输入保证给定的图构成一颗树。

       最后一行为 n 个互不相同的整数 a1、a2、...、an( 1 ≤ ai ≤ n)，代表待检验的顶点序列。

【输出形式】

       如果待检验的序列是一个正确的BFS序列，输出"Yes"，否则输出"No"。

【样例输入1】

4
1 2
1 3
2 4
1 2 3 4
【样例输出1】

Yes
【样例输入2】

4
1 2
1 3
2 4
1 2 4 3
【样例输出2】

No
*/
/*
 * 核心思路:
 * 要验证一个序列是否是有效的BFS（广度优先搜索）序列，我们可以模拟BFS的执行过程，并检查
 * 给定的序列是否严格符合BFS的规则。
 *
 * 1. BFS的核心规则:
 * a. 搜索从一个指定的起点开始（本题为节点1）。
 * b. 遍历是逐层进行的。所有在 k 层的节点都必须在 k+1 层的任何节点之前被访问。
 * c. 对于任意一个父节点 u，它的所有子节点（即，在BFS树中与u相连的、层数更深的邻居）
 * 必须在序列中以一个“连续的块”出现。然而，这些子节点在这个块内的相对顺序可以是任意的。
 *
 * 2. 模拟验证算法:
 * 我们的策略是，按照给定的序列 `a` 来“消费”节点，同时用一个标准的队列 `q` 来驱动BFS的
 * 逐层逻辑。
 * a. 初始化一个标准的BFS队列 `q`，并将起点1放入。同时，用一个指针 `seq_idx` 指向
 * 序列 `a` 中下一个期望被发现的节点的位置，初始为 1。
 * b. 当队列 `q` 不为空时，我们取出一个父节点 `u`。
 * c. 根据BFS规则，接下来序列 `a` 中出现的一组节点必须是 `u` 的所有子节点。我们找出 `u`
 * 在树中的所有实际子节点（即，未被访问过的邻居）。假设有 `k` 个子节点。
 * d. 那么，在给定的序列 `a` 中，从 `seq_idx` 开始的 `k` 个节点
 * (`a[seq_idx]` 到 `a[seq_idx + k - 1]`) 必须恰好是 `u` 的这 `k` 个子节点。
 * e. **验证**: 为了忽略顺序，我们可以将 `u` 的实际子节点集合与序列 `a` 中对应的节点集合
 * 进行比较。如果两个集合内的元素完全相同，则验证通过。
 * f. **推进**: 如果验证通过，我们将这 `k` 个子节点（按照它们在序列 `a` 中出现的顺序）
 * 加入到我们的模拟队列 `q` 中，然后将 `seq_idx` 指针向后移动 `k` 位，接着处理队列
 * 中的下一个父节点。
 * g. 如果在任何一步验证失败，或者节点数量对不上，则序列无效。
 * h. 如果整个过程顺利完成，说明序列 `a` 是一个有效的BFS序列。
 *
 * 3. 数据结构与效率:
 * - 树的邻接关系：使用 `vector<unordered_set<int>>` 来存储邻接表。`unordered_set`
 * 可以让我们高效地进行集合比较，这对于验证步骤至关重要。
 * - 算法复杂度: 每个节点和每条边都只会被处理常数次。集合的构建和比较操作的总
 * 时间与总边数成正比。因此，整个算法的时间复杂度是 O(N)。
 *
 * 时间复杂度: O(N)
 * - N是节点的数量。建图需要 O(N) 时间。BFS模拟的每一步（包括集合操作）分摊下来
 * 是常数时间，因为每个节点和每条边都只会被访问一次，这是最优的。
 *
 * 空间复杂度: O(N)
 * - 需要 O(N) 的空间来存储邻接表、访问标记数组和队列。
 */

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>

using namespace std;

void solve() {
    int n;
    cin >> n;

    // 使用 unordered_set 存储邻居，方便后续进行高效的、无序的集合比较
    vector<unordered_set<int>> adj(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].insert(v);
        adj[v].insert(u);
    }

    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    // 规则1：BFS序列的第一个节点必须是起点1
    if (a[0] != 1) {
        cout << "No" << endl;
        return;
    }

    // 如果只有一个节点，它本身就是其唯一的、有效的BFS序列
    if (n == 1) {
        cout << "Yes" << endl;
        return;
    }

    // 用于BFS模拟的队列
    queue<int> q;
    q.push(1);

    // 标记节点是否被访问过，防止在树中回溯
    vector<bool> visited(n + 1, false);
    visited[1] = true;

    // 指向给定序列 a 中，下一个期望出现的子节点的位置
    int seq_idx = 1;

    // 当模拟队列非空且序列 a 还未被完全消费时
    while (!q.empty() && seq_idx < n) {
        int u = q.front();
        q.pop();

        // 1. 找出节点 u 在树中的所有实际子节点（即未访问过的邻居）
        unordered_set<int> actual_children;
        for (int neighbor : adj[u]) {
            if (!visited[neighbor]) {
                actual_children.insert(neighbor);
            }
        }
        int children_count = actual_children.size();

        // 2. 从序列 a 中提取出期望的子节点块
        // 检查序列中是否还有足够的节点来构成这个子节点块
        if (seq_idx + children_count > n) {
            cout << "No" << endl; // 序列 a 中的节点数量不足，不匹配
            return;
        }
        unordered_set<int> expected_children;
        for (int i = 0; i < children_count; ++i) {
            expected_children.insert(a[seq_idx + i]);
        }

        // 3. 比较实际子节点集和期望子节点集是否完全相同
        if (actual_children != expected_children) {
            cout << "No" << endl; // 节点集合不匹配
            return;
        }

        // 4. 验证通过，将被验证的子节点加入队列，并更新访问状态和序列指针
        for (int i = 0; i < children_count; ++i) {
            int child = a[seq_idx + i];
            // 如果一个节点已经被访问过，说明图中有环或者序列有重复，但题目保证是树
            if(visited[child]) {
                cout << "No" << endl;
                return;
            }
            visited[child] = true;
            q.push(child);
        }
        seq_idx += children_count;
    }

    // 如果所有节点都按规则处理完毕 (seq_idx 走到了序列末尾)，则序列有效
    if (seq_idx == n) {
        cout << "Yes" << endl;
    } else {
        // 如果循环结束但序列 a 未被完全消费，说明序列中包含了无法通过BFS访问到的节点，
        // 或者图不连通，但题目保证是树，所以这种情况暗示了序列结构错误。
        cout << "No" << endl;
    }
}

int main() {
    // 优化C++标准库的I/O性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 根据题目描述，只有一组测试数据
    solve();

    return 0;
}