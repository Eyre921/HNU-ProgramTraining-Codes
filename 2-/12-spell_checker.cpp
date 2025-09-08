/*
*12. 拼写检查
【问题描述】

作为一个新的拼写检查程序开发团队的成员，您将编写一个模块，用已知的所有形式正确的词典来检查给定单词的正确性。
如果字典中没有这个词，那么可以用下列操作中的一个来替换正确的单词（从字典中）：
1. 从单词中删除一个字母；
2. 用一个任意字母替换单词中的一个字母；
3. 在单词中插入一个任意字母。
你的任务是编写一个程序，为每个给定的单词找到字典中所有可能的替换。

【输入形式】

输入的第一部分包含所有字典中的词，每个单词占用一行，以一个单一字符“#”作为结束。所有单词都不相同，字典中至多1000个单词。

接下来的部分包含所有需要进行检查的单词，同样每个单词占用一行。这部分也以一个单一字符“#”作为结束。至多有50个单词需要检查。

在输入中所有的单词（字典中的和需要检查的）都仅由小写字母组成，每个最多包含15个字符。

【输出形式】

对于每个在输入中出现的单词，按照它们在输入的第二部分出现的顺序输出一行。如果该单词是正确的（也就是说它包含在字典中）则输出信息：“is correct”；如果该单词不正确，则首先输出该单词，然后输入符号':'（冒号），之后空一格，写出它所有可能的替代，以空格分隔。这些替代的单词按照它们在字典中（输入的第一部分）出现的顺序写出。如果没有可替代的单词，则在冒号后面直接输出换行。

【样例输入】

i
is
has
have
be
my
more
contest
me
too
if
award
#
me
aware
m
contest
hav
oo
or
i
fi
mre
#
【样例输出】

me is correct
aware: award
m: i my me
contest is correct
hav: has have
oo: too
or:
i is correct
fi: i
mre: more me
*/
/**
 * @file spell_checker.cpp
 * @brief 解决方案：实现一个简单的拼写检查器
 *
 * 核心思路:
 * 本质上是一个“编辑距离”为1的问题。对于每个待检查的单词，我们需要在字典中找到
 * 所有与之编辑距离为1的单词。
 *
 * 1.  数据结构选择:
 * -   使用 `std::vector<string> dictionary` 存储字典单词，以保证输出建议单词时
 * 能够遵循它们在字典中的原始顺序。
 * -   同时，使用 `std::unordered_set<string> dictSet` 存储一份完全相同的字典，
 * 用于快速（平均O(1)时间）判断一个单词是否存在于字典中。
 *
 * 2.  算法流程:
 * a.  读取并存储字典：循环读取单词，同时存入 `dictionary` 和 `dictSet`，
 * 直到遇到'#'。
 * b.  读取待检查单词：循环读取待检查的单词，存入一个 `vector<string>`，
 * 直到遇到'#'。
 * c.  逐个检查单词：遍历待检查的单词列表。
 * i.   首先，使用 `dictSet` 检查当前单词是否“正确”（即直接存在于字典中）。
 * 如果是，则按要求输出并处理下一个单词。
 * ii.  如果单词不正确，则遍历原始的 `dictionary` 向量。对于字典中的每一个词，
 * 调用一个辅助函数 `isEditDistanceOne()` 来判断它与当前待检查单词的
 * 编辑距离是否为1。
 * iii. `isEditDistanceOne()` 函数的逻辑是：比较两个字符串的长度，根据长度差
 * 分别处理“替换”、“插入”、“删除”这三种情况。
 * iv.  将所有满足条件的字典单词收集到一个建议列表中，然后按格式要求输出。
 *
 * 3.  关于样例 `aware: award` 的说明:
 * `aware` 和 `award` 之间的编辑距离是2（需要替换'r'为'd'，并替换'e'为'd'）。
 * 这与题目描述的单次操作不符。在其他所有样例都符合编辑距离为1的规则下，
 * 我们有理由相信该样例存在笔误。本代码将严格按照题目描述的“单次操作”规则实现。
 *
 * 时间复杂度: O(W * D * L)
 * W是待检查单词数，D是字典单词数，L是单词最大长度。对于每个待检查单词，
 * 在最坏情况下（单词不正确），都需要与字典中的所有单词进行一次比较，
 * 每次比较的时间复杂度与单词长度L成正比。
 *
 * 空间复杂度: O(D * L)
 * 主要空间开销用于存储字典的 `vector` 和 `unordered_set`。
 */

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <cmath> // For std::abs

// 使用标准命名空间，简化代码
using namespace std;

/**
 * @brief 判断两个单词的编辑距离是否为1。
 * @param queryWord 待检查的单词。
 * @param dictWord 字典中的单词。
 * @return 如果可以通过对 queryWord 进行单次增、删、改操作得到 dictWord，则返回true。
 */
bool isEditDistanceOne(const string& queryWord, const string& dictWord) {
    int lenQ = queryWord.length();
    int lenD = dictWord.length();

    // 长度差超过1，编辑距离必然大于1
    if (abs(lenQ - lenD) > 1) {
        return false;
    }

    // 情况一：替换 (长度相等)
    if (lenQ == lenD) {
        int diffCount = 0;
        for (int i = 0; i < lenQ; ++i) {
            if (queryWord[i] != dictWord[i]) {
                diffCount++;
            }
        }
        return diffCount == 1;
    }

    // 确保 shorterWord 总是较短的那个字符串
    const string& shorterWord = (lenQ < lenD) ? queryWord : dictWord;
    const string& longerWord = (lenQ < lenD) ? dictWord : queryWord;

    // 情况二 & 三：插入或删除 (长度差1)
    // 检查短的是否是长的的子序列，且只差一个字符
    size_t i = 0, j = 0;
    int diffCount = 0;
    while (i < shorterWord.length() && j < longerWord.length()) {
        if (shorterWord[i] == longerWord[j]) {
            i++;
            j++;
        } else {
            diffCount++;
            j++; // 跳过 longerWord 中的一个字符
        }
    }
    // 如果长的单词末尾还有剩余字符，也计入差异
    diffCount += (longerWord.length() - j);

    return diffCount == 1;
}

int main() {
    // 优化C++标准流的IO性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    vector<string> dictionary;
    unordered_set<string> dictSet;
    string word;

    // 1. 读取字典
    while (getline(cin, word) && word != "#") {
        dictionary.push_back(word);
        dictSet.insert(word);
    }

    // 2. 读取待检查的单词
    vector<string> wordsToCheck;
    while (getline(cin, word) && word != "#") {
        wordsToCheck.push_back(word);
    }

    // 3. 逐个处理待检查的单词
    for (const string& query : wordsToCheck) {
        // 检查是否为正确单词
        if (dictSet.count(query)) {
            cout << query << " is correct" << endl;
            continue;
        }

        // 如果不正确，则寻找建议
        cout << query << ":";
        vector<string> suggestions;
        for (const string& dictWord : dictionary) {
            if (isEditDistanceOne(query, dictWord)) {
                suggestions.push_back(dictWord);
            }
        }

        // 输出所有建议
        for (const string& suggestion : suggestions) {
            cout << " " << suggestion;
        }
        cout << endl;
    }

    return 0;
}