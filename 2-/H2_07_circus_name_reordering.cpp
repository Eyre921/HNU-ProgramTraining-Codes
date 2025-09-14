/*
 *
* 【问题描述】

你在信天翁马戏团（是的，它是由一群小丑组成）从事管理工作，你刚刚写完一个程序的输出是将他们的姓名按长度为非递减的方式排列，名称列表（使每名至少只要它之前的）。然而，你的老板不喜欢这种输出方式，而是希望输出出现更对称，较短的字符串在顶部和底部，而较长的字符串在中间。他的规则是，每一对名称都是在该列表的相对的两端，并且在该组中的第一个名字总是在列表的顶部。比如在下面的第一个例子中，Bo和Pat是第一对，Jean和Kevin是第二对，等等。

【输入形式】

输入由1到多个字符串集合组成，最后一行为0表示输入结束，每个集合开始于一个整数n，表示该集合字符串的个数，接下来n行由n个字符串按长度非递减的方式排列，每个集合至少包含一个但不超过15个字符串，每个字符串不超过25个字符。

【输出形式】

对于每个集合，第一行输出"set-n", n从1开始，接下来的若干行对应输入每个集合重新排列的结果，如样例所示。

【样例输入】

7
Bo
Pat
Jean
Kevin
Claude
William
Marybeth
6
Jim
Ben
Zoe
Joey
Frederick
Annabelle
5
John
Bill
Fran
Stan
Cece
0
【样例输出】

set-1
Bo
Jean
Claude
Marybeth
William
Kevin
Pat
set-2
Jim
Zoe
Frederick
Annabelle
Joey
Ben
set-3
John
Fran
Cece
Stan
Bill
 */

/**
 * @file circus_name_reordering.cpp
 * @brief 解决方案：对称排列马戏团成员姓名
 *
 * 核心思路:
 * 题目要求将一个按长度排好序的姓名列表重新排列成一个对称的结构：
 * 短的姓名在顶部和底部，长的在中间。
 *
 * 观察输入和输出的规律可以发现：
 * - 输入列表中的第1, 3, 5, ...个姓名（即奇数位置的姓名），
 * 构成了输出列表的上半部分，且顺序不变。
 * - 输入列表中的第2, 4, 6, ...个姓名（即偶数位置的姓名），
 * 构成了输出列表的下半部分，但顺序是颠倒的。
 *
 * 基于这个观察，我们可以设计一个简单高效的两步打印算法：
 * 1.  首先，读取给定集合的所有n个姓名，并存储在一个 `std::vector` 中。
 * 2.  第一遍遍历：从头开始，步长为2（即访问索引 0, 2, 4, ...），
 * 顺序打印这些姓名。这就构成了输出列表的“上半部分”。
 * 3.  第二遍遍历：从列表的末尾开始，同样以步长为2向后遍历（即访问索引
 * n-1或n-2, n-3或n-4, ...），逆序打印这些姓名。
 * 这就构成了输出列表的“下半部分”。
 *
 * 这种方法不需要创建额外的 `vector` 来存储输出结果，直接根据规则打印，
 * 既节省了空间，又简化了逻辑。
 *
 * 时间复杂度: O(S)
 * 其中S是所有输入字符串的总字符数。对于每个集合，我们需要读取n个字符串并遍历它们两次。
 *
 * 空间复杂度: O(S_max)
 * 其中S_max是单个集合中所有字符串的总字符数。我们需要一个 `vector` 来存储一个集合
 * 的所有字符串。
 */

#include <iostream> // 用于标准输入输出 (cin, cout)
#include <vector>   // 用于使用vector容器
#include <string>   // 用于使用string类

// 使用标准命名空间，简化代码
using namespace std;

void solve() {
    int stringCount;
    int setCounter = 1; // 用于追踪 "set-n" 的编号

    // 循环读取每组测试数据，直到输入的数量为0
    while (cin >> stringCount && stringCount != 0) {
        // 输出集合的头部信息
        cout << "set-" << setCounter++ << endl;

        // 创建一个vector来存储当前集合的所有字符串
        vector<string> names(stringCount);
        // 读取所有字符串
        for (int i = 0; i < stringCount; ++i) {
            cin >> names[i];
        }

        // --- 核心重排逻辑 ---

        // 第一步：顺序打印奇数位置的姓名（索引为 0, 2, 4, ...）
        // 这些姓名构成新列表的顶部
        for (int i = 0; i < stringCount; i += 2) {
            cout << names[i] << endl;
        }

        // 第二步：逆序打印偶数位置的姓名（索引为 1, 3, 5, ...）
        // 这些姓名构成新列表的底部
        //
        // 确定起始索引：
        // - 如果总数是偶数，最后一个偶数位置的索引是 stringCount - 1
        // - 如果总数是奇数，最后一个偶数位置的索引是 stringCount - 2
        int startIdx = (stringCount % 2 == 0) ? (stringCount - 1) : (stringCount - 2);

        for (int i = startIdx; i >= 1; i -= 2) {
            cout << names[i] << endl;
        }
    }
}

int main() {
    // 优化C++标准流的IO性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 调用解决问题的函数
    solve();

    // 样例测试
    // 【样例输入】
    // 7
    // Bo
    // Pat
    // Jean
    // Kevin
    // Claude
    // William
    // Marybeth
    // 6
    // Jim
    // Ben
    // Zoe
    // Joey
    // Frederick
    // Annabelle
    // 5
    // John
    // Bill
    // Fran
    // Stan
    // Cece
    // 0
    // 【预期输出】
    // set-1
    // Bo
    // Jean
    // Claude
    // Marybeth
    // William
    // Kevin
    // Pat
    // set-2
    // Jim
    // Zoe
    // Frederick
    // Annabelle
    // Joey
    // Ben
    // set-3
    // John
    // Fran
    // Cece
    // Stan
    // Bill

    return 0;
}