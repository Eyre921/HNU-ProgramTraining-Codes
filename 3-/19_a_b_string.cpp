 
/*
19. ab串
【问题描述】

       给定一个由字符'a'和字符'b'组成的字符串，可以删除若干字符，使得剩下来的字符串满足前后段为a，中间段为b（aaa....aaabbbb.....bbbbaaa.....aaa）,区段可以没有字符（ba,ab,b,aa都是合法的），求最长剩下字符串的长度。

【输入形式】

      输入为一行一个长度不超过5000的非空字符串，字符串仅由字符'a'和字符'b'组成。
【输出形式】

      输出为一个整数，表示符合要求的最长剩下字符串长度
【样例输入1】

abba
【样例输出1】

4
【样例输入2】

bab
【样例输出2】

2
 */
/**
 * @file 19.ab_string.cpp
 * @brief "ab串"问题的动态规划解法
 *
 * 核心思路:
 * 这是一个典型的动态规划问题，目标是寻找满足特定模式 "a...ab...ba...a" 的最长子序列。
 * 我们可以通过一次遍历字符串来解决此问题。在遍历过程中，我们维护三个状态的子序列的最长长度：
 * 1. 状态一 (纯'a'前缀): 只由 'a' 构成的子序列。我们用 `maxLenA` 记录其最长长度。
 * 2. 状态二 ('ab'结构): 形如 "a...ab...b" 的子序列。我们用 `maxLenAB` 记录其最长长度。
 * 3. 状态三 (完整'aba'结构): 形如 "a...ab...ba...a" 的子序列。我们用 `maxLenABA` 记录其最长长度。
 *
 * 状态转移逻辑如下：
 * - 当遇到字符 'a' 时：
 * - 状态一：可以在之前的纯'a'子序列上加一，即 `maxLenA++`。
 * - 状态三：可以在一个 "a...ab...b" 子序列后开启第三段 'a'，或者在已有的 "a...ab...ba...a" 子序列上继续添加 'a'。
 * 因此，状态三的新长度是 `max(maxLenAB, maxLenABA) + 1`。
 *
 * - 当遇到字符 'b' 时：
 * - 状态二：可以在一个纯'a'子序列后开启第二段 'b'，或者在已有的 "a...ab...b" 子序列上继续添加 'b'。
 * 因此，状态二的新长度是 `max(maxLenA, maxLenAB) + 1`。
 *
 * 最终结果：
 * 题目允许每个段为空，所以 "aaa", "bbb", "aaabbb" 等都是合法的。我们的三个状态变量正好覆盖了
 * 所有这些可能的情况。遍历结束后，`maxLenA`, `maxLenAB`, `maxLenABA` 分别代表以纯'a'、'ab'结构、
 * 'aba'结构结尾的最长子序列的长度。这三者中的最大值即为最终答案。
 *
 * 时间复杂度: O(N)，其中 N 是输入字符串的长度。我们只需要对字符串进行一次线性扫描。
 * 空间复杂度: O(1)，我们只使用了固定数量的变量来存储状态，与输入规模无关。
 */

#include <iostream>
#include <string>
#include <algorithm>

// 按照规范，直接使用 std 命名空间
using namespace std;

int main() {
    // 优化C++标准流的输入输出性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string s;
    cin >> s;

    // DP状态变量初始化
    // maxLenA: 状态一 (a...a) 的最长子序列长度
    int maxLenA = 0;
    // maxLenAB: 状态二 (a...ab...b) 的最长子序列长度
    int maxLenAB = 0;
    // maxLenABA: 状态三 (a...ab...ba...a) 的最长子序列长度
    int maxLenABA = 0;

    // 使用范围for循环遍历输入字符串，根据字符更新DP状态
    for (char ch : s) {
        if (ch == 'a') {
            // 当前字符是 'a'
            // 1. 扩展状态一 (a...a): 直接在之前的基础上加1
            maxLenA++;

            // 2. 扩展状态三 (a...ab...ba...a):
            // 它可以由一个状态二的子序列加上当前'a'形成，或者由一个已有的状态三子序列加上当前'a'形成。
            // 我们取这两种可能中的较长者。
            maxLenABA = max(maxLenAB, maxLenABA) + 1;

        } else { // ch == 'b'
            // 当前字符是 'b'
            // 1. 扩展状态二 (a...ab...b):
            // 它可以由一个状态一的子序列加上当前'b'形成，或者由一个已有的状态二子序列加上当前'b'形成。
            // 同样，取两者中的较长者。
            maxLenAB = max(maxLenA, maxLenAB) + 1;
        }
    }

    // 最终答案是所有可能合法形式的最长长度。
    // maxLenA 覆盖了 "aaa" 形式, maxLenAB 覆盖了 "bbb" 和 "aaabbb" 形式,
    // maxLenABA 覆盖了 "aaabbbaaa" 形式。三者取最大即可。
    // 使用初始化列表的 max 函数是 C++11 及以上版本提供的简洁写法。
    int result = max({maxLenA, maxLenAB, maxLenABA});

    cout << result << endl;



    return 0;
}