/*
9. 挖掘机技术哪家强
【问题描述】

为了用事实说明挖掘机技术到底哪家强，组织一场挖掘机技能大赛。现请你根据比赛结果统计出技术最强的那个学校。

【输入形式】

输入在第1行给出不超过105的正整数N，即参赛人数。随后N行，每行给出一位参赛者的信息和成绩，包括其所代表的学校的编号、及其比赛成绩（百分制），中间以空格分隔。

【输出形式】

在一行中给出总得分最高的学校的编号、及其总分，中间以空格分隔。题目保证答案唯一，没有并列。

【样例输入】

6
3 65
2 80
1 100
2 70
3 40
3 0

【样例输出】

2 150

【问题说明】

建议练习使用STL中的map
 */
/**
 * @file ExcavatorSchoolContest.cpp
 * @brief 核心思路:
 * 本题要求统计N个参赛选手的成绩，找出总分最高的学校。
 * 1. 使用哈希表 (std::unordered_map) 来存储每个学校的总得分。键(key)为学校ID，值(value)为该学校的累计总分。
 * 选择 `unordered_map` 而非题目建议的 `map` 是基于性能考量：`unordered_map` 提供了平均O(1)的插入和访问时间复杂度，
 * 而 `map` (基于红黑树) 是O(logK)（K为不同学校的数量）。在数据量较大时，`unordered_map` 的性能优势更明显。
 * 2. 为了追求最高效率，我们在一次遍历中完成所有操作：
 * - 循环N次，读取每个选手的学校ID和分数。
 * - 将分数累加到对应学校ID在哈希表中的总分上。`unordered_map` 的 `[]` 操作符能优雅地处理学校ID首次出现的情况
 * （若ID不存在，会自动创建条目并将其值初始化为0）。
 * - 在每次更新总分后，立即与当前记录的最高分进行比较。如果当前学校的总分超过了已知的最高分，
 * 就更新最高分和对应的学校ID。
 * 3. 这种单遍扫描 (single-pass) 的方法避免了在读完所有数据后再进行一次遍历来寻找最大值，从而简化了逻辑并提升了整体效率。
 * 4. 遍历结束后，我们记录下的最高分和对应的学校ID即为最终答案。
 *
 * @time_complexity O(N)，其中 N 是参赛人数。我们需要遍历所有参赛者，每次操作（哈希表访问和更新）的平均时间是 O(1)。
 * @space_complexity O(K)，其中 K 是不同学校的数量 (K <= N)。哈希表需要空间来存储每个不同学校的总分。
 */

#include <iostream>
#include <unordered_map>

// 使用标准命名空间，以符合题目要求简化代码
using namespace std;

int main() {
    // 优化C++标准流的I/O性能，对于处理大量输入数据（N可达10^5）至关重要。
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int numContestants;
    // 读取第一行的参赛总人数
    cin >> numContestants;

    // 使用unordered_map来存储每个学校ID -> 总分 的映射
    // 键是int类型的学校ID，值是int类型的累计分数
    unordered_map<int, int> schoolScores;

    // 用于追踪最高分和对应的学校ID
    int winningSchoolId = -1;
    // 初始化最高分为0，因为分数都是非负的（百分制）
    int maxScore = 0;

    // 在单次循环中处理所有参赛者数据，完成分数累加和最值查找
    for (int i = 0; i < numContestants; ++i) {
        int schoolId;
        int score;
        cin >> schoolId >> score;

        // 将当前选手的得分累加到其所在学校的总分中。
        // 如果map中不存在该schoolId，`[]`操作符会自动创建条目并初始化为0，然后再进行加法。
        schoolScores[schoolId] += score;

        // 在更新分数后，立即检查是否产生了新的最高分。
        // 这种方式可以避免在所有数据读完后，再对map进行一次遍历来找最大值，效率更高。
        if (schoolScores[schoolId] > maxScore) {
            maxScore = schoolScores[schoolId];
            winningSchoolId = schoolId;
        }
    }

    // 测试用例演示：
    // 输入:
    // 6
    // 3 65
    // 2 80
    // 1 100
    // 2 70
    // 3 40
    // 3 0

    // 输出最终结果：总得分最高的学校ID和它的总分
    cout << winningSchoolId << " " << maxScore << endl;
    // 预期输出: 2 150

    return 0;
}