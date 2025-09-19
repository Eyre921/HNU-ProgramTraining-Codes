/*
 *
5. 染色平台
CF1767E

【问题描述】

    有 n 个平台，被染上了 m 种颜色，你需要从第 1 个平台走到第 n 个平台。

    当你在第 i 个平台时，你可以选择走到第 i+1 或第 i+2 个平台，你只能走到你购买过的颜色的平台上。

    问最少需要使用多少钱才能从平台 1 走到平台 n。

    第 1 个平台和第 n 个平台的颜色必须购买。每种颜色只需要购买一次。

【输入形式】

    第一行两个整数 n、m，表示平台的数量和颜色的数量。

    第二行 n 个整数，第 i 个整数表示第 i 个平台的颜色。

    第三行 m 个整数，第 i 个整数表示颜色 i 的价格 p[i]。

    对于前10%的数据，m=1;

    对于前30%的数据，m<=10，n<=10;

    对于前60%的数据，m<=16;

    对于所有的数据，n<=300000，m<=40，p[i]<=1000000;

【输出形式】

    输出一行一个整数，表示最少花费。
【样例输入】

5 3
1 3 2 3 1
1 10 100
【样例输出】

11

【样例说明】

购买颜色 1 和 2。

【出题人】

拔尖班2022级朱名扬


 */
/*
题目：染色平台（等价于 Codeforces 风格的构造/优化题）

核心思路（图论转化 + 40 点以内的 MWIS 见面法 DP）：
1) 可达性约束等价转化
   - 从 i 只能走到 i+1 或 i+2，且只能落在已购买颜色的平台。
   - 想要从 1 到 n 可达，等价于不存在连续两个“未购买”的平台。
   - 换句话说：对每一条相邻边 (i, i+1)，至少有一个端点的平台颜色被购买。
   - 将“颜色是否购买”看作布尔变量，对每条边 (i, i+1) 给出子句 (color[c[i]] OR color[c[i+1]])。
   - 若 c[i] == c[i+1]，则子句退化为 color[c[i]] 必须为真 ⇒ 该颜色强制必须购买。
   - 对“颜色”为顶点、若两颜色在相邻位置中出现过相邻则连一条边，则问题变为：
     “在颜色图上选择最小权顶点覆盖（权为颜色价格），并且强制包含颜色 c[1] 和 c[n]。”

2) 强制颜色的处理
   - 强制颜色集合 F 包含：c[1]、c[n]，以及所有出现过 c[i]==c[i+1] 的颜色（自环）。
   - 这些颜色必买，其代价直接计入答案；并把与其相连的边都视为已覆盖从图中删除。
   - 剩余子图（仅包含非强制颜色）上求最小权顶点覆盖。

3) 最小权顶点覆盖与最大权独立集的对偶
   - 在任意图（不仅仅是二分图）中：最小权顶点覆盖重量 = 顶点总权重 - 最大权独立集重量（MWIS）。
   - 因此我们在剩余颜色子图上转而求解 MWIS。

4) m ≤ 40 的高效算法
   - 将剩余颜色点（≤ 40）用位集表示，拆成左右两半（各 ≤ 20）。
   - 对右半做“最大权独立集 on subset”的记忆化搜索 DP：bestR[mask] = mask 限制下的 MWIS。
   - 枚举左半的所有独立子集（用子集 DP 判断独立并累计与右半相邻的禁止集），
     对每个左半独立集 S_L，右半允许的可选 mask = AllR ^ N(S_L)，答案候选为
     weight(S_L) + bestR[allowedR]。
   - 取所有候选中的最大值即为 MWIS，总答案 = 强制和 + (剩余总权重 - MWIS)。

时间复杂度：
- 建图 O(n + m)，n ≤ 3e5, m ≤ 40。
- 见面法 MWIS：O(2^(m/2) · poly(m))，最多约 2^20 量级，实际非常快。

空间复杂度：
- 位集与 DP 表大小约 O(2^(m/2))，即最多 ~ 1,048,576 个状态，使用 long long 存储权值。

正确性要点：
- “每条边 (i, i+1) 至少选一端颜色” ⇔ “不存在连续两个未购买平台” ⇔ 可达。
- 自环代表强制选该颜色；强制点买入后删边不影响剩余图的覆盖可行性。
- MWIS 与最小权顶点覆盖的权对偶在任意图均成立。
*/

#include <bits/stdc++.h>
using namespace std;

struct MWISRightDP {
    int n2;                               // 右半大小（≤ 20）
    vector<long long> wR;                 // 右半权重
    vector<unsigned int> adjR;            // 右半内部邻接（位集）
    vector<long long> memo;               // bestR[mask] 记忆化
    MWISRightDP() = default;

