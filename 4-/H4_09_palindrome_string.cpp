/*
9. 回文串
问题描述

“回文串”是一个正读和反读都一样的字符串，比如“level”或者“noon”等等就是回文串。给你一个字符串，问最少在字符串尾添加多少字符，可以使得字符串变为回文串。

输入格式

有多组测试数据。

每组测试数据第一行是一个正整数N，表示字符串长度，接下来一行是长度为N的字符串，字符串中只有小写字母。

N=0表示输入结束，并且不需要处理。

40%的数列元素个数N 1 ≤ N≤ 100；

30%的数列元素个数N 1 ≤ N≤ 1000；

20%的数列元素个数N 1 ≤ N≤ 10000；

10%的数列元素个数N 1 ≤ N≤ 100000；

输出格式

  　　对于每组测试数据，输出一个非负整数：添加最少的字符数，可以使得字符串变为回文串。

样例输入


3
aba
4
aaac
0
样例输出


0
3
*/
/**
 * @brief 在字符串尾部添加最少字符构造回文串问题的最优解
 *
 * 核心思路:
 * 问题的目标是在字符串s末尾添加最少字符，使其变为回文串。这等价于找到s的最长回文后缀，
 * 然后将该回文后缀之前的部分反转，再拼接到s的末尾。添加的字符数即为 s.length() - length(最长回文后缀)。
 *
 * 关键在于如何高效地找到最长回文后缀的长度L。
 * 一个后缀是回文的，意味着它等于其自身的反转。而一个后缀的反转，恰好是整个字符串反转后的一个前缀。
 * 因此，问题转化为：寻找s的最长后缀，该后缀同时也是reverse(s)的前缀。
 *
 * 这是一个经典的字符串匹配问题，可以通过KMP算法的预处理步骤在O(N)时间内解决。
 * 1. 构造一个特殊的字符串 T = reverse(s) + '#' + s ('#'为特殊分隔符)。
 * 2. 计算T的KMP `next` (或称 lps/pi) 数组。
 * 3. `next`数组的最后一个值 `next.back()` 就代表了T的最长公共前后缀的长度，
 * 这个长度也就是reverse(s)的前缀和s的后缀的最大匹配长度，即L。
 * 4. 最终结果为 s.length() - L。
 *
 * 时间复杂度: O(N)
 * 字符串反转、拼接以及KMP next数组的计算都是线性时间复杂度。
 *
 * 空间复杂度: O(N)
 * 需要额外空间存储反转后的字符串、拼接后的T字符串以及next数组。
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // for std::reverse

// 启用C++标准库，无需显式使用 std:: 前缀
using namespace std;

/**
 * @brief 计算使字符串s成为回文串所需在末尾添加的最少字符数
 * @param s 原始输入字符串
 * @return 需要添加的最少字符数
 */
int solvePalindromeAppend(const string& s) {
    int n = s.length();
    // 边界情况：空字符串或单字符已是回文
    if (n <= 1) {
        return 0;
    }

    // 步骤1: 构造辅助字符串 T = reverse(s) + '#' + s
    string sReversed = s;
    reverse(sReversed.begin(), sReversed.end());
    string kmpPattern = sReversed + '#' + s;

    // 步骤2: 计算kmpPattern的next数组
    int patternLength = kmpPattern.length();
    vector<int> next(patternLength, 0);

    // KMP next数组的经典实现
    // len: 当前已匹配的最长公共前后缀的长度
    // i:   主迭代指针
    for (int i = 1, len = 0; i < patternLength; ) {
        if (kmpPattern[i] == kmpPattern[len]) {
            // 字符匹配成功，长度加一
            len++;
            next[i] = len;
            i++;
        } else {
            if (len != 0) {
                // 发生失配，利用已计算的next值进行回溯，缩小len
                len = next[len - 1];
            } else {
                // len已为0，无法再回溯，此位置的匹配长度为0
                next[i] = 0;
                i++;
            }
        }
    }

    // 步骤3: 获取最长回文后缀的长度
    int longestPalindromicSuffixLength = next.empty() ? 0 : next.back();

    // 步骤4: 计算需要添加的字符数
    return n - longestPalindromicSuffixLength;
}

int main() {
    // 优化I/O性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    /*
     * --- 测试用例说明 ---
     * 【样例输入】
     * 3
     * aba
     * 4
     * aaac
     * 0
     *
     * 【样例输出】
     * 0
     * 3
     *
     * --- 其他关键测试用例 ---
     * 输入 N=6, s="google"
     * s_rev="elgoog", T="elgoog#google"
     * 最长回文后缀为 "e" (长度1), 结果为 6 - 1 = 5.
     *
     * 输入 N=4, s="race"
     * s_rev="ecar", T="ecar#race"
     * 最长回文后缀为 "e" (长度1), 结果为 4 - 1 = 3.
     */

    int n;
    // 循环读取输入，直到N为0，这是题目要求的输入结束标志
    while (cin >> n && n != 0) {
        string s;
        cin >> s;
        cout << solvePalindromeAppend(s) << endl;
    }

    return 0;
}