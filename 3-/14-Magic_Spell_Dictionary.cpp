/*
14. 魔咒词典
【问题描述】

哈利波特在魔法学校的必修课之一就是学习魔咒。据说魔法世界有100000种不同的魔咒，哈利很难全部记住，但是为了对抗强敌，他必须在危急时刻能够调用任何一个需要的魔咒，所以他需要你的帮助。

给你一部魔咒词典。当哈利听到一个魔咒时，你的程序必须告诉他那个魔咒的功能；当哈利需要某个功能但不知道该用什么魔咒时，你的程序要替他找到相应的魔咒。如果他要的魔咒不在词典中，就输出“what?”

【输入形式】

首先列出词典中不超过100000条不同的魔咒词条，每条格式为：

[魔咒] 对应功能

其中“魔咒”和“对应功能”分别为长度不超过20和80的字符串，字符串中保证不包含字符“[”和“]”，且“]”和后面的字符串之间有且仅有一个空格。词典最后一行以“@END@”结束，这一行不属于词典中的词条。
词典之后的一行包含非负整数N（0=<N<=1000），随后是N个测试用例。每个测试用例占一行，或者给出“[魔咒]”，或者给出“对应功能”。

【输出形式】

每个测试用例的输出占一行，输出魔咒对应的功能，或者功能对应的魔咒。如果魔咒不在词典中，就输出“what?”

【样例输入】

[expelliarmus] the disarming charm
[rictusempra] send a jet of silver light to hit the enemy
[tarantallegra] control the movement of one's legs
[serpensortia] shoot a snake out of the end of one's wand
[lumos] light the wand
[obliviate] the memory charm
[expecto patronum] send a Patronus to the dementors
[accio] the summoning charm
@END@
4
[lumos]
the summoning charm
[arha]
take me to the sky
【样例输出】

light the wand
accio
what?
what?

 */
/*
 * @file: Spell_Dictionary.cpp
 *
 * @brief:
 * 本文件实现了"魔咒词典"问题的解决方案。程序首先读取一个魔咒及其功能的词典，
 * 然后能够根据用户提供的魔咒查询其功能，或根据功能查询其对应的魔咒，
 * 实现了一个高效的双向查询系统。
 *
 * @core_idea:
 * 核心思路是构建一个双向索引。由于我们需要双向查询（魔咒->功能 和 功能->魔咒），
 * 使用两个哈希表（`std::unordered_map`）是最高效的数据结构。
 * 1.  **spellToFunction**: 一个哈希表，键(key)是魔咒字符串，值(value)是其对应的功能字符串。
 * 它用于处理 "根据魔咒查功能" 的请求，提供了平均O(1)的查找速度。
 * 2.  **functionToSpell**: 另一个哈希表，键是功能字符串，值是其对应的魔咒字符串。
 * 它用于处理 "根据功能查魔咒" 的请求，同样具有平均O(1)的查找效率。
 * 3.  **词典解析**: 逐行读取输入，直到遇到终止符"@END@"。对于每一行，通过查找']'字符
 * 的位置，精确地解析出方括号内的魔咒和后面的功能描述。随后，将这对信息同时
 * 存入上述两个哈希表中，完成双向索引的构建。
 * 4.  **查询处理**: 读取查询数量N，然后循环N次。对于每次查询：
 * a. 判断查询类型：检查输入行的第一个字符是否为'['。
 * b. 如果是，则为魔咒查询。从字符串中提取出魔咒，然后在 `spellToFunction` 中查找。
 * c. 如果不是，则为功能查询。直接使用整行字符串作为键，在 `functionToSpell` 中查找。
 * d. 如果在对应的哈希表中找到条目，则输出结果；否则，按要求输出 "what?"。
 *
 * @time_complexity: O(D*L + N*L)
 * D是词典条目数，N是查询次数，L是字符串的平均长度。构建词典和处理查询的时间
 * 都与条目总数和字符串长度成线性关系。
 *
 * @space_complexity: O(D*L)
 * 需要存储整个词典两次（一次正向，一次反向），空间复杂度与词典的总大小成正比。
 */

#include <iostream>
#include <string>
#include <unordered_map>
#include <limits> // 用于 std::numeric_limits

// 为了代码简洁，直接使用 std 命名空间
using namespace std;

