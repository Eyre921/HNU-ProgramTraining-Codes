/*
 *
4. Omsk Metro
【问题描述】

最开始有一个编号为1权值为1的点，接下来有 n 个操作：

1、+   u   x   表示添加一条起点为 u 且连向点的点权为 x 的边。x∈{-1,1}，假设在该操作之前点的总数为tot，则该边的终点应为tot+1

2、？ u  v  k    表示询问 u 到 v 的最短路径上是否存在一个子段使得子段上的点的权值和恰好等于 k ，并输出“YES”或“NO”。

*子段：元素的连续序列，这个子段可以为空，且子段为空时子段和为0.

可以保证任意两点之间只有唯一一条路径,且不会出现自环。

【输入形式】

 第一行包含一个正整数n，表示操作的个数

  接下来 n 行，每行表示下列操作之一：

·首先是一个符号‘+’，其次是两个正整数 u、x∈{-1,1}表示新建的边。

·首先是一个符号‘？’，其次是两个正整数 u、v、k 表示询问 u、v 之间的最短路径之间是否存在权重之和恰好为 k 的子段（可以为空）。


【输出形式】

 对于每次询问，输出一行，若存在询问子段则输出“YES”，否则输出“NO”

【样例输入1】


8
+ 1 -1
? 1 1 2
? 1 2 1
+ 1 1
? 1 3 -1
? 1 1 1
? 1 3 2
? 1 1 0
【样例输出1】


NO
YES
NO
YES
YES
YES
【样例输入2】


7
+ 1 -1
+ 2 -1
+ 2 1
+ 3 -1
? 5 2 2
? 3 1 -1
? 5 4 -3
【样例输出2】



NO
YES
YES
【样例1说明】

第二个问题的答案是 "YES"，因为存在一条路径1

在第四题中，我们可以再次选择1路径。

在第五个问题中，答案是 "YES"，因为存在路径 1−3

在第六个问题中，我们可以选择一条空路径，因为它的权重之和为0

不难证明，不存在满足第一和第三个查询的路径。

【评分标准】

对于 20% 的数据，保证 1≤ n ≤ 10000，并且点的个数最多为5000

对于额外的 30% 的数据，保证 ‘?’ 操作中 u = 1

对于100% 的数据，保证 1≤ n ≤ 200000，x ∈{-1,1}


【出题人】

 拔尖班2202级林新晨
 */

/**
 * @file omsk_metro_lct.cpp
 * @brief Omsk Metro (动态树 + 路径子段和查询)
 *
 * 核心思路:
 * 1.  问题建模: 题目描述了一个动态加点的过程，每个新点连接到已存在的某个点上，且保证无环。
 * 这实质上是在动态地构建一棵树。查询操作是在这棵树上任意两点间的唯一路径上进行的。
 * 2.  查询转化: 询问 "u到v的路径上是否存在点权和为k的子段"，由于点权仅为+1或-1，
 * 路径上的点权序列构成了一个由+1和-1组成的数组。一个关键性质是：对于这样的序列，
 * 其所有非空子段和会构成一个连续的整数区间 [minSubarraySum, maxSubarraySum]。
 * 因此，问题转化为求出 u-v 路径上的最大和最小子段和，然后判断 k 是否在该区间内。
 * 特别地，题目允许空子段，其和为0，所以当 k=0 时，答案恒为"YES"。
 * 3.  数据结构选择: 为了高效地支持动态加边（连接）和路径信息查询（最大/最小子段和），
 * Link-Cut Tree (LCT) 是理想的数据结构。LCT 可以将树的路径操作转化为对辅助树
 * (Splay Tree) 的序列操作，均摊时间复杂度为 O(log N)。
 * 4.  信息维护: 我们在 LCT 的每个 Splay 节点上维护其所代表的路径片段的聚合信息。
 * 这包括：区间和、最大/最小前缀和、最大/最小后缀和、以及最大/最小子段和。
 * 这些信息可以在 Splay 树的节点合并时（对应于路径片段的拼接）高效地更新。
 * 5.  查询流程: 查询 u 到 v 的路径时，我们通过 LCT 的 `makeRoot(u)` 和 `access(v)` 操作，
 * 将 u 到 v 的路径提取到一棵单独的 Splay 树中，其根节点就是 v。此时，v 节点上
 * 维护的聚合信息 `pathInfoData[v]` 即为整条 u-v 路径的信息。
 *
 * 时间复杂度: O(M * log N)，其中 M 是操作总数，N 是节点总数。LCT 的每次操作（加点、查询）
 * 的均摊复杂度都是 O(log N)。
 * 空间复杂度: O(N)，LCT 的所有辅助数组都需要线性的空间。
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

// 使用标准命名空间，简化代码
using namespace std;

// 无穷大常量，用于初始化最小/最大值
static const int INF = 1e9;

// ------------------------------ LCT 节点维护的路径信息 ------------------------------

// 用于维护 LCT 辅助树 Splay 节点信息的结构体
// 代表了 Splay 子树中序遍历所形成的路径（或路径片段）的聚合信息
struct PathInfo {
    int sum;         // 区间内所有节点权值的总和
    int prefMax;     // 从区间左端点开始的最大前缀和 (非空)
    int prefMin;     // 从区间左端点开始的最小前缀和 (非空)
    int sufMax;      // 到区间右端点结束的最大后缀和 (非空)
    int sufMin;      // 到区间右端点结束的最小后缀和 (非空)
    int subMax;      // 区间内的最大子段和 (非空)
    int subMin;      // 区间内的最小子段和 (非空)
};

/**
 * @brief 创建一个代表“空路径”的信息单元，作为合并操作的单位元。
 * @return PathInfo 代表空路径的信息。
 */
