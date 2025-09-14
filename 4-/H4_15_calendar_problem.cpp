/*
15. 日历问题
【问题描述】

在我们现在使用的日历中, 闰年被定义为能被4整除的年份，但是能被100整除而不能被400整除的年是例外，它们不是闰年。例如：1700, 1800, 1900 和 2100 不是闰年，而 1600, 2000 和 2400是闰年。 给定从公元2000年1月1日开始逝去的天数，你的任务是给出这一天是哪年哪月哪日星期几。

【输入形式】

输入包含若干行，每行包含一个正整数，表示从2000年1月1日开始逝去的天数。输入最后一行是−1, 不必处理。可以假设结果的年份不会超过9999。

【输出形式】

对每个测试样例，输出一行，该行包含对应的日期和星期几。格式为“YYYY-MM-DD DayOfWeek”, 其中 “DayOfWeek” 必须是下面中的一个： "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday" and "Saturday“。

【样例输入】

1730
1740
1750
1751
-1
【样例输出】

2004-09-26 Sunday
2004-10-06 Wednesday
2004-10-16 Saturday
2004-10-17 Sunday
*/
/**
 * @file calendar_problem.cpp
 * @brief 根据从2000-01-01逝去的天数计算具体日期和星期
 *
 * 核心思路:
 * 本程序采用日期模拟法。从一个已知的基准日期（2000年1月1日，星期六）开始，
 * 将给定的逝去天数`d`逐次分配到年、月、日中。
 * 1.  **年份计算**: 循环地从总天数`d`中减去每年的天数（365或366），直到剩余天数不足一年。
 * 这个过程依赖一个精确的isLeap()函数来判断闰年。
 * 2.  **月份计算**: 在确定年份后，继续从剩余天数中减去每个月的天数，直到剩余天数不足一月。
 * 3.  **日期计算**: 经过上述两步，最后剩余的天数（0-indexed）加1即为当月的日期。
 * 4.  **星期计算**: 利用基准日（星期六，索引为6）和总逝去天数`d`，通过模7运算 `(6 + d) % 7` 直接得到目标日的星期索引。
 * * 程序通过辅助函数和常量数组来封装逻辑，提高了代码的可读性和可维护性。
 *
 * 时间复杂度: O(Y)，其中Y为输入天数所跨越的年份数。对于题目给定的年份上限9999，
 * 这是一个非常高效的算法，远快于O(d)。
 *
 * 空间复杂度: O(1)，仅使用少量固定大小的存储空间。
 */

#include <iostream>
#include <string>
#include <vector>
#include <iomanip> // 用于 std::setw 和 std::setfill

// 启用C++标准库，无需显式使用 std:: 前缀
using namespace std;

/**
 * @brief 判断指定年份是否为闰年
 * @param year 待判断的年份
 * @return 如果是闰年返回true，否则返回false
 */
bool isLeap(int year) {
    // 闰年规则：能被4整除但不能被100整除，或者能被400整除
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

/**
 * @brief 根据逝去的天数计算并打印日期
 * @param totalElapsedDays 从2000-01-01开始逝去的天数
 */
void solveDate(int totalElapsedDays) {
    if (totalElapsedDays < 0) return;

    // --- 日期计算 ---
    long long days = totalElapsedDays;
    int year = 2000;

    // 1. 确定年份
    while (true) {
        int daysInYear = isLeap(year) ? 366 : 365;
        if (days >= daysInYear) {
            days -= daysInYear;
            year++;
        } else {
            break;
        }
    }

    // 2. 确定月份
    // daysInMonth[0]为平年, daysInMonth[1]为闰年。月份从1开始索引。
    const int daysInMonth[2][13] = {
        {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
    };
    int month = 1;
    int leapIndex = isLeap(year) ? 1 : 0; // 决定使用平年还是闰年的月份表

    while (true) {
        int daysInCurrentMonth = daysInMonth[leapIndex][month];
        if (days >= daysInCurrentMonth) {
            days -= daysInCurrentMonth;
            month++;
        } else {
            break;
        }
    }

    // 3. 确定日期
    // 剩余的 `days` 是0-indexed的，所以实际日期是 days + 1
    int day = days + 1;

    // --- 星期计算 ---
    // 基准日 2000-01-01 是星期六。
    // 星期日=0, 星期一=1, ..., 星期六=6
    const string dayNames[] = {
        "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
    };
    // (基准日星期索引 + 总逝去天数) % 7
    int dayOfWeekIndex = (6 + totalElapsedDays) % 7;
    string dayOfWeek = dayNames[dayOfWeekIndex];

    // --- 格式化输出 ---
    cout << year << "-"
         << setfill('0') << setw(2) << month << "-"
         << setfill('0') << setw(2) << day << " "
         << dayOfWeek << endl;
}

int main() {
    // 优化I/O性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    /*
     * --- 测试用例说明 ---
     * 【样例输入】
     * 1730
     * 1740
     * 1750
     * 1751
     * -1
     *
     * 【样例输出】
     * 2004-09-26 Sunday
     * 2004-10-06 Wednesday
     * 2004-10-16 Saturday
     * 2004-10-17 Sunday
     */

    int elapsedDays;
    // 循环读取输入，直到遇见-1
    while (cin >> elapsedDays && elapsedDays != -1) {
        solveDate(elapsedDays);
    }

    return 0;
}