int main() {
    // 提高C++标准输入输出流的效率
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 定义两个哈希表以实现高效的双向查找
    unordered_map<string, string> spellToFunction;
    unordered_map<string, string> functionToSpell;

    string line;
    // 步骤1: 循环读取并构建词典
    while (getline(cin, line) && line != "@END@") {
        // 查找']'的位置，这是分割魔咒和功能的关键分隔符
        size_t closingBracketPos = line.find(']');

        // 提取魔咒：从第一个字符'['之后（索引1）开始，长度为 closingBracketPos - 1
        string spell = line.substr(1, closingBracketPos - 1);

        // 提取功能：从']'和一个空格之后（索引 closingBracketPos + 2）开始，直到行尾
        string function = line.substr(closingBracketPos + 2);

        // 将魔咒和功能的关系存入两个哈希表，建立双向索引
        spellToFunction[spell] = function;
        functionToSpell[function] = spell;
    }

    int n;
    // 步骤2: 读取查询的数量
    cin >> n;

    // 关键步骤：当使用 `cin >> n` 后，换行符会留在输入缓冲区。
    // 必须使用 `cin.ignore()` 清除这个换行符，否则后续的第一个 `getline` 会错误地读取一个空行。
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // 步骤3: 循环处理N个查询
    for (int i = 0; i < n; ++i) {
        getline(cin, line);

        // 判断查询类型：是查询魔咒还是查询功能
        if (!line.empty() && line[0] == '[') {
            // 类型一：输入的是魔咒 "[spell]"
            // 提取魔咒，去掉前后的'['和']'
            string spellQuery = line.substr(1, line.length() - 2);

            // 在 spellToFunction 哈希表中查找。使用 .count() 判断是否存在以避免创建新条目。
            if (spellToFunction.count(spellQuery)) {
                cout << spellToFunction[spellQuery] << endl;
            } else {
                cout << "what?" << endl;
            }
        } else {
            // 类型二：输入的是功能
            string functionQuery = line;

            // 在 functionToSpell 哈希表中查找
            if (functionToSpell.count(functionQuery)) {
                cout << functionToSpell[functionQuery] << endl;
            } else {
                cout << "what?" << endl;
            }
        }
    }

    return 0;
}

/*
 * ========================================
 * 测试用例与执行分析
 * ========================================
 *
 * 【样例输入】
 * [expelliarmus] the disarming charm
 * ... (其他词典条目) ...
 * [accio] the summoning charm
 * @END@
 * 4
 * [lumos]
 * the summoning charm
 * [arha]
 * take me to the sky
 *
 * 【代码执行流程】
 * 1. **词典构建阶段**:
 * - 循环调用 `getline` 读取每一行，例如 "[lumos] light the wand"。
 * - 解析出 spell="lumos", function="light the wand"。
 * - 执行 `spellToFunction["lumos"] = "light the wand";`
 * - 执行 `functionToSpell["light the wand"] = "lumos";`
 * - 重复此过程，直到读到 "@END@" 停止。
 *
 * 2. **查询处理阶段**:
 * - 读取 N = 4。`cin.ignore()` 清除 N 后面的换行符。
 * - **查询1**: `getline` 读取 "[lumos]"。
 * - `line[0]` 是 '['，判定为魔咒查询。
 * - 提取出 `spellQuery` = "lumos"。
 * - `spellToFunction.count("lumos")` 返回 true (假设lumos在词典里)。
 * - 输出 `spellToFunction["lumos"]` 的值："light the wand"。
 *
 * - **查询2**: `getline` 读取 "the summoning charm"。
 * - `line[0]` 不是 '['，判定为功能查询。
 * - `functionQuery` = "the summoning charm"。
 * - `functionToSpell.count("the summoning charm")` 返回 true (因为词典中有[accio]条目)。
 * - 输出 `functionToSpell["the summoning charm"]` 的值："accio"。
 *
 * - **查询3**: `getline` 读取 "[arha]"。
 * - 判定为魔咒查询，提取出 `spellQuery` = "arha"。
 * - `spellToFunction.count("arha")` 在哈希表中未找到，返回 false。
 * - 输出 "what?"。
 *
 * - **查询4**: `getline` 读取 "take me to the sky"。
 * - 判定为功能查询，`functionQuery` = "take me to the sky"。
 * - `functionToSpell.count("take me to the sky")` 在哈希表中未找到，返回 false。
 * - 输出 "what?"。
 *
 * 【样例输出】
 * light the wand
 * accio
 * what?
 * what?
 */