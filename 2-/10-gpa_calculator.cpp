/*10. 绩点计算
【问题描述】

学校对本科生的成绩施行绩点制（GPA）。将学生的实际考分根据不同学科的不同学分按一定的公式进行计算。规定如下：

实际成绩        绩点

90-100          4.0

85-89            3.7

82-84            3.3

78-81            3.0

75-77            2.7

72-74            2.3

68-71            2.0

64-67            1.5

60-63            1.0

60以下            0

1. 一门课程的学分绩点=该课绩点*该课学分

2. 总评绩点=所有学科绩点之和/所有课程学分之和

现要求你编程求出某人的总评绩点(GPA)

【输入形式】

第一行 总的课程数n

第二行 相应课程的学分（两个学分间用空格隔开）

第三行 对应课程的实际得分

此处输入的所有数字均为整数

【输出形式】

输出有一行，总评绩点，保留两位小数

【样例输入】

5
4 3 4 2 3
91 88 72 69 56
【样例输出】

2.52
*/

/**
 * @file gpa_calculator.cpp
 * @brief 解决方案：根据课程学分和分数计算总评绩点（GPA）
 *
 * 核心思路:
 * 1.  模块化设计：将核心的“根据分数获取绩点”逻辑封装到一个独立的辅助函数
 * `getGpaPoint(int score)` 中。这使得主函数逻辑更清晰，代码更易于理解和维护。
 * `if-else if` 结构是实现分数到绩点映射的最直接高效的方式。
 *
 * 2.  累加计算：
 * a.  在主函数中，首先读取课程总数n，然后分别将n个课程的学分和分数读入
 * 两个 `std::vector` 中。
 * b.  定义两个 `double` 类型的累加器变量：`totalWeightedPoints` 用于累加
 * “每门课的绩点 * 该课学分”，`totalCredits` 用于累加总学分。
 * 使用 `double` 类型可以避免在计算过程中出现精度损失。
 * c.  遍历所有课程，对于每门课程：
 * i.   调用 `getGpaPoint()` 函数获取当前课程分数对应的绩点。
 * ii.  计算该课程的学分绩点（绩点 * 学分），并累加到 `totalWeightedPoints`。
 * iii. 将该课程的学分累加到 `totalCredits`。
 *
 * 3.  结果计算与输出：
 * a.  所有课程遍历完毕后，总评绩点（GPA）即为 `totalWeightedPoints / totalCredits`。
 * b.  在进行除法前，需要考虑总学分为0的极端情况，以避免除零错误。
 * c.  使用 `<iomanip>` 库中的 `std::fixed` 和 `std::setprecision(2)`
 * 来格式化输出，确保结果保留两位小数。
 *
 * 时间复杂度: O(N)
 * 其中N是课程的总数。我们需要遍历一次所有课程来计算总和。
 *
 * 空间复杂度: O(N)
 * 需要使用两个 `vector` 来存储N门课程的学分和分数。
 */

#include <iostream>   // 用于标准输入输出 (cin, cout)
#include <vector>     // 用于使用vector容器
#include <iomanip>    // 用于使用fixed和setprecision格式化输出

// 使用标准命名空间，简化代码
using namespace std;

/**
 * @brief 根据实际考分返回对应的绩点。
 * @param score 学生的实际考分 (整数)。
 * @return 对应的绩点 (浮点数)。
 */
double getGpaPoint(int score) {
    if (score >= 90) return 4.0;
    if (score >= 85) return 3.7;
    if (score >= 82) return 3.3;
    if (score >= 78) return 3.0;
    if (score >= 75) return 2.7;
    if (score >= 72) return 2.3;
    if (score >= 68) return 2.0;
    if (score >= 64) return 1.5;
    if (score >= 60) return 1.0;
    return 0.0;
}

int main() {
    // 优化C++标准流的IO性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int courseCount;
    cin >> courseCount;

    // 如果课程数为0或负数，直接退出
    if (courseCount <= 0) {
        cout << fixed << setprecision(2) << 0.0 << endl;
        return 0;
    }

    // 使用vector存储所有课程的学分
    vector<int> credits(courseCount);
    double totalCredits = 0.0;
    for (int i = 0; i < courseCount; ++i) {
        cin >> credits[i];
        totalCredits += credits[i];
    }

    // 使用vector存储所有课程的实际得分
    vector<int> scores(courseCount);
    for (int i = 0; i < courseCount; ++i) {
        cin >> scores[i];
    }

    // 初始化总学分绩点之和
    double totalWeightedPoints = 0.0;

    // 遍历每门课程，计算总的学分绩点
    for (int i = 0; i < courseCount; ++i) {
        // 获取当前课程分数对应的绩点
        double gpaPoint = getGpaPoint(scores[i]);
        // 累加学分绩点 (绩点 * 学分)
        totalWeightedPoints += gpaPoint * credits[i];
    }

    double finalGpa = 0.0;
    // 健壮性检查：只有在总学分不为0时才进行除法运算
    if (totalCredits > 0) {
        finalGpa = totalWeightedPoints / totalCredits;
    }

    // 设置输出格式为保留两位小数并输出最终的GPA
    cout << fixed << setprecision(2) << finalGpa << endl;

    return 0;
}