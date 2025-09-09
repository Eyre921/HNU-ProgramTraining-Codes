/*
宇宙回响——星际语法的验证
背景故事
在2077年，位于贵州的世界最大射电望远镜“天眼”（FAST）首次捕捉到了一段来自半人马座α星的、具有明显逻辑结构的重复信号。全球顶尖的密码学家和语言学家齐聚一堂，展开了对这段“宇宙回响”的破译工作。

经过数月的分析，团队有了一个惊人发现：这个外星文明的语言，其基础语法严格依赖于“语义闭包”结构。也就是说，每个表达核心概念的“起始词”都必须在后续文段中由一个特定的“终止词”来完美地闭合，类似于编程语言中的 begin...end 或 HTML 的 <tag>...</tag>。更复杂的是，这些语义闭包可以层层嵌套，形成复杂的逻辑结构。

如果一个句子片段的语义闭包结构不完整或嵌套错误，那么整个片段就会被认为是“已损坏的”或无意义的宇宙噪音。

现在，你作为项目组最年轻的天才程序员，任务是编写一个验证程序，根据已破译的“语义词典”，快速判断新接收到的信号片段在语法上是“有效的”还是“已损坏的”。

问题描述 (Problem Description)
给定一个“语义词典”，其中包含若干对“起始词”和“终止词”。然后给定一个由空格分隔的单词组成的信号片段。你需要判断这个信号片段是否遵循以下语法规则：

每一个起始词都必须有一个对应的、正确的终止词来闭合。

词语对的嵌套必须正确。例如 A B B A 是正确的，而 A B A B 是错误的。

信号片段中可能包含不属于词典的“内容词”，这些词不影响语法结构，应被忽略。

最终，一个有效的信号片段在分析完所有单词后，不应有任何未被闭合的起始词。

输入格式
输入包含多组测试用例。

第一行是一个整数 T (1
leT
le10)，代表测试用例的数量。

对于每个测试用例：
第一行是一个整数 M (1
leM
le100)，代表语义词典中词语对的数量。

接下来的 M 行，每行包含两个由单个空格隔开的单词 S_open 和 S_close，代表一对起始词和终止词。单词仅由小写字母和中划线 - 组成，长度不超过20。

最后一行是一个待验证的信号片段，由若干单词组成，单词间由单个空格隔开。信号片段总词数不超过1000。

输出格式 (Output Format)
对于每个测试用例，输出一行。

如果信号片段的语法结构有效，则输出 Valid。

如果语法结构无效（损坏），则输出 Corrupted。

样例输入 (Sample Input)
3
3
begin end
if endif
log-session logout
begin log-session user-alpha logged-in logout if true endif end
2
<data> <close>
<header> <end-header>
<data> <header> important-info <end-header> <close>
2
proc end-proc
call return
proc call a call b return return end-proc call return
样例输出 (Sample Output)
Valid
Valid
Corrupted
样例解释 (Sample Explanation)
样例 1: begin 与 end 匹配, log-session 与 logout 匹配, if 与 endif 匹配。嵌套关系为 begin(log-session() if())end，是完美的。所以输出 Valid。

样例 2: data 和 close 匹配，header 和 end-header 匹配。嵌套关系为 <data>(<header>()</close>)，也是完美的。important-info 是内容词，被忽略。所以输出 Valid。

样例 3: 最后一个 call return 在 proc...end-proc 结构之外，并且第一个 proc...end-proc 内部的 call...return 嵌套也是正确的。但是，在整个句子分析结束后，还有一个多余的 call 没有被 return 闭合（栈中还剩一个call）。因此整个句子是损坏的。所以输出 Corrupted。

提示 (Hint)
这个问题是“合法括号序列”问题的泛化版本。

使用栈（Stack）数据结构是解决此问题的经典方法。

使用映射（Map）来存储起始词和终止词的对应关系会非常高效。
*/


// Filename: advanced_valid_parentheses.cpp
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <sstream> // Required for stringstream

using namespace std;

// 函数用于检查一个由单词组成的字符串是否是合法的括号串
void solve()
{
    // --- 1. 读取括号对定义 ---
    int m; // 假设有 m 对括号关系
    cin >> m;

    map<string, string> open_to_close;
    map<string, string> close_to_open;

    // 读取 m 行，每行是一对开/闭括号单词
    string open_word, close_word;
    for (int i = 0; i < m; ++i)
    {
        cin >> open_word >> close_word;
        open_to_close[open_word] = close_word;
        close_to_open[close_word] = open_word;
    }

    // --- 2. 读取并处理待验证的句子 ---
    // cin 在读取了 m 之后，会留下一个换行符在缓冲区，需要忽略掉
    cin.ignore();

    string line;
    getline(cin, line); // 读取一整行句子

    // 使用 stringstream 将句子按空格分割成单词
    stringstream ss(line);
    string word;

    stack<string> s;
    bool is_valid = true;

    while (ss >> word)
    {
        // 依次读取每个单词

        // 判断单词是否为开括号
        if (open_to_close.count(word))
        {
            s.push(word);
        }
        // 判断单词是否为闭括号
        else if (close_to_open.count(word))
        {
            // 遇到闭括号，但栈是空的，说明没有匹配的开括号
            if (s.empty())
            {
                is_valid = false;
                break;
            }

            // 检查栈顶的开括号是否与当前闭括号匹配
            string expected_open_word = close_to_open[word];
            if (s.top() == expected_open_word)
            {
                s.pop(); // 匹配成功，出栈
            } else
            {
                // 不匹配
                is_valid = false;
                break;
            }
        }
        // 如果单词既不是开括号也不是闭括号，我们可以选择忽略它
        // else { /* do nothing for non-bracket words */ }
    }

    // --- 3. 最终判断 ---
    // 循环结束后，如果字符串合法，栈必须是空的
    if (!s.empty())
    {
        is_valid = false;
    }

    if (is_valid)
    {
        cout << "Yes" << endl;
    } else
    {
        cout << "No" << endl;
    }
}

int main()
{
    // 假设我们要处理 n 个测试用例
    int n;
    cin >> n;
    while (n--)
    {
        solve();
    }
    return 0;
}
