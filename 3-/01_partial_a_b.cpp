/*
1. 部分A+B
【问题描述】

正整数A的“DA（为1位整数）部分”定义为由A中所有DA组成的新整数PA。例如：给定A = 3862767，DA = 6，则A的“6部分”PA是66，因为A中有2个6；给定A = 3862767，DA = 1，则A的“1部分”PA是0，因为A中有0个1。

现给定A、DA、B、DB，请编写程序计算PA + PB。

【输入形式】

输入在一行中依次给出A、DA、B、DB，中间以空格分隔，其中0 < A, B < 1012。

【输出形式】

在一行中输出PA + PB的值。

【样例输入】

3862767 6 13530293 3
【样例输出】

399
 */

/**
 * @file Partial_A+B.cpp
 * @brief 求解A的DA部分与B的DB部分之和 (PAT Basic 1016)
 *
 * 核心思路:
 * 1.  由于输入的整数A和B可能非常大（最多12位），超过了标准`int`的范围，但仍在`long long`的范围内。
 * 更稳妥且处理更方便的方式是，将它们作为字符串读入。
 * 2.  遍历数字字符串A中的每一个字符，如果该字符对应的数字等于DA，就将其计入PA。
 * 3.  构造PA的过程可以纯粹通过数学运算完成：每找到一个DA，就将当前的PA乘以10再加上DA。
 * 例如，对于A="3862767", DA=6：
 * - 找到第一个'6'，PA = 6
 * - 找到第二个'6'，PA = 6 * 10 + 6 = 66
 * 这种方法比先拼接字符串再转换为数字效率更高。如果A中不存在DA，则PA自然为0。
 * 4.  对B和DB执行相同的操作得到PB。
 * 5.  最后，计算PA + PB并输出结果。
 *
 * 时间复杂度: O(N + M)
 * 其中N是整数A的位数，M是整数B的位数。我们需要分别遍历两个数字字符串一次。
 *
 * 空间复杂度: O(N + M)
 * 主要用于存储输入的两个数字字符串。算法本身只使用了常数级别的额外空间。
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

/**
 * @brief 从一个数字字符串中提取指定数字构成的部分
 * @param numberStr 代表原始正整数的字符串
 * @param digit 要提取的个位数字 (0-9)
 * @return 由所有'digit'构成的'long long'类型的整数。如果不存在，则返回0。
 */
long long extractPart(const string& numberStr, int digit) {
    // 用于存储最终由指定数字构成的数值，例如 PA 或 PB
    long long partValue = 0;

    // 使用范围for循环遍历字符串中的每个字符，这是C++11以来的现代化写法
    for (char ch : numberStr) {
        // 将字符转换为对应的整数值，并与目标数字进行比较
        if (ch - '0' == digit) {
            // 如果找到匹配的数字，通过算术运算构建新的数值
            // 这比拼接字符串再转换要高效
            partValue = partValue * 10 + digit;
        }
    }

    return partValue;
}

int main() {
    // 使用 std::ios_base::sync_with_stdio(false) 和 cin.tie(NULL)
    // 是高性能计算中常用的技巧，可以显著加速C++的I/O操作
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 声明变量。使用string存储大数，int存储个位数字
    string a, b;
    int da, db;

    // 从标准输入读取数据
    cin >> a >> da >> b >> db;

    // 调用辅助函数，计算A的DA部分
    long long pa = extractPart(a, da);

    // 调用辅助函数，计算B的DB部分
    long long pb = extractPart(b, db);

    // ---- 测试用例演示 ----
    // 对于样例输入: 3862767 6 13530293 3
    // extractPart("3862767", 6) 会返回 66
    // extractPart("13530293", 3) 会返回 333
    // pa + pb = 66 + 333 = 399

    // 输出最终的和
    cout << pa + pb << endl;

    return 0;
}