inline PathInfo createEmptyPathInfo() {
    return PathInfo{
        0,
        -INF, +INF, // 前缀和：空路径无非空前缀，故设为极值
        -INF, +INF, // 后缀和：同上
        -INF, +INF  // 子段和：同上
    };
}

/**
 * @brief 根据单个节点的权值创建一个路径信息单元。
 * @param weight 节点的权值 (+1 或 -1)。
 * @return PathInfo 代表单个节点路径的信息。
 */
inline PathInfo createSingleNodePathInfo(int weight) {
    return PathInfo{
        weight,
        weight, weight, // 单个点的前缀、后缀、子段和都是其自身的值
        weight, weight,
        weight, weight
    };
}

/**
 * @brief 合并两个相邻路径片段 (left + right) 的信息。
 * @param leftPath 左侧路径片段的信息。
 * @param rightPath 右侧路径片段的信息。
 * @return 合并后的新路径片段信息。
 */
inline PathInfo mergePathInfo(const PathInfo& leftPath, const PathInfo& rightPath) {
    PathInfo result;
    // 总和是简单的相加
    result.sum = leftPath.sum + rightPath.sum;

    // 新前缀最大和: 要么是左半部分的前缀最大和，要么是贯穿整个左半部分并延伸到右半部分
    result.prefMax = max(leftPath.prefMax, leftPath.sum + rightPath.prefMax);
    result.prefMin = min(leftPath.prefMin, leftPath.sum + rightPath.prefMin);

    // 新后缀最大和: 要么是右半部分的后缀最大和，要么是贯穿整个右半部分并延伸到左半部分
    result.sufMax = max(rightPath.sufMax, leftPath.sufMax + rightPath.sum);
    result.sufMin = min(rightPath.sufMin, leftPath.sufMin + rightPath.sum);

    // 新子段最大和: 来源有三：完全在左半部分、完全在右半部分、或跨越中点（左半部分的后缀+右半部分的前缀）
    result.subMax = max({leftPath.subMax, rightPath.subMax, leftPath.sufMax + rightPath.prefMax});
    result.subMin = min({leftPath.subMin, rightPath.subMin, leftPath.sufMin + rightPath.prefMin});

    return result;
}

// ------------------------------ Link-Cut Tree 实现 ------------------------------
struct LinkCutTree {
    int maxNodeCount;                      // 预分配的最大节点数量
    vector<int> parent;                    // 存储每个节点的父节点指针（在Splay树中）
    vector<int> leftChild, rightChild;     // Splay树的左右孩子
    vector<bool> isReversed;               // 翻转标记，用于LCT的makeRoot操作
    vector<int> nodeValue;                 // 存储原始图中节点的权值（+1 或 -1）
    vector<PathInfo> pathInfoData;         // 存储每个Splay节点聚合的路径信息

