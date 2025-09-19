/*
 *
4. 软件包管理器
【问题描述】

    你正在为一款自研操作系统设计自己的软件包管理器。软件包之间常常存在依赖，如果软件包a依赖软件包b，那么安装a之前，必须先安装b。反过来卸载b之前，也必须先卸载a。
    现在你拥有所有软件包之间的依赖关系。特别的是，除了0号软件包，所有的软件包有有且仅有1个依赖。0号不拥有依赖。已知依赖关系不会构成环。(用'->'表示依赖关系的话，即不存在a->b->...->a)
    现在你要为你的软件包管理器设计一个依赖处理工具。该工具需要在用户准备安装或卸载某个软件包时，快速统计出这一操作实际上会改变多少个软件包的状态(安装时实际安装的软件包数量，或者卸载时实际卸载的软件包数量)
    注意，安装一个已安装的软件包，或卸载一个未安装的软件包，都不会改变任何软件包的安装状态。

【输入形式】

    第一行一个正整数n，表示拥有n个软件包，请注意软件包从0开始编号
    第二行n-1个整数，第i个整数表示i号软件包依赖的软件包编号。
    然后一行一个正整数q，表示用户有q个操作。
    接下来q行每行一条操作，这些操作可能为两种情况：
        install x 表示安装x号软件包
        uninstall x 表示卸载x号软件包
    一开始所有的软件包都是未安装状态。
    对于每个操作，你需要输出该操作会改变状态的软件包的数量，然后不要忘了应用这一操作(改变目前状态)。



    对于10%的数据，n = 5,000，q = 5,000

    对于另外10%的数据，n = 100,000，q = 100,000，仅包含安装操作

    对于另外20%的数据，n = 100,000，q = 100,000，编号位i的软件包的依赖在0~i-1间均匀随机。每次执行操作的软件包编号在0~n-1均匀随机。

    对于另外60%的数据，n = 100,000，q = 100,000

【输出形式】

    输出q行，每行1个整数，含义如上。

【样例输入】

10
0 1 2 1 3 0 0 3 2
10
install 0
install 3
uninstall 2
install 7
install 5
install 9
uninstall 9
install 4
install 1
install 9
【样例输出】

1
3
2
1
3
1
1
1
0
1

【样例说明】

    一开始所有软件包都处于未安装状态。

    安装
5
号软件包，需要安装
0
,
1
,
5
三个软件包。

    之后安装
6
号软件包，只需要安装
6
6 号软件包。此时安装了
0
,
1
,
5
,
6
四个软件包。

    卸载
1
号软件包需要卸载
1
,
5
,
6
三个软件包。此时只有
0
号软件包还处于安装状态。

    之后安装
4
号软件包，需要安装
1
,
4
两个软件包。此时
0
,
1
,
4
处在安装状态。最后，卸载
0
号软件包会卸载所有的软件包。

【出题人】

    安全2202班方禹
 */
/*
题意总结与算法设计（核心思路、复杂度分析）

1) 核心思路
- 依赖关系构成一棵以 0 为根的树：除 0 外每个软件包有且仅有一个依赖（父节点），无环。
- 安装 x：必须确保从根 0 到 x 的整条路径全部“已安装”。因此本次实际改变的数量 = 路径长度 - 路径上已安装节点数；随后把整条路径全部标记为已安装。
- 卸载 x：必须卸载 x 的整个子树中所有“已安装”的包。本次实际改变的数量 = 子树中已安装节点数；随后把该子树全部标记为未安装。

为高效支持“路径求和/赋值”和“子树求和/赋值”，采用：
- 重链剖分（HLD）将任意根到某节点的路径分解为 O(log N) 段连续片段；
- 在线段树上做范围赋值（设为 0 或 1）和范围求和，统计“已安装”的数量。子树在 HLD 的位置序上对应连续区间，因此可 O(log N) 处理。

关键操作：
- 安装 x：
  ones = sum_path(0, x)          // 路径上已安装的数量
  len  = depth[x] + 1             // 路径长度（含根 0）
  changed = len - ones            // 实际改变的数量
  assign_path(0, x, 1)            // 将路径全部置为已安装
- 卸载 x：
  ones = sum_subtree(x)           // 子树内已安装的数量
  changed = ones
  assign_subtree(x, 0)            // 将子树全部置为未安装

2) 时间复杂度
- 重链剖分预处理：O(N)
- 线段树每次区间求和/赋值：O(log N)
- 每个“安装”操作：O(log^2 N)（路径被分解为若干段，每段线段树操作 O(log N)）
- 每个“卸载”操作：O(log N)
- 总体在 N, Q ≤ 1e5 时稳定通过。

3) 空间复杂度
- 邻接表、HLD 数组、线段树等：O(N)

实现细节说明
- 使用迭代式 DFS + 逆序统计 size/重儿子，避免递归栈风险。
- HLD 分解使用迭代方式保证子树在 base 数组中为连续区间。
- 线段树支持区间赋值（设为 0/1）+ 区间求和，lazy 为 -1 表示无标记，0/1 表示强制赋值。
*/

