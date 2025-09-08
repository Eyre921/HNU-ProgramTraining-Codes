//1. 字符串反转2
//【问题描述】
//
//         给定一个句子（只包含字母和空格）， 将句子中的单词位置反转，单词用空格分割, 单词之间只有一个空格，前后没有空格。 比如： “hello xiao mi”-> “mi xiao hello”
//
//【输入形式】
//
//     输入数据有多组，每组占一行，包含一个句子(句子长度小于1000个字符)
//【输出形式】
//
//       对于每个测试示例，要求输出句子中单词反转后形成的句子
//【样例输入】
//
//hello xiao mi
//I am a student
//【样例输出】
//
//mi xiao hello
//student a am I

/**
 * @file reverse_words_in_sentence.cpp
 * @brief 解决方案：反转句子中的单词顺序
 *
 * 核心思路:
 * 利用C++标准库中的字符串流（istringstream）来高效地分割句子。
 * 1. 读取整行输入句子。
 * 2. 将该行字符串送入一个istringstream对象，istringstream可以像处理cin一样，
 * 自动以空格为分隔符提取单词。
 * 3. 将提取出的单词依次存入一个vector<string>容器中。
 * 4. 使用STL中的std::reverse算法，将vector中存储的单词顺序进行原地反转。
 * 5. 遍历反转后的vector，将单词拼接输出，并在单词间添加空格。
 *
 * 时间复杂度: O(N)
 * 其中N是输入句子的总字符数。
 * - 读取和分割字符串为单词，需要遍历一次所有字符，复杂度为O(N)。
 * - 反转单词向量，若有K个单词，复杂度为O(K)。
 * - 输出拼接字符串，需要遍历一次所有单词及其字符，复杂度为O(N)。
 * 因此，总体时间复杂度为O(N)。
 *
 * 空间复杂度: O(N)
 * 需要一个vector<string>来存储句子中的所有单词，其占用的空间与输入句子的总字符数N成正比。
 */

#include <iostream> // 用于标准输入输出 (cin, cout)
#include <string>   // 用于使用string类
#include <vector>   // 用于使用vector容器
#include <sstream>  // 用于使用istringstream进行字符串流处理
#include <algorithm>// 用于使用std::reverse算法
#include <iterator> // 虽然不是必须，但了解流迭代器也是一种思路

// 使用标准命名空间，简化代码
using namespace std;

/**
 * @brief 反转给定句子中的单词顺序
 * @param sentence 原始句子字符串
 * @return 单词顺序反转后的新句子字符串
 */
string reverseWords(const string& sentence) {
    // 使用istringstream可以非常方便地按空格分割单词
    istringstream wordStream(sentence);
    
    // 使用vector存储从流中读取的所有单词
    vector<string> words;
    string currentWord;

    // 循环从字符串流中提取单词，直到流结束
    // '>>' 操作符默认会以空白字符（空格、换行等）为分隔符
    while (wordStream >> currentWord) {
        words.push_back(currentWord);
    }
    
    // 如果没有单词（例如，输入是空字符串），直接返回空字符串
    if (words.empty()) {
        return "";
    }

    // 使用STL的reverse函数原地反转vector中元素的顺序，高效且简洁
    reverse(words.begin(), words.end());

    // 使用ostringstream高效地构建输出字符串，避免多次字符串拼接的性能开销
    ostringstream resultStream;
    
    // 遍历反转后的单词向量，构建结果
    for (size_t i = 0; i < words.size(); ++i) {
        resultStream << words[i];
        // 只在单词之间添加空格，不在最后一个单词后添加
        if (i < words.size() - 1) {
            resultStream << " ";
        }
    }

    return resultStream.str();
}

int main() {
    // 优化C++标准流的IO性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string inputLine;

    // 使用getline循环读取多组测试数据，直到输入结束
    // getline可以读取包含空格的一整行
    while (getline(cin, inputLine)) {
        // 调用核心函数处理当前行
        string reversedSentence = reverseWords(inputLine);
        
        // 输出结果，并换行
        cout << reversedSentence << endl;
    }

    return 0;
}
