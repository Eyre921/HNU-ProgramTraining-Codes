/*6. 小A的计算器
【问题描述】

        以往的操作系统内部的数据表示都是二进制方式，小A新写了一个操作系统，系统内部的数据表示为26进制，其中0-25分别由a-z表示。
        现在小A要在这个操作系统上实现一个计算器，这个计算器要能实现26进制数的加法运算。你能帮小A实现这个计算器吗？

【输入形式】

       输入的第一行包括一个整数N(1<=N<=100)。
       接下来的N行每行包括两个26进制数x和y，它们之间用空格隔开，每个数的位数最多为10位,我们可以保证相加的结果的位数最多也是10位。每个数会用小A所设计的操作系统中的表示方法来表示，如：bsadfasdf。即每个数的各个位均由26个小写字母a-z中的一个来表示。

【输出形式】

        输出x和y相加后的结果，结果也要用题目中描述的26进制数来表示。

【样例输入】

4
ba cd
c b
b c
ba c
【样例输出】

dd
d
d
bc
*/

/**
 * @file base26_calculator.cpp
 * @brief 解决方案：实现一个26进制（a-z）的加法计算器
 *
 * 核心思路:
 * 这个问题的本质是实现一个自定义进制的“大数加法”。我们可以完全模拟手动进行
 * 十进制加法的“列式竖式”计算过程，但将基数从10换为26。
 *
 * 1.  数字表示：字符'a'到'z'分别代表数值0到25。
 * -   字符转数值：`value = character - 'a'`
 * -   数值转字符：`character = value + 'a'`
 *
 * 2.  加法算法：
 * a. 从两个数的最末位（最右边，即个位）开始，逐位相加。
 * b. 维护一个进位变量 `carry`，初始为0。
 * c. 在每个位置上，将两个数对应位的数值以及来自低位的 `carry` 相加，得到 `currentSum`。
 * d. 当前位置的结果数值为 `currentSum % 26`，然后将其转换为对应的字符。
 * e. 新的进位 `carry` 为 `currentSum / 26`，用于更高位的计算。
 * f. 使用指针或索引从右向左移动，重复此过程，直到两个数的所有位都被处理完毕且`carry`为0。
 * g. 由于我们是从低位向高位计算，得到的结果字符需要逆序排列才是最终答案。一个高效的方法是
 * 先将结果顺序存入字符串，最后对整个字符串进行一次反转。
 *
 * 时间复杂度: O(L)
 * 其中 L 是两个输入字符串中较长者的长度。我们需要遍历每个数位一次。
 *
 * 空间复杂度: O(L)
 * 需要一个字符串来存储结果，其长度与输入字符串的长度相当。
 */

#include <iostream>   // 用于标准输入输出 (cin, cout)
#include <string>     // 用于使用string类
#include <algorithm>  // 用于使用 std::reverse 函数
#include <vector>     // 虽然不是必须，但也是一种选择

// 使用标准命名空间，简化代码
using namespace std;

/**
 * @brief 对两个26进制表示的字符串进行相加。
 * @param numA 第一个26进制数。
 * @param numB 第二个26进制数。
 * @return 两个数相加后的结果，仍为26进制字符串。
 */
string addBase26(const string& numA, const string& numB) {
    string result = ""; // 用于存储结果，先顺序存储，最后反转
    int carry = 0;      // 进位，初始为0

    // 使用两个指针分别指向两个字符串的末尾
    int ptrA = numA.length() - 1;
    int ptrB = numB.length() - 1;

    // 循环条件：只要还有数字位或进位存在，就继续计算
    while (ptrA >= 0 || ptrB >= 0 || carry > 0) {
        // 获取当前位的数值。如果某个指针已越界，则该位数值视为0
        int valA = (ptrA >= 0) ? (numA[ptrA] - 'a') : 0;
        int valB = (ptrB >= 0) ? (numB[ptrB] - 'a') : 0;

        // 当前位的总和 = A的位值 + B的位值 + 来自低位的进位
        int currentSum = valA + valB + carry;

        // 计算当前位的结果字符并追加到结果字符串
        // 当前位的数值为总和对26取模
        result += (char)((currentSum % 26) + 'a');

        // 计算向高位的新进位
        // 进位为总和整除26
        carry = currentSum / 26;

        // 移动指针到更高位（即字符串的前一个字符）
        ptrA--;
        ptrB--;
    }

    // 因为我们是从低位到高位（右到左）计算的，结果是反的，需要反转
    reverse(result.begin(), result.end());

    return result;
}

int main() {
    // 优化C++标准流的IO性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int testCaseCount;
    cin >> testCaseCount;

    // 循环处理每个测试用例
    while (testCaseCount--) {
        string numX, numY;
        cin >> numX >> numY;

        // 调用核心函数计算结果并输出
        cout << addBase26(numX, numY) << endl;
    }

    // 样例测试
    // 【样例输入】
    // 4
    // ba cd
    // c b
    // b c
    // ba c
    // 【预期输出】
    // dd
    // d
    // d
    // bc

    return 0;
}