#include <bits/stdc++.h>
using namespace std;

struct SegmentTree {
    int n = 0;
    vector<int> sum;   // 区间内“已安装”数量
    vector<int> lazy;  // -1: 无标记, 0/1: 区间赋值

    void init(int n_) {
        n = n_;
        sum.assign(4 * n, 0);
        lazy.assign(4 * n, -1);
    }

    inline void apply(int p, int l, int r, int val) {
        // 将 [l, r] 区间强制赋值为 0 或 1
        sum[p] = val ? (r - l + 1) : 0;
        lazy[p] = val;
    }

    inline void push(int p, int l, int r) {
        // 下传懒标记
        if (lazy[p] != -1) {
            int m = (l + r) >> 1;
            apply(p << 1, l, m, lazy[p]);
            apply(p << 1 | 1, m + 1, r, lazy[p]);
            lazy[p] = -1;
        }
    }

    void update(int p, int l, int r, int ql, int qr, int val) {
        if (ql > r || qr < l) return;
        if (ql <= l && r <= qr) {
            apply(p, l, r, val);
            return;
        }
        push(p, l, r);
        int m = (l + r) >> 1;
        update(p << 1, l, m, ql, qr, val);
        update(p << 1 | 1, m + 1, r, ql, qr, val);
        sum[p] = sum[p << 1] + sum[p << 1 | 1];
    }

    int query(int p, int l, int r, int ql, int qr) {
        if (ql > r || qr < l) return 0;
        if (ql <= l && r <= qr) return sum[p];
        push(p, l, r);
        int m = (l + r) >> 1;
        return query(p << 1, l, m, ql, qr) + query(p << 1 | 1, m + 1, r, ql, qr);
    }

    // 便捷包装
    void assignRange(int l, int r, int val) { if (l <= r) update(1, 0, n - 1, l, r, val); }
    int  rangeSum(int l, int r) { return (l <= r) ? query(1, 0, n - 1, l, r) : 0; }
};

struct HLD {
    int n = 0;
    vector<vector<int>> g;    // 邻接表（树）
    vector<int> parent;
    vector<int> depth;
    vector<int> heavyChild;
    vector<int> subtreeSize;

    vector<int> head;         // 链头
    vector<int> pos;          // 在 base 数组中的位置（0..n-1）
    vector<int> rpos;         // 位置到结点的反向映射
    int curPos = 0;

    SegmentTree seg;

    HLD() = default;
    explicit HLD(int n_) { init(n_); }

    void init(int n_) {
        n = n_;
        g.assign(n, {});
        parent.assign(n, -1);
        depth.assign(n, 0);
        heavyChild.assign(n, -1);
        subtreeSize.assign(n, 1);
        head.assign(n, 0);
        pos.assign(n, 0);
        rpos.assign(n, 0);
        curPos = 0;
        seg.init(n);
    }

    void addEdge(int u, int v) { g[u].push_back(v); }

