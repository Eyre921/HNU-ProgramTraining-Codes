 
/*
 *
5. 成绩大排队
【问题描述】

读入n名学生的姓名、学号、成绩，分别输出成绩最高和成绩最低学生的姓名和学号。

【输入形式】

每个测试输入包含1个测试用例，格式为

 第1行：正整数n
  第2行：第1个学生的姓名 学号 成绩
  第3行：第2个学生的姓名 学号 成绩
  ... ... ...
  第n+1行：第n个学生的姓名 学号 成绩
其中姓名和学号均为不超过20个字符的字符串，成绩为0到100之间的一个整数，这里保证在一组测试用例中没有两个学生的成绩是相同的。

【输出形式】

对每个测试用例输出2行，第1行是成绩最高学生的姓名和学号，第2行是成绩最低学生的姓名和学号，字符串间有1空格。

【样例输入】

3
Joe Math990112 89
Mike CS991301 100
Mary EE990830 95
【样例输出】

Mike CS991301
Joe Math990112
 */
/*
 * @file: Grade_Ranking.cpp
 * @brief:
 * 本文件实现了"成绩大排队"问题的解决方案。程序会读取N名学生的信息，
 * 并快速找出成绩最高和最低的学生，然后输出他们的姓名和学号。
 *
 * @core_idea:
 * 采用单次遍历（Single Pass）的思路来寻找最高分和最低分的学生。
 * 1. 定义一个结构体 `Student` 来清晰地存储学生的姓名、学号和成绩。
 * 2. 读取第一个学生的信息，并将其同时初始化为 `maxScoreStudent` (最高分学生) 和 `minScoreStudent` (最低分学生)。
 * 3. 循环遍历剩余的 n-1 名学生。在每次迭代中：
 * a. 读取当前学生的信息。
 * b. 将当前学生的成绩与 `maxScoreStudent` 的成绩比较，如果更高，则更新 `maxScoreStudent`。
 * c. 将当前学生的成绩与 `minScoreStudent` 的成绩比较，如果更低，则更新 `minScoreStudent`。
 * 4. 遍历结束后，`maxScoreStudent` 和 `minScoreStudent` 就分别保存了成绩最高和最低的学生信息。
 * 这种方法只需对输入数据进行一次扫描，是解决此类问题的最优策略。
 *
 * @time_complexity: O(N)
 * 其中 N 是学生的数量。我们只需要遍历一次所有学生即可找到结果，因此算法的时间复杂度与学生数量成线性关系。
 *
 * @space_complexity: O(1)
 * 我们只需要常数级别的额外空间来存储最高分和最低分的学生信息，以及当前正在处理的学生信息。
 * 空间使用不随学生数量 N 的增长而增长，因此空间复杂度是常数级的。
 */

#include <iostream>
#include <string>

using namespace std;

// 定义学生结构体，用于清晰地表示每个学生的数据模型
struct Student {
    string name; // 学生姓名
    string id;   // 学生学号
    int score;   // 学生成绩
};

int main() {
    // 提高C++标准输入输出流的效率，对于大量IO操作的场景有明显优化
    // 这在高性能计算和算法竞赛中是常见的实践
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    // 从标准输入读取学生总数
    cin >> n;

    // 健壮性检查：处理n<=0的边界情况。虽然题目保证n为正整数，但健壮的代码应覆盖所有可能。
    if (n <= 0) {
        return 0; // 如果没有学生，程序直接退出
    }

    // 声明最高分和最低分学生的对象
    Student maxScoreStudent, minScoreStudent;

    // 关键初始化步骤：直接使用第一个学生的信息进行初始化。
    // 这避免了需要为成绩设置一个不可能的初始值（如-1或101），使代码更简洁、更鲁棒。
    cin >> minScoreStudent.name >> minScoreStudent.id >> minScoreStudent.score;
    maxScoreStudent = minScoreStudent; // 此时，第一个学生既是最高分也是最低分

    // 循环遍历剩下的 n-1 名学生
    // 从索引 1 开始，因为第 0 个学生已经用于初始化
    for (int i = 1; i < n; ++i) {
        Student currentStudent;
        // 读取当前学生的信息
        cin >> currentStudent.name >> currentStudent.id >> currentStudent.score;

        // 核心比较逻辑：更新最高分记录
        // 如果当前学生成绩更高，则用当前学生信息覆盖已记录的最高分学生信息
        if (currentStudent.score > maxScoreStudent.score) {
            maxScoreStudent = currentStudent;
        }

        // 核心比较逻辑：更新最低分记录
        // 如果当前学生成绩更低，则用当前学生信息覆盖已记录的最低分学生信息
        if (currentStudent.score < minScoreStudent.score) {
            minScoreStudent = currentStudent;
        }
    }

    // 按题目要求格式输出结果
    // 第1行是成绩最高学生的姓名和学号
    cout << maxScoreStudent.name << " " << maxScoreStudent.id << endl;
    // 第2行是成绩最低学生的姓名和学号
    cout << minScoreStudent.name << " " << minScoreStudent.id << endl;

    return 0;
}

/*
 * ========================================
 * 测试用例与执行分析
 * ========================================
 *
 * 【样例输入】
 * 3
 * Joe Math990112 89
 * Mike CS991301 100
 * Mary EE990830 95
 *
 * 【代码执行流程】
 * 1. 读取 n = 3。
 * 2. 读取第一个学生 "Joe Math990112 89"。
 * - 初始化 maxScoreStudent = { "Joe", "Math990112", 89 }
 * - 初始化 minScoreStudent = { "Joe", "Math990112", 89 }
 * 3. 循环开始 (i = 1, 共需遍历 n-1=2 次):
 * a. 第一次迭代 (i=1):
 * - 读取第二个学生 "Mike CS991301 100"。
 * - 检查最高分: currentStudent.score (100) > maxScoreStudent.score (89)，条件成立。
 * 更新 maxScoreStudent 为 Mike 的信息。
 * - 检查最低分: currentStudent.score (100) < minScoreStudent.score (89)，条件不成立。
 * minScoreStudent 保持不变。
 * b. 第二次迭代 (i=2):
 * - 读取第三个学生 "Mary EE990830 95"。
 * - 检查最高分: currentStudent.score (95) > maxScoreStudent.score (100)，条件不成立。
 * maxScoreStudent 保持不变 (仍为Mike)。
 * - 检查最低分: currentStudent.score (95) < minScoreStudent.score (89)，条件不成立。
 * minScoreStudent 保持不变 (仍为Joe)。
 * 4. 循环结束。
 * 5. 输出 maxScoreStudent 的姓名和学号: "Mike CS991301"。
 * 6. 输出 minScoreStudent 的姓名和学号: "Joe Math990112"。
 *
 * 【最终输出】
 * Mike CS991301
 * Joe Math990112
 */