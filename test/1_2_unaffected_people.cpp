/*
 * 核心思路:
 * 这个问题旨在识别出所有“未受激励”的学生。一名学生A(时间tA, 分数sA)会激励学生B(时间tB, 分数sB)的条件是 tA <= tB 且 sA >= sB，
 * 并且两者的时间和分数不完全相同。一个“未受激励”的学生，是指不存在任何其他学生能激励他/她。
 *
 * 暴力解法（O(N^2)）对于百万级的数据量显然会超时。观察到时间和分数的范围相对较小（t <= 240, s <= 500），
 * 这提示我们可以利用空间换时间，通过预处理来优化判断过程。
 *
 * 我们可以将问题转化一下：一个学生P(t, s)未受激励，等价于对于任何其他学生P'(t', s')，都不满足 t' <= t 且 s' >= s (P和P'不完全相同)。
 * 这意味着，P要想不被激励，他的分数s必须：
 * 1. 严格大于所有用时比他更短的学生中的最高分。
 * 2. 是所有用时与他相同的学生中的最高分。
 *
 * 基于这个思路，我们可以设计如下高效算法：
 * 1. 第一次遍历所有学生，使用一个数组 `maxScoreForTime` 记录在每个特定时间点上出现的最高分。
 * 2. 第二次遍历时间轴（1到240），计算出一个前缀最大值数组 `maxScoreUpToTime`，其中 `maxScoreUpToTime[t]` 表示在
 * 时间点 t 及之前所有学生中的最高分。
 * 3. 第三次遍历原始学生列表，对于每个学生(id, t, s)，利用预处理好的两个数组进行O(1)的判断：
 * - 该学生的分数 `s` 是否等于 `maxScoreForTime[t]`（即他是该时间点的最高分）。
 * - 该学生的分数 `s` 是否严格大于 `maxScoreUpToTime[t-1]`（即他比所有用时更短的人都强）。
 * 同时满足这两个条件的学生就是未受激励的“大佬”。
 *
 * 时间复杂度: O(N + T_max)，其中N是学生数量，T_max是最大做题时间（240）。遍历学生列表两次（读入、检查），遍历时间轴一次。
 * 空间复杂度: O(N + T_max)，需要存储所有学生信息，以及两个辅助数组。
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

// 为了代码清晰，定义一个结构体来存储学生信息
struct Student {
    int id;
    int time;
    int score;
};

int main() {
    // 优化C++标准流的输入输出，应对大量数据
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int studentCount;
    std::cin >> studentCount;

    // 存储所有学生的信息
    std::vector<Student> students;
    students.reserve(studentCount);

    // `maxScoreForTime[t]` 存储用时为t的学生中的最高分
    // 时间范围 1 <= t <= 240，数组大小设为241方便使用1-based索引
    std::vector<int> maxScoreForTime(241, 0);

    // 第一次遍历：读取输入并填充 maxScoreForTime 数组
    for (int i = 1; i <= studentCount; ++i) {
        int t, s;
        std::cin >> t >> s;
        students.push_back({i, t, s});
        // 更新在时间t上出现的最高分
        if (s > maxScoreForTime[t]) {
            maxScoreForTime[t] = s;
        }
    }

    // `maxScoreUpToTime[t]` 存储用时小于等于t的所有学生中的最高分（前缀最大值）
    std::vector<int> maxScoreUpToTime(241, 0);

    // 第二次遍历（时间轴）：计算前缀最大值
    // 这个数组帮助我们快速判断一个学生的分数是否高于所有用时比他短的学生
    for (int t = 1; t <= 240; ++t) {
        maxScoreUpToTime[t] = std::max(maxScoreUpToTime[t - 1], maxScoreForTime[t]);
    }

    std::vector<int> unaffectedStudentIds;

    // 第三次遍历：检查每个学生是否未受激励
    for (const auto& student : students) {
        // 检查条件1：该学生的分数必须是所有用时为 student.time 的学生中的最高分
        bool isTopForCurrentTime = (student.score == maxScoreForTime[student.time]);

        // 检查条件2：该学生的分数必须严格大于所有用时比他短的学生中的最高分
        // 如果 student.time 是1，则不存在用时比他更短的人，此条件天然满足
        bool isBetterThanAllFaster = (student.time == 1) || (student.score > maxScoreUpToTime[student.time - 1]);

        // 同时满足两个条件，说明该学生未受任何其他学生激励
        if (isTopForCurrentTime && isBetterThanAllFaster) {
            unaffectedStudentIds.push_back(student.id);
        }
    }

    // 按题目要求，按id升序输出
    std::sort(unaffectedStudentIds.begin(), unaffectedStudentIds.end());

    for (int id : unaffectedStudentIds) {
        std::cout << id << "\n";
    }

    return 0;
}
