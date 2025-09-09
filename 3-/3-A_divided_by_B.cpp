 /*
*
* 3. A除以B
【问题描述】

本题要求计算A/B，其中A是不超过1000位的整数（A>=0），B是1位正整数。你需要输出商数Q和余数R，使得A = B * Q + R成立。

【输入形式】

输入在1行中依次给出A和B，中间以1空格分隔。

【输出形式】

在1行中依次输出Q和R，中间以1空格分隔。

【样例输入】

123456789050987654321 7
【样例输出】

17636684150141093474 3

*
 */
/**
 * @file A_divided_by_B.cpp
 * @brief 计算一个大整数A除以一个单位正整数B的商和余数。
 *
 * 核心思路:
 * 模拟手动长除法的过程。由于被除数A的位数可能高达1000位，我们必须使用字符串来存储它。
 * 除数B是一个单位数，这大大简化了计算。
 *
 * 算法步骤如下：
 * 1. 从A的最高位开始，逐位处理。
 * 2. 维护一个当前的余数`currentRemainder`，初始为0。
 * 3. 在每一步中，将`currentRemainder`乘以10，再加上A的当前位上的数字，形成一个新的被除数部分。
 * 4. 用这个新的被除数部分去除以B，得到的商就是结果商Q的当前位，得到的余数则更新为`currentRemainder`，用于下一步计算。
 * 5. 将计算出的商的每一位数字追加到结果字符串中。
 *
 * 特殊处理：
 * - 商的最高位不能是0（除非整个商就是0）。我们在构建商字符串时，只有在字符串非空或当前商位不为0时才追加，以此来避免前导零。
 * - 如果A本身就小于B，商为0。我们的算法在循环结束后，如果商字符串仍为空，则将其置为"0"。
 *
 * 时间复杂度: O(L)
 * 其中L是A的位数。我们只需要遍历一次A的数字字符串。
 *
 * 空间复杂度: O(L)
 * 主要用于存储输入的字符串A和输出的商字符串Q。
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// 使用标准命名空间，简化代码
using namespace std;

int main() {
 // 加速C++ I/O操作
 ios_base::sync_with_stdio(false);
 cin.tie(NULL);

 // dividendStr: 被除数A (大整数)
 // divisor: 除数B (单位正整数)
 string dividendStr;
 int divisor;
 cin >> dividendStr >> divisor;

 // quotientStr: 用于构建最终的商Q
 string quotientStr;
 // currentRemainder: 在长除法过程中，每一步计算的余数
 long long currentRemainder = 0; // 使用long long确保安全，尽管int在此题也足够

 // 遍历被除数A的每一位
 for (char digitChar : dividendStr) {
  // 核心步骤1: "带上"前一步的余数，并"取下"A的一位新数字，形成当前的计算单元
  currentRemainder = currentRemainder * 10 + (digitChar - '0');

  // 核心步骤2: 计算当前位的商
  int currentQuotientDigit = currentRemainder / divisor;

  // 核心步骤3: 将计算出的商位追加到结果中，同时处理前导零
  // 只有在商字符串已经有内容，或者当前计算出的商位不为0时，才进行追加
  if (!quotientStr.empty() || currentQuotientDigit != 0) {
   // 使用 push_back 和字符转换比 to_string 更高效
   quotientStr.push_back(currentQuotientDigit + '0');
  }

  // 核心步骤4: 更新余数，用于下一轮计算
  currentRemainder = currentRemainder % divisor;
 }

 // 边界情况处理: 如果循环结束后商字符串仍为空，说明结果是0 (例如 3 / 7)
 if (quotientStr.empty()) {
  quotientStr = "0";
 }

 // 输出最终的商和余数
 cout << quotientStr << " " << currentRemainder << endl;

 return 0;
}