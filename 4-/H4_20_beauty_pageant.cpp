/*
20. 选美比赛
【问题描述】

在选美大奖赛的半决赛现场，有n名选手（2<n<100）参加比赛。比赛结束时，要在现场按照选手的出场顺序宣布最后名次，获得相同分数的选手具有相同的名次，名次连续编号，不用考虑同名次的选手人数。如：

选手数量：  7

选手得分：  5，3，4，7，3，5，6

宣布名次：  3，5，4，1，5，3，2

请编程帮助大奖赛组委会完成半决赛的评分排名工作。

【输入形式】

选手数量：7

选手得分：5 3 4 7 3 5 6

【输出形式】

选手的排名：3 5 4 1 5 3 2

【样例输入】

7
5 3 4 7 3 5 6
【样例输出】

3 5 4 1 5 3 2
*/
/*
 * 核心思路:
 * 这个问题要求我们为一系列分数评定名次，规则是分数越高，名次越靠前（数字越小），
 * 且分数相同的选手名次相同。这是一种典型的“密集排名”（Dense Ranking）。
 *
 * 算法步骤如下：
 * 1. **数据存储**: 首先，读取并存储所有选手的原始得分，保持其出场顺序不变。
 *
 * 2. **确定排名规则**: 为了建立分数到名次的映射关系，我们需要知道所有不重复的分数，
 * 并按从高到低的顺序给它们分配名次。
 * a. 复制一份原始得分列表。
 * b. 对复制的列表进行排序（例如，升序）。
 * c. 使用 `std::unique` 算法移除排序后列表中的重复分数，得到一个包含所有唯一分数的有序列表。
 * d. 倒序遍历这个唯一分数列表，为每个分数分配一个从1开始递增的名次。
 * e. 将这个“分数 -> 名次”的对应关系存储在一个哈希表（`std::unordered_map`）中，以便快速查询。
 *
 * 3. **生成最终名次**: 遍历原始的、保持出场顺序的得分列表。对于每个得分，
 * 在哈希表中查找其对应的名次，并将该名次存入最终结果列表。
 *
 * 4. **输出**: 按格式要求打印最终名次列表。
 *
 * 这种方法将“计算排名”和“按顺序输出”两个步骤解耦，使得逻辑非常清晰。
 * 使用STL算法和数据结构（如 `vector`, `unordered_map`, `sort`, `unique`）可以写出非常简洁和高效的代码。
 *
 * 时间复杂度: O(N log N)
 * 主要瓶颈在于对得分列表的排序操作。后续建立哈希表和查询操作的平均时间复杂度都接近 O(N)。
 *
 * 空间复杂度: O(N)
 * 需要额外的空间来存储原始得分、得分副本、哈希表以及最终的名次列表。
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

// 遵循规范，直接使用 std 命名空间
using namespace std;

int main() {
    // 提高IO性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    // 步骤1: 读取并存储原始得分，保持出场顺序
    vector<int> originalScores(n);
    for (int i = 0; i < n; ++i) {
        cin >> originalScores[i];
    }

    // 步骤2: 确定排名规则
    // a. 复制一份得分用于处理，不破坏原始顺序
    vector<int> uniqueScores = originalScores;

    // b. 排序
    sort(uniqueScores.begin(), uniqueScores.end());

    // c. 移除重复元素，`unique`会把不重复的元素移到前面，并返回尾后迭代器
    uniqueScores.erase(unique(uniqueScores.begin(), uniqueScores.end()), uniqueScores.end());

    // d. & e. 建立分数到名次的映射
    // 使用 unordered_map 以获得平均 O(1) 的查询效率
    unordered_map<int, int> scoreToRankMap;
    int currentRank = 1;
    // 从高分到低分（即从uniqueScores的末尾到开头）遍历，分配名次
    for (int i = uniqueScores.size() - 1; i >= 0; --i) {
        int score = uniqueScores[i];
        scoreToRankMap[score] = currentRank;
        currentRank++;
    }

    // 步骤3: 根据原始顺序生成最终名次列表
    vector<int> finalRanks(n);
    for (int i = 0; i < n; ++i) {
        finalRanks[i] = scoreToRankMap[originalScores[i]];
    }

    // 步骤4: 输出结果
    for (int i = 0; i < n; ++i) {
        cout << finalRanks[i] << (i == n - 1 ? "" : " ");
    }
    cout << endl;

    // --- main 函数的测试用例说明 ---
    // 本程序根据题目要求，从标准输入动态读取测试数据。
    //
    // 样例输入:
    // 7
    // 5 3 4 7 3 5 6
    //
    // 内部处理流程:
    // 1. originalScores = {5, 3, 4, 7, 3, 5, 6}
    // 2. uniqueScores 经过排序去重后变为 {3, 4, 5, 6, 7}
    // 3. 建立 scoreToRankMap:
    //    - score 7 -> rank 1
    //    - score 6 -> rank 2
    //    - score 5 -> rank 3
    //    - score 4 -> rank 4
    //    - score 3 -> rank 5
    // 4. 遍历 originalScores 并查询 map，得到 finalRanks = {3, 5, 4, 1, 5, 3, 2}
    //
    // 最终输出:
    // 3 5 4 1 5 3 2

    return 0;
}