    long long dfs(unsigned int mask) {
        if (mask == 0u) return 0LL;
        long long &res = memo[mask];
        if (res != -1) return res;
        // 取最低位作为分支点
        unsigned int v = __builtin_ctz(mask);
        unsigned int m0 = mask & ~(1u << v);                    // 不选 v
        long long skip_v = dfs(m0);
        // 选 v：从 mask 移除 v 及其邻居
        unsigned int m1 = m0 & ~adjR[v];
        long long take_v = wR[v] + dfs(m1);
        res = max(skip_v, take_v);
        return res;
    }

    long long solve(unsigned int allowedMask) {
        return dfs(allowedMask);
    }
};

long long solveInstance(int n, int m, const vector<int>& colors1based, const vector<long long>& price1based) {
    // 0-based 化
    vector<int> c(n);
    for (int i = 0; i < n; ++i) c[i] = colors1based[i] - 1;
    vector<long long> p(m);
    for (int i = 0; i < m; ++i) p[i] = price1based[i];

    // 强制颜色标记：c[0], c[n-1], 以及所有自环（相邻相同颜色）出现的颜色
    vector<char> forced(m, 0);
    forced[c[0]] = 1;
    forced[c[n-1]] = 1;
    for (int i = 0; i + 1 < n; ++i) {
        if (c[i] == c[i + 1]) forced[c[i]] = 1;
    }

    // 原始颜色图的邻接（忽略自环边，因为自环已强制）
    vector<unsigned long long> adjOrig(m, 0ull);
    for (int i = 0; i + 1 < n; ++i) {
        int a = c[i], b = c[i + 1];
        if (a == b) continue; // 自环通过 forced 处理
        adjOrig[a] |= (1ull << b);
        adjOrig[b] |= (1ull << a);
    }

    // 统计强制颜色成本与构建剩余颜色映射
    long long forcedCost = 0;
    vector<int> idMap(m, -1);        // 原颜色 -> 剩余图的编号 [0..r-1]
    vector<int> remColors;           // 剩余图中每个点对应的原颜色
    for (int col = 0; col < m; ++col) {
        if (forced[col]) forcedCost += p[col];
        else {
            idMap[col] = (int)remColors.size();
            remColors.push_back(col);
        }
    }

    int r = (int)remColors.size();
    if (r == 0) {
        // 全部强制，直接返回
        return forcedCost;
    }

    // 构建剩余子图（只保留两端均不强制的边）
    vector<unsigned long long> adjRemULL(r, 0ull);
    vector<long long> wRem(r, 0);
    for (int i = 0; i < r; ++i) {
        int orig = remColors[i];
        wRem[i] = p[orig];
        // 遍历与 orig 相邻的原颜色，若该颜色不强制则连边
        unsigned long long mask = adjOrig[orig];
        while (mask) {
            int nb = __builtin_ctzll(mask);
            mask &= mask - 1;
            if (forced[nb]) continue;
            int j = idMap[nb];
            if (j >= 0 && j != i) {
                adjRemULL[i] |= (1ull << j);
            }
        }
    }

    // 拆分左右两半
    int n1 = r / 2;
    int n2 = r - n1;

    // 左半权重与邻接（仅限左半）
    vector<long long> wL(n1, 0);
    vector<unsigned int> adjL(n1, 0);
    // 右半权重与邻接（仅限右半）
    vector<long long> wR(n2, 0);
    vector<unsigned int> adjR(n2, 0);
    // 左半 -> 右半的邻接（限制右半可选集合）
    vector<unsigned int> adjRfromL(n1, 0);

    // 准备映射：全局 r 点中 [0..n1-1] 为左半，[n1..r-1] 为右半
    for (int i = 0; i < n1; ++i) {
        wL[i] = wRem[i];
    }
    for (int j = 0; j < n2; ++j) {
        wR[j] = wRem[n1 + j];
    }

    // 填充左半、右半内部邻接与左右跨边邻接
    for (int i = 0; i < r; ++i) {
        unsigned long long mask = adjRemULL[i];
        while (mask) {
            int j = __builtin_ctzll(mask);
            mask &= mask - 1;
            if (j == i) continue;
            if (i < n1 && j < n1) {
                adjL[i] |= (1u << j);
            } else if (i >= n1 && j >= n1) {
                int ii = i - n1, jj = j - n1;
                adjR[ii] |= (1u << jj);
            } else if (i < n1 && j >= n1) {
                int jj = j - n1;
                adjRfromL[i] |= (1u << jj);
            } else { // i >= n1 && j < n1
                int ii = j;       // 左边的点
                int jj = i - n1;  // 右边的点
                adjRfromL[ii] |= (1u << jj);
            }
        }
    }

    // 右半：准备 MWIS 子问题（bestR[mask]）
    MWISRightDP solverR;
    solverR.n2 = n2;
    solverR.wR = std::move(wR);
    solverR.adjR = std::move(adjR);
    solverR.memo.assign(1u << n2, -1);

    // 左半：子集 DP，预计算
    int Llim = 1 << n1;
    vector<char> isInd(Llim, 0);
    vector<long long> sumWL(Llim, 0);
    vector<unsigned int> forbidR(Llim, 0);

    isInd[0] = 1;
    sumWL[0] = 0;
    forbidR[0] = 0;

    for (int mask = 1; mask < Llim; ++mask) {
        int v = __builtin_ctz(mask);
        int pmask = mask & (mask - 1); // 去掉最低位 v
        sumWL[mask] = sumWL[pmask] + wL[v];
        // 独立性：pmask 独立且 pmask 中无与 v 相邻者
        isInd[mask] = (isInd[pmask] && ((pmask & adjL[v]) == 0));
        // 禁止的右半集合 = pmask 的禁止集合 ∪ v 的右邻居
        forbidR[mask] = forbidR[pmask] | adjRfromL[v];
    }

    // 枚举所有左半独立子集，配合右半 bestR 取最大和
    unsigned int allR = (n2 == 32 ? 0xFFFFFFFFu : ((1u << n2) - 1u));
    long long bestMWIS = 0;
    for (int mask = 0; mask < Llim; ++mask) {
        if (!isInd[mask]) continue;
        unsigned int allowedR = allR & (~forbidR[mask]);
        long long cand = sumWL[mask] + solverR.solve(allowedR);
        if (cand > bestMWIS) bestMWIS = cand;
    }

    long long remTotal = 0;
    for (int i = 0; i < r; ++i) remTotal += wRem[i];

    long long minCoverRem = remTotal - bestMWIS;
    long long answer = forcedCost + minCoverRem;
    return answer;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 尝试读取正式输入；若失败，则运行内置测试
    int n, m;
    if (cin >> n >> m) {
        vector<int> colors(n);
        for (int i = 0; i < n; ++i) cin >> colors[i];
        vector<long long> price(m);
        for (int i = 0; i < m; ++i) cin >> price[i];
        cout << solveInstance(n, m, colors, price) << "\n";
        return 0;
    }

    // 内置测试用例（演示正确用法与期望输出）
    {
        // 样例 1（题面样例）：
        // 5 3
        // 1 3 2 3 1
        // 1 10 100
        // 期望输出：11
        int n1 = 5, m1 = 3;
        vector<int> c1 = {1, 3, 2, 3, 1};
        vector<long long> p1 = {1, 10, 100};
        cout << "Sample 1 Answer = " << solveInstance(n1, m1, c1, p1) << " (expected 11)\n";
    }
    {
        // 测试 2：仅端点强制 + 可直接 2 步跳过中间
        // n=3, m=2
        // colors: 1 2 1
        // price:  1 1
        // 购买颜色 1 即可（端点同色），答案=1
        int n2 = 3, m2 = 2;
        vector<int> c2 = {1, 2, 1};
        vector<long long> p2 = {1, 1};
        cout << "Sample 2 Answer = " << solveInstance(n2, m2, c2, p2) << " (expected 1)\n";
    }
    {
        // 测试 3：含自环（相邻相同颜色），该颜色被强制购买
        // n=2, m=2
        // colors: 2 2
        // price:  5 7
        // 颜色 2 强制购买（也是端点色），答案=7
        int n3 = 2, m3 = 2;
        vector<int> c3 = {2, 2};
        vector<long long> p3 = {5, 7};
        cout << "Sample 3 Answer = " << solveInstance(n3, m3, c3, p3) << " (expected 7)\n";
    }
    {
        // 测试 4：三角图（非二分），验证一般图下 MWIS 的正确性
        // 构造颜色序列：1 2 3 1 2 3，端点为 1 与 3 均强制
        // price: 1->100, 2->5, 3->100
        // 强制成本=200；剩余只剩颜色 2 与无边，则无需再选，答案=200
        int n4 = 6, m4 = 3;
        vector<int> c4 = {1, 2, 3, 1, 2, 3};
        vector<long long> p4 = {100, 5, 100};
        cout << "Sample 4 Answer = " << solveInstance(n4, m4, c4, p4) << " (expected 200)\n";
    }

    return 0;
}