    // 预处理：parent, depth（迭代 DFS 前序），然后逆序统计 size 与重儿子
    void prepare(int root = 0) {
        // 迭代前序，得到一个父在前、子在后的顺序
        vector<int> order;
        order.reserve(n);
        {
            vector<int> st;
            st.reserve(n);
            st.push_back(root);
            parent[root] = -1;
            depth[root] = 0;
            while (!st.empty()) {
                int u = st.back(); st.pop_back();
                order.push_back(u);
                for (int v : g[u]) {
                    parent[v] = u;
                    depth[v] = depth[u] + 1;
                    st.push_back(v);
                }
            }
        }

        // 逆序统计子树尺寸及重儿子
        for (int idx = (int)order.size() - 1; idx >= 0; --idx) {
            int u = order[idx];
            int mx = 0, heavy = -1;
            int sz = 1;
            for (int v : g[u]) {
                sz += subtreeSize[v];
                if (subtreeSize[v] > mx) {
                    mx = subtreeSize[v];
                    heavy = v;
                }
            }
            subtreeSize[u] = sz;
            heavyChild[u] = heavy;
        }

        // 重链剖分（迭代）：先沿重儿子一路到底，再依次处理所有轻儿子作为新链头
        {
            vector<pair<int,int>> st; // (u, headOfChain)
            st.reserve(n);
            st.emplace_back(root, root);
            while (!st.empty()) {
                auto [u, h] = st.back();
                st.pop_back();

                // 处理链：从 u 沿重儿子一路到底
                int v = u;
                while (v != -1) {
                    head[v] = h;
                    pos[v] = curPos;
                    rpos[curPos] = v;
                    curPos++;

                    // 所有轻儿子作为新链入栈（先入后出，不影响正确性）
                    for (int w : g[v]) {
                        if (w == heavyChild[v]) continue;
                        st.emplace_back(w, w);
                    }

                    // 沿重儿子继续
                    v = heavyChild[v];
                }
            }
        }
    }

    // 线段树路径求和与赋值（u-v）
    int queryPath(int u, int v) {
        int res = 0;
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) swap(u, v);
            res += seg.rangeSum(pos[head[u]], pos[u]);
            u = parent[head[u]];
        }
        if (depth[u] < depth[v]) swap(u, v);
        res += seg.rangeSum(pos[v], pos[u]);
        return res;
    }

    void assignPath(int u, int v, int val) {
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) swap(u, v);
            seg.assignRange(pos[head[u]], pos[u], val);
            u = parent[head[u]];
        }
        if (depth[u] < depth[v]) swap(u, v);
        seg.assignRange(pos[v], pos[u], val);
    }

    // 子树查询与赋值
    int  querySubtree(int u)  { return seg.rangeSum(pos[u], pos[u] + subtreeSize[u] - 1); }
    void assignSubtree(int u, int val) { seg.assignRange(pos[u], pos[u] + subtreeSize[u] - 1, val); }
};

static void solve(istream& in, ostream& out) {
    int n;
    if (!(in >> n)) return;

    HLD hld(n);

    // 读取父节点信息：第 i 个整数表示 i 号软件包的依赖（父），i 从 1 到 n-1
    for (int i = 1; i < n; ++i) {
        int p; in >> p;
        // i 依赖 p，即树边 p -> i
        hld.addEdge(p, i);
    }

    // 预处理 HLD 与线段树
    hld.prepare(0);

    int q; in >> q;
    while (q--) {
        string op; int x;
        in >> op >> x;
        if (op[0] == 'i') { // install
            // 路径上现有已安装数量
            int ones = hld.queryPath(x, 0);
            int len  = hld.depth[x] + 1; // 含根
            int changed = len - ones;
            // 将路径全部置为已安装
            hld.assignPath(x, 0, 1);
            out << changed << '\n';
        } else { // uninstall
            int ones = hld.querySubtree(x);
            int changed = ones;
            // 将子树全部置为未安装
            hld.assignSubtree(x, 0);
            out << changed << '\n';
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 若有标准输入，则走评测逻辑；否则运行内置样例演示
    // 尝试预读一个字符来判断是否有输入
    if (cin.peek() == EOF) {
        // 内置样例演示
        const string sampleInput =
            "10\n"
            "0 1 2 1 3 0 0 3 2\n"
            "10\n"
            "install 0\n"
            "install 3\n"
            "uninstall 2\n"
            "install 7\n"
            "install 5\n"
            "install 9\n"
            "uninstall 9\n"
            "install 4\n"
            "install 1\n"
            "install 9\n";
        // 期望输出：
        // 1
        // 3
        // 2
        // 1
        // 3
        // 1
        // 1
        // 1
        // 0
        // 1
        istringstream in(sampleInput);
        solve(in, cout);
    } else {
        solve(cin, cout);
    }
    return 0;
}