    explicit LinkCutTree(int size = 0) { init(size); }

    void init(int size) {
        maxNodeCount = size;
        parent.assign(maxNodeCount + 1, 0);
        leftChild.assign(maxNodeCount + 1, 0);
        rightChild.assign(maxNodeCount + 1, 0);
        isReversed.assign(maxNodeCount + 1, false);
        nodeValue.assign(maxNodeCount + 1, 0);
        pathInfoData.assign(maxNodeCount + 1, createEmptyPathInfo());
    }

    // 判断节点 x 是否为其所在 Splay 树的根
    inline bool isSplayRoot(int nodeId) const {
        int parentId = parent[nodeId];
        return parentId == 0 || (leftChild[parentId] != nodeId && rightChild[parentId] != nodeId);
    }

    // 对节点 x 应用翻转操作
    inline void applyReverse(int nodeId) {
        if (!nodeId) return;
        isReversed[nodeId] = !isReversed[nodeId];
        swap(leftChild[nodeId], rightChild[nodeId]);
        // 翻转路径后，前缀和与后缀和也需要交换
        swap(pathInfoData[nodeId].prefMax, pathInfoData[nodeId].sufMax);
        swap(pathInfoData[nodeId].prefMin, pathInfoData[nodeId].sufMin);
    }

    // 下放懒标记（当前仅有翻转标记）
    inline void pushDown(int nodeId) {
        if (isReversed[nodeId]) {
            if (leftChild[nodeId]) applyReverse(leftChild[nodeId]);
            if (rightChild[nodeId]) applyReverse(rightChild[nodeId]);
            isReversed[nodeId] = false;
        }
    }

    // 从子节点信息更新父节点信息
    inline void pushUp(int nodeId) {
        PathInfo leftInfo = leftChild[nodeId] ? pathInfoData[leftChild[nodeId]] : createEmptyPathInfo();
        PathInfo midInfo = createSingleNodePathInfo(nodeValue[nodeId]);
        PathInfo rightInfo = rightChild[nodeId] ? pathInfoData[rightChild[nodeId]] : createEmptyPathInfo();
        // 合并顺序必须是：左子树 -> 当前节点 -> 右子树，以保证中序遍历的正确性
        pathInfoData[nodeId] = mergePathInfo(mergePathInfo(leftInfo, midInfo), rightInfo);
    }

    // Splay 树的旋转操作
    void rotate(int nodeId) {
        int parentNode = parent[nodeId];
        int grandparentNode = parent[parentNode];
        bool isRightChild = (nodeId == rightChild[parentNode]);
        int childSubtree = isRightChild ? leftChild[nodeId] : rightChild[nodeId];

        if (!isSplayRoot(parentNode)) {
            if (parentNode == leftChild[grandparentNode]) leftChild[grandparentNode] = nodeId;
            else if (parentNode == rightChild[grandparentNode]) rightChild[grandparentNode] = nodeId;
        }
        parent[nodeId] = grandparentNode;

        if (isRightChild) {
            rightChild[parentNode] = childSubtree;
            leftChild[nodeId] = parentNode;
        } else {
            leftChild[parentNode] = childSubtree;
            rightChild[nodeId] = parentNode;
        }
        if (childSubtree) parent[childSubtree] = parentNode;
        parent[parentNode] = nodeId;

        pushUp(parentNode);
        pushUp(nodeId);
    }

    // 将节点 x 旋转到其所在 Splay 树的根
    void splay(int nodeId) {
        vector<int> ancestorStack;
        ancestorStack.reserve(32); // 预分配空间，避免频繁重分配
        int currentNode = nodeId;
        while (true) {
            ancestorStack.push_back(currentNode);
            if(isSplayRoot(currentNode)) break;
            currentNode = parent[currentNode];
        }
        // 从上到下 pushDown，确保旋转路径上的信息正确
        for (int i = (int)ancestorStack.size() - 1; i >= 0; --i) {
            pushDown(ancestorStack[i]);
        }

        while (!isSplayRoot(nodeId)) {
            int parentNode = parent[nodeId];
            int grandparentNode = parent[parentNode];
            if (!isSplayRoot(parentNode)) {
                bool zigzig = ((nodeId == leftChild[parentNode]) == (parentNode == leftChild[grandparentNode]));
                if (zigzig) rotate(parentNode);
                else rotate(nodeId);
            }
            rotate(nodeId);
        }
        pushUp(nodeId);
    }

    // LCT核心操作：打通从根到 x 的路径，使其成为一条偏爱路径
    void access(int nodeId) {
        int lastChildSplayRoot = 0;
        for (int currentNode = nodeId; currentNode; currentNode = parent[currentNode]) {
            splay(currentNode);
            // 断开旧的偏爱孩子，连接新的（来自更深层的部分）
            rightChild[currentNode] = lastChildSplayRoot;
            pushUp(currentNode);
            lastChildSplayRoot = currentNode;
        }
        splay(nodeId);
    }

    // 将节点 x 设置为它所在原树的根
    void makeRoot(int nodeId) {
        access(nodeId);
        applyReverse(nodeId);
    }

    // 查找节点 x 所在原树的根
    int findRoot(int nodeId) {
        access(nodeId);
        while (true) {
            pushDown(nodeId);
            if (!leftChild[nodeId]) break;
            nodeId = leftChild[nodeId];
        }
        splay(nodeId);
        return nodeId;
    }

    // 连接两个节点（x作为y的子节点），前提是它们不在同一棵树中
    void link(int childNode, int parentNode) {
        makeRoot(childNode);
        if (findRoot(parentNode) != childNode) {
            parent[childNode] = parentNode;
        }
    }

    // 查询 u 到 v 的路径信息
    PathInfo queryPath(int u, int v) {
        makeRoot(u);
        access(v);
        return pathInfoData[v];
    }

    // 设置节点 x 的权值
    void setNodeValue(int nodeId, int weight) {
        access(nodeId);
        nodeValue[nodeId] = weight;
        pushUp(nodeId);
    }
};


/**
 * @brief 主逻辑函数，处理所有输入和操作
 */
void solve() {
    // 提高C++流的性能
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int operationCount;
    if (!(cin >> operationCount)) return;

    // 节点总数最多为 operationCount + 1 (初始的节点1)
    int maxNodes = operationCount + 1;
    LinkCutTree lct(maxNodes + 5); // 预留一些缓冲空间
    int currentNodeCount = 1;      // 当前已创建的节点总数，初始为1

    // 题目规定，节点1初始存在且权值为1
    lct.setNodeValue(1, 1);

    for (int i = 0; i < operationCount; ++i) {
        char operationType;
        cin >> operationType;

        if (operationType == '+') {
            int parentNodeId, newNodeValue;
            cin >> parentNodeId >> newNodeValue; // 读取父节点 u 和新节点权值 x

            ++currentNodeCount; // 节点总数加一，新节点编号即为 currentNodeCount

            lct.setNodeValue(currentNodeCount, newNodeValue); // 设置新节点的权值
            lct.link(parentNodeId, currentNodeCount);         // 将新节点作为 parentNodeId 的子节点连接

        } else if (operationType == '?') {
            int u, v, k;
            cin >> u >> v >> k; // 读取查询的起点、终点和目标子段和

            // 特殊情况：k=0。题目允许空子段，其和为0，因此总是存在。
            if (k == 0) {
                cout << "YES\n";
                continue;
            }

            // 查询 u 到 v 路径上的聚合信息
            PathInfo pathResult = lct.queryPath(u, v);

            // 核心判断：因为权值只有+1和-1，路径上所有非空子段和会构成一个连续的整数区间。
            // 这个区间就是 [pathResult.subMin, pathResult.subMax]。
            // 因此，只需判断 k 是否落在这个区间内即可。
            if (pathResult.subMin <= k && k <= pathResult.subMax) {
                cout << "YES\n";
            } else {
                cout << "NO\n";
            }
        }
    }
}

int main() {
    solve();
    return 0;
}