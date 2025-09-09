 /*7. 写出来吧
【问题描述】

读入一个自然数n，计算其各位数字之和，用汉语拼音写出和的每一位数字。

【输入形式】

每个测试输入包含1个测试用例，即给出自然数n的值。这里保证n小于10的100次方。

【输出形式】

在一行内输出n的各位数字之和的每一位，拼音数字间有1 空格，但一行中最后一个拼音数字后没有空格。

【样例输入】

1234567890987654321123456789
【样例输出】

yi san wu
【样例说明】
友情提示汉语拼音

0~9：ling yi er san si wu liu qi ba jiu shi


*/
/**
 * @file Write_It_Out.cpp
 * @brief 读入一个大数，计算其各位数字之和，并用汉语拼音输出和的每一位。
 *
 * 核心思路:
 * 1.  由于输入的自然数n可能长达100位，超过了所有基本整型（包括`long long`）的存储范围，
 * 因此必须使用`std::string`来读取和存储这个大数。
 * 2.  计算各位数字之和。我们遍历输入字符串的每一个字符，将其从字符形式（如'9'）
 * 转换成对应的整数值（如9），然后累加到一个整型变量中。
 * 由于n最多100位，每位最大是9，所以其各位数字之和最大为 100 * 9 = 900。
 * 一个标准的`int`类型足以安全地存储这个和。
 * 3.  输出和的拼音。为了方便地访问和的每一位数字，我们将计算出的整数和转换回`std::string`类型。
 * 例如，整数`135`变为字符串`"135"`。
 * 4.  创建一个从数字到拼音的映射（可以使用`std::vector<std::string>`）。
 * 5.  遍历代表和的字符串，对每一位数字字符，查找其对应的拼音并输出。为了满足格式要求
 * （拼音间有空格，行末无空格），我们在输出除第一个拼音外的每个拼音前，先输出一个空格。
 *
 * 时间复杂度: O(L)
 * L是输入大数n的位数。主要耗时在于遍历这个字符串以计算各位数字之和。
 * 后续将和转换为字符串及查表输出的步骤，由于和最大仅为900（常数位数），其时间可视为O(1)。
 *
 * 空间复杂度: O(L)
 * 主要用于存储输入的大数字符串。
 */

#include <iostream>
#include <string>
#include <vector>
#include <numeric>

// 使用标准命名空间，简化代码
using namespace std;

int main() {
 // 加速C++ I/O操作
 ios_base::sync_with_stdio(false);
 cin.tie(NULL);

 // 使用一个常量向量作为数字0-9到汉语拼音的映射表，清晰且高效
 const vector<string> pinyinMap = {
  "ling", "yi", "er", "san", "si", "wu", "liu", "qi", "ba", "jiu"
};

 // 步骤1: 使用字符串读取大数n
 string largeNumberStr;
 cin >> largeNumberStr;

 // 步骤2: 计算各位数字之和
 int sumOfDigits = 0;
 // 使用范围for循环遍历字符串中的每个字符，代码更简洁现代
 for (char digitChar : largeNumberStr) {
  // 将字符转换为整数并累加
  sumOfDigits += (digitChar - '0');
 }

 // 步骤3: 将整数和转换为字符串，方便按位处理
 string sumStr = to_string(sumOfDigits);

 // 步骤4 & 5: 遍历和的每一位，查表输出拼音，并处理空格
 for (size_t i = 0; i < sumStr.length(); ++i) {
  // 在输出非首个拼音单词前，先输出一个空格作为分隔符
  if (i > 0) {
   cout << " ";
  }

  // 获取当前位的数字值
  int digit = sumStr[i] - '0';

  // 从映射表中查找并输出对应的拼音
  cout << pinyinMap[digit];
 }
 // 输出换行符，结束本次输出
 cout << endl;

 return 0;
}