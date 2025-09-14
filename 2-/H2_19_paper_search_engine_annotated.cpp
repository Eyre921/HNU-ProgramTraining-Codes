/*
 *
19. Engine-字符串
【问题描述】

       谷歌、百度等搜索引擎已经成为了互连网中不可或缺的一部分。在本题中，你的任务也是设计一个搜索论文的搜索引擎，当然，本题的要求比起实际的需求要少了许多。
       本题的输入将首先给出一系列的论文，对于每篇论文首先给出标题，然后给出它被引用的次数。然后会有一系列的搜索询问，询问标题中包含特定关键词的论文有哪些。
       每一个询问可能包含多个关键词，你需要找出标题包含所有关键词的论文。
    “包含”必须是标题中有一个词正好是给定的关键词，不区分大小写。
      对每个询问，都按被引用的次数从多到少输出满足条件的论文的标题。如果有被引用的次数相同的论文，则按照论文在输入中的顺序排列，先给出的论文排在前面。

【输入形式】输入包含多组数据。
     每组数据首先有一行包含一个整数N(1<=N<=1000)，表示论文的数目，N=0表示输入结束。每组论文的信息第一行是论文的标题，由字母（大小写均可）和空格组成，不超过10个词，每个词不超过20个字符，标题总共不超过250个字符。第二行是一个整数K(0<=K&lt;=108)，表示它被引用的次数。在论文信息结束以后，有一行包含一个整数M(1<=M<=100)，表示询问的数目。接下来有M行，每行是一个询问，由L(1<=L<=10)个空格分开的词构成，每个词不超过20个字符。

【输出形式】

      对每个询问，按照题目给定的顺序输出满足条件的论文的标题；如果没有满足条件的论文，就不输出。在每组询问的输出之后输出一行“***”，在每组数据的输出之后输出一行“---”。

【样例输入1】

6
Finding the Shortest Path
120
Finding the k Shortest Path
80
Find Augmenting Path in General Graph
80
Matching in Bipartite Graph
200
Finding kth Shortest Path
50
Graph Theory and its Applications
40
6
shortest path
k shortest path
graph
path
find
application
0

【样例输出1】

Finding the Shortest Path
Finding the k Shortest Path
Finding kth Shortest Path
***
Finding the k Shortest Path
***
Matching in Bipartite Graph
Find Augmenting Path in General Graph
Graph Theory and its Applications
***
Finding the Shortest Path
Finding the k Shortest Path
Find Augmenting Path in General Graph
Finding kth Shortest Path
***
Find Augmenting Path in General Graph
***

***
---

【样例输入2】

1
Finding the Shortest Path
120
2
Path

Pat
0

【样例输出2】

Finding the Shortest Path

***

***

---

【样例说明】

需要查询的内容为“k shortest path”，他包含了3个独立的关键词。

【评分标准】
*
 */

/**
 * @file paper_search_engine_annotated.cpp
 * @brief 解决方案：实现一个简单的论文搜索引擎（注释版）
 *
 * 核心思路:
 * 本程序通过直接模拟搜索过程来响应查询。它首先将所有论文的基础信息（标题、引用数、输入顺序）
 * 读入内存并存储在一个 `std::vector<Paper>` 容器中。
 *
 * 对于每一个用户查询，程序会执行以下步骤：
 * 1.  **遍历所有论文**：程序会完整地遍历一遍内存中存储的所有论文。
 * 2.  **实时匹配**：对于每篇论文，调用 `containsAllKeywords` 函数进行匹配检查。
 * a.  该函数会**实时地**将论文标题和查询字符串分割成单词（分词），并全部转为小写。
 * b.  然后，它检查论文标题分词后的集合是否包含了查询字符串中的所有关键词。
 * 3.  **收集结果**：将所有匹配的论文存入一个临时的 `matchedPapers` 向量。
 * 4.  **排序**：使用一个自定义的比较函数 `comparePapers` 对 `matchedPapers` 向量进行排序。
 * 排序规则为：首先按引用次数降序，如果引用次数相同，则按论文的原始输入顺序升序。
 * 5.  **输出**：打印排序后的结果，并根据题目要求输出分隔符。
 *
 * 这种方法的优点是实现直观，但缺点是在有大量查询时效率较低，因为它对每篇论文的标题
 * 进行了反复的分词和大小写转换处理。
 *
 * 时间复杂度: O(M * (N * K * T * L + F * logF))
 * N: 论文总数, M: 查询总数, T: 标题平均词数, K: 查询平均词数, F: 匹配的论文数, L: 单词平均长度。
 * 主要开销在于处理 M 个查询，每个查询都需要遍历 N 篇论文，并进行复杂度约为 O(K*T*L) 的匹配操作。
 *
 * 空间复杂度: O(N * L_title)
 * L_title 是论文标题的平均长度。主要空间开销用于在内存中存储 N 篇论文的原始信息。
 */
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cctype>

