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

/*
 * 核心思路：
 * 1. 构建动态树结构，每次添加边时扩展树
 * 2. 对于查询操作，先找到u到v的路径，然后使用前缀和+哈希表检查是否存在目标子段和
 * 3. 路径查询使用LCA（最近公共祖先）算法，通过DFS预处理父节点关系
 * 4. 子段和检查：如果存在前缀和prefix[i]和prefix[j]使得prefix[j]-prefix[i]=k，则存在目标子段
 *
 * 时间复杂度：O(n * path_length)，其中path_length平均为O(log n)
 * 空间复杂度：O(n)
 */

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

class OmskMetro {
private:
    vector<vector<int>> adjacencyList;  // 邻接表存储树结构
    vector<int> nodeValues;             // 存储每个节点的权值
    vector<int> parentNodes;            // 存储每个节点的父节点
    vector<int> nodeDepths;             // 存储每个节点的深度
    int totalNodes;                     // 当前总节点数

public:
    OmskMetro() {
        totalNodes = 1;
        // 初始化编号为1、权值为1的根节点
        adjacencyList.resize(1, vector<int>());
        nodeValues.resize(1, 1);
        parentNodes.resize(1, -1);  // 根节点无父节点
        nodeDepths.resize(1, 0);    // 根节点深度为0
    }

    // 添加新边操作
    void addEdge(int sourceNode, int targetValue) {
        totalNodes++;

        // 扩展所有存储结构
        adjacencyList.resize(totalNodes);
        nodeValues.resize(totalNodes);
        parentNodes.resize(totalNodes);
        nodeDepths.resize(totalNodes);

        int newNodeIndex = totalNodes - 1;

        // 建立双向连接
        adjacencyList[sourceNode].push_back(newNodeIndex);
        adjacencyList[newNodeIndex].push_back(sourceNode);

        // 设置新节点属性
        nodeValues[newNodeIndex] = targetValue;
        parentNodes[newNodeIndex] = sourceNode;
        nodeDepths[newNodeIndex] = nodeDepths[sourceNode] + 1;
    }

    // 查找两点间的路径
    vector<int> findPath(int startNode, int endNode) {
        if (startNode == endNode) {
            return {startNode};
        }

        // 使路径上的两点深度相同
        vector<int> pathFromStart, pathFromEnd;
        int currentStart = startNode, currentEnd = endNode;

        // 将深度大的节点向上移动至同一深度
        while (nodeDepths[currentStart] > nodeDepths[currentEnd]) {
            pathFromStart.push_back(currentStart);
            currentStart = parentNodes[currentStart];
        }
        while (nodeDepths[currentEnd] > nodeDepths[currentStart]) {
            pathFromEnd.push_back(currentEnd);
            currentEnd = parentNodes[currentEnd];
        }

        // 同时向上查找最近公共祖先
        while (currentStart != currentEnd) {
            pathFromStart.push_back(currentStart);
            pathFromEnd.push_back(currentEnd);
            currentStart = parentNodes[currentStart];
            currentEnd = parentNodes[currentEnd];
        }

        // 添加公共祖先节点
        pathFromStart.push_back(currentStart);

        // 构建完整路径：起点->LCA->终点
        vector<int> completePath = pathFromStart;
        reverse(pathFromEnd.begin(), pathFromEnd.end());
        completePath.insert(completePath.end(), pathFromEnd.begin(), pathFromEnd.end());

        return completePath;
    }

    // 检查路径上是否存在和为k的子段
    bool hasSubsegmentWithSum(int startNode, int endNode, int targetSum) {
        vector<int> pathNodes = findPath(startNode, endNode);

        // 使用前缀和+哈希表的经典算法
        unordered_set<int> prefixSums;
        prefixSums.insert(0);  // 空子段的前缀和为0

        int currentPrefixSum = 0;
        for (int nodeIndex : pathNodes) {
            currentPrefixSum += nodeValues[nodeIndex];

            // 检查是否存在前缀和prefix，使得currentPrefixSum - prefix = targetSum
            if (prefixSums.count(currentPrefixSum - targetSum)) {
                return true;
            }

            prefixSums.insert(currentPrefixSum);
        }

        return false;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int operationCount;
    cin >> operationCount;

    OmskMetro metro;

    for (int i = 0; i < operationCount; i++) {
        char operationType;
        cin >> operationType;

        if (operationType == '+') {
            // 添加边操作
            int sourceNode, targetValue;
            cin >> sourceNode >> targetValue;
            sourceNode--;  // 转换为0基础索引
            metro.addEdge(sourceNode, targetValue);
        } else if (operationType == '?') {
            // 查询操作
            int startNode, endNode, targetSum;
            cin >> startNode >> endNode >> targetSum;
            startNode--; endNode--;  // 转换为0基础索引

            bool hasValidSubsegment = metro.hasSubsegmentWithSum(startNode, endNode, targetSum);
            cout << (hasValidSubsegment ? "YES" : "NO") << '\n';
        }
    }

    return 0;
}