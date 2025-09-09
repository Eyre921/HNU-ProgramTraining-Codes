 /*6. 字符串数字置换

【问题描述】

从键盘接收用户输入的字符串, 对用户输入的每个字符串的处理是：将字符串内的每一个十进制数字字符置换成下列表格中右边所对应的一个字符串（所有其他字符不变），然后将转换的结果显示在屏幕上；并分别计算每个数字的置换次数。

十进制数字字符

置换成

0

(Zero)

1

(One)

2

(Two)

3

(Three)

4

(Four)

5

(Five)

6

(Six)

7

(Seven)

8

(Eight)

9

(Nine)

例如，若用户输入的字符串为

                 Page112-Line3，

则程序5的输出是：

                 Page(One) (One) (Two)-Line(Three),

数字0到9的置换次数分别是  0 2 1 1 0 0 0 0 0 0

【输入形式】

输入一行字符串，其中可包含字母、数字、空格或其他符号（英文）

【输出形式】

第一行为将字符串中的数字转换为表格中的内容后输出

第二行为数字0~9被转换的次数

【样例输入】

Page112-Line3

【样例输出】

Page(One)(One)(Two)-Line(Three)

0 2 1 1 0 0 0 0 0 0
*/
/**
 * @file String_Digit_Replacement
 * @brief 核心思路:
 * 本程序旨在解决字符串中数字字符的替换与计数问题。
 * 1. 使用一个 `std::vector<string>` 作为常量映射表，存储 '0' 到 '9' 对应的替换字符串，
 * 这样可以通过数字的整数值直接索引，实现 O(1) 的查找效率。
 * 2. 使用一个 `std::vector<int>` (大小为10) 来记录0到9每个数字出现的次数。
 * 3. 遍历输入的字符串，对每个字符进行判断：
 * - 如果是数字，则从映射表中获取对应的英文字符串追加到结果中，并更新该数字的计数器。
 * - 如果不是数字，则直接将原字符追加到结果中。
 * 4. 为了高效地构建最终字符串，我们使用 `std::stringstream`，它在内部优化了内存管理，
 * 避免了在循环中反复拼接 `std::string` 可能导致的多次内存重分配和拷贝，从而提升性能。
 * 5. 遍历结束后，先输出由 `stringstream` 构建的新字符串，然后按格式输出数字出现次数的统计结果。
 *
 * @time_complexity O(N)，其中 N 是输入字符串的长度。我们只需要单次遍历字符串。
 * @space_complexity O(M)，其中 M 是结果字符串的长度。主要空间开销来自于存储结果的 `stringstream`。在最坏情况下（所有字符都是数字），
 * 结果字符串的长度可能数倍于原字符串。计数器和映射表的空间是 O(1) 的常量空间。
 */

#include <iostream>
#include <string>
#include <vector>
#include <cctype>   // 包含 isdigit 函数，用于字符判断
#include <sstream>  // 包含 stringstream，用于高效构建字符串

// 使用标准命名空间，以符合题目要求简化代码
using namespace std;

int main() {
    // 优化C++标准流的I/O性能，这是高性能计算中的一个良好实践
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 定义常量映射表：存储0-9数字对应的英文字符串
    // 使用 const vector 可以确保数据不会被意外修改，并且由于其大小和内容固定，编译器可能进行优化。
    const vector<string> digitMap = {
        "(Zero)", "(One)", "(Two)", "(Three)", "(Four)",
        "(Five)", "(Six)", "(Seven)", "(Eight)", "(Nine)"
    };

    // 定义计数器：初始化一个大小为10的向量，所有元素为0，用于统计0-9的出现次数
    vector<int> digitCounts(10, 0);

    string inputLine;
    // 从标准输入读取一整行，这可以正确处理包含空格的输入
    getline(cin, inputLine);

    // 使用 stringstream 高效构建结果字符串
    stringstream resultStream;

    // 遍历输入字符串中的每一个字符
    // 使用基于范围的for循环是现代C++的推荐做法，比传统的下标循环更简洁、更安全。
    for (char ch : inputLine) {
        // 判断当前字符是否是十进制数字 ('0'-'9')
        if (isdigit(ch)) {
            // 将字符数字转换为整数索引 (例如 '1' -> 1)
            int digitValue = ch - '0';
            // 从映射表中查找对应的字符串并追加到结果流
            resultStream << digitMap[digitValue];
            // 对应数字的计数器加一
            digitCounts[digitValue]++;
        } else {
            // 如果不是数字，直接将原字符追加到结果流
            resultStream << ch;
        }
    }

    // 测试用例演示：
    // 输入: Page112-Line3

    // 输出第一行：转换后的字符串
    // resultStream.str() 会返回流中内容的一个拷贝
    cout << resultStream.str() << endl;
    // 预期输出: Page(One)(One)(Two)-Line(Three)

    // 输出第二行：0-9的置换次数
    // 循环遍历计数器向量并输出，注意处理元素之间的空格
    for (size_t i = 0; i < digitCounts.size(); ++i) {
        cout << digitCounts[i] << (i == digitCounts.size() - 1 ? "" : " ");
    }
    cout << endl;
    // 预期输出: 0 2 1 1 0 0 0 0 0 0

    return 0;
}