using namespace std;

// 用于存储单篇论文信息的结构体
struct Paper {
    string title;     // 论文的原始标题
    int citations;    // 论文的被引用次数
    int order;        // 论文的原始输入顺序，用于排序时的次要关键字

    // 构造函数
    Paper(string t, int c, int o) : title(t), citations(c), order(o) {}
};

/**
 * @brief 将给定的字符串转换为全小写形式。
 * @param str 原始字符串。
 * @return 全小写的新字符串。
 */
string toLowerCase(const string& str) {
    string result = str;
    // 使用范围for循环和引用，直接在副本上修改字符
    for (char& c : result) {
        c = tolower(c);
    }
    return result;
}

/**
 * @brief 将给定的标题字符串按空格分割成一系列小写单词。
 * @param title 论文标题字符串。
 * @return 一个包含所有小写单词的vector。
 */
vector<string> splitTitle(const string& title) {
    vector<string> words;
    // stringstream 用于方便地按空格进行流式分词
    stringstream ss(title);
    string word;

    // 从流中提取每一个单词
    while (ss >> word) {
        words.push_back(toLowerCase(word)); // 转换为小写后存入
    }

    return words;
}

/**
 * @brief 检查一篇论文的标题是否包含查询中的所有关键词。
 * @param title 待检查的论文标题。
 * @param keywords 查询关键词的列表。
 * @return 如果标题包含了所有关键词，则返回true；否则返回false。
 */
bool containsAllKeywords(const string& title, const vector<string>& keywords) {
    // 每次调用时，都对标题进行实时分词和转小写处理
    vector<string> titleWords = splitTitle(title);

    // 遍历查询中的每一个关键词
    for (const string& keyword : keywords) {
        bool found = false; // 标记当前关键词是否在标题中找到
        // 遍历标题中的每一个单词
        for (const string& word : titleWords) {
            // 进行不区分大小写的比较
            if (word == toLowerCase(keyword)) {
                found = true;
                break; // 找到即跳出内层循环，检查下一个关键词
            }
        }
        // 如果当前关键词遍历完所有标题单词都未找到，则该论文不匹配
        if (!found) {
            return false;
        }
    }

    // 所有关键词都被找到了
    return true;
}

/**
 * @brief `std::sort` 使用的自定义比较函数。
 * @param a 论文a。
 * @param b 论文b。
 * @return 如果a应该排在b前面，则返回true。
 */
bool comparePapers(const Paper& a, const Paper& b) {
    // 主要排序依据：引用次数，降序排列
    if (a.citations != b.citations) {
        return a.citations > b.citations;
    }
    // 次要排序依据：原始输入顺序，升序排列
    return a.order < b.order;
}

int main() {
    int n;

    // 循环处理多组测试数据，直到输入n为0
    while (cin >> n && n != 0) {
        // 在读取整数n后，必须忽略掉这一行剩下的内容（主要是换行符），
        // 以确保后续的getline能正确读取标题行。
        cin.ignore();

        vector<Paper> papers; // 存储当前测试数据的所有论文

        // --- 阶段一：读取所有论文信息 ---
        for (int i = 0; i < n; i++) {
            string title;
            int citations;

            // 读取可能包含空格的标题
            getline(cin, title);
            // 读取引用次数
            cin >> citations;
            // 同样，忽略掉数字后的换行符
            cin.ignore();

            // 将论文信息存入vector，记录其原始输入顺序i
            papers.emplace_back(title, citations, i);
        }

        int m;
        cin >> m; // 读取查询数量
        cin.ignore();

        // --- 阶段二：逐个处理所有查询 ---
        for (int i = 0; i < m; i++) {
            string query;
            getline(cin, query); // 读取完整的查询行

            // 1. 将查询字符串分词
            vector<string> keywords;
            stringstream ss(query);
            string keyword;

            while (ss >> keyword) {
                keywords.push_back(keyword);
            }

            // 2. 遍历所有论文，找到匹配项
            vector<Paper> matchedPapers;

            for (const Paper& paper : papers) {
                if (containsAllKeywords(paper.title, keywords)) {
                    matchedPapers.push_back(paper);
                }
            }

            // 3. 对匹配结果进行排序
            sort(matchedPapers.begin(), matchedPapers.end(), comparePapers);

            // 4. 输出排序后的结果
            for (const Paper& paper : matchedPapers) {
                cout << paper.title << endl;
            }

            // 每个查询结束后输出分隔符
            cout << "***" << endl;
        }

        // 每组测试数据结束后输出分隔符
        cout << "---" << endl;
    }

    return 0;
}