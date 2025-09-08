/*
16. 点球大战
【问题描述】在足球比赛中，有不少赛事，例如世界杯淘汰赛和欧洲冠军联赛淘汰赛中，当比赛双方经过正规比赛和加时赛之后仍然不分胜负时，需要进行点球大战来决定谁能够获得最终的胜利。点球大战的规则非常简单，两方轮流派出球员罚点球，每方各罚5个。当5轮点球结束以后如果仍然不分胜负，则进入一轮定胜负的阶段。两方各派一名球员罚点球，直到有一方罚进而另一方没有进为止。
在北美职业冰球联赛中，也有点球大战。与足球的规则不同的是，它只先罚3轮点球，随后就进入一轮定胜负的阶段，而其他的规则完全一样。
在本题中，输入将给出每次点球是否罚进，而你的任务则是输出一个“比分板”。

【输入形式】输入包含多组数据。每组数据的第一行包含一个整数N(1<=N<=18)，表示双方总共罚了多少个点球，N=0表示输入结束。随后有N行，每行是一个如下形式的字符串：
XXXX good：表示这个点球罚进
或者XXXX no good：表示这个点球没有罚进
其中XXXX表示球员名字（全部由字母和空格组成，保证不会出现歧义）
每一行保证不超过100个字符。
XXXX和good以及XXXX和no、no和good之间保证有且只有1个空格。
good、no good都是小写。本题是大小写相关的。
数据不保证点球大战一定结束，也不保证在结束以后立即结束这组数据（即：不用判断点球大战是否结束，只用把罚进的点球往比分上加即可）。

【输出形式】对每组数据，输出一个比分板。一个点球如果罚进，则在对应的地方标上’O’，如果没有进则标上’X’。先罚球的队伍的信息在上面，后罚的在下面。最右边标上两队的比分。具体格式参考样例输出。注意如果一轮点球只罚了一个，则后面那个点球对应的地方写上’-’。

【样例输入】

6
Riise good
Ballack good
Gerrard no good
Lampard no good
Fernando Torres good
Malouda good
9
Christiano Ronaldo no good
Messi no good
Giggs good
Abidal no good
Carrick good
Ronaldinho good
Rooney good
Henry no good
Tevez good
0

【样例输出】

1 2 3 Score
O X O 2
O X O 2
1 2 3 4 5 Score
X O O O O 4
X X O X - 1

【样例说明】
【评分标准】


 *
 */
/**
 * @file penalty_shootout.cpp
 * @brief 解决方案：生成点球大战的比分板
 *
 * 核心思路:
 * 1.  数据结构：
 * -   使用两个 `std::vector<char>` 分别存储两队（先罚球队Team A，后罚球队Team B）
 * 每次点球的结果（'O' 表示进球, 'X' 表示未进）。
 * -   使用两个整型变量 `scoreA` 和 `scoreB` 来实时统计两队的得分。
 *
 * 2.  输入处理：
 * -   由于球员姓名可能包含空格，不能简单使用 `cin >> ...` 来读取。
 * 最佳实践是使用 `getline` 来读取一整行。
 * -   在读取每组数据的 N 之后，需要使用 `cin.ignore()` 来清除换行符，
 * 为后续的 `getline` 做准备。
 * -   点球轮次是交替进行的：第1、3、5...次罚球属于 Team A，第2、4、6...次罚球
 * 属于 Team B。我们可以通过罚球的序号（1-indexed）的奇偶性来判断。
 * -   判断是否罚进：可以通过检查读取的整行字符串是否以 "good" 结尾来确定。
 * `string::rfind()` 是一个非常适合此任务的函数。
 *
 * 3.  逻辑处理与输出：
 * a.  循环N次，处理每一次罚球，将结果存入对应队伍的 `vector` 并更新分数。
 * b.  处理完所有N次罚球后，检查两队的罚球次数是否相等。如果不等（即N为奇数），
 * 说明最后一轮未完成，后罚的队伍需要一个占位符 '-'。
 * c.  输出时，严格按照样例格式分为三行：
 * i.   标题行：根据总轮数（即罚球较多队伍的罚球次数）打印轮次数字。
 * ii.  Team A 行：打印 Team A 的所有罚球结果，最后打印其总分。
 * iii. Team B 行：打印 Team B 的所有罚球结果（可能包含'-'），最后打印其总分。
 *
 * 时间复杂度: O(N * L)
 * 对于每组测试数据，N是罚球总数，L是输入行的最大长度。主要开销在于读取和
 * 解析N行输入。
 *
 * 空间复杂度: O(N)
 * 主要用于存储两队罚球结果的 `vector`。由于N最大为18，空间开销非常小。
 */

#include <iostream>
#include <string>
#include <vector>
#include <limits> // For numeric_limits, used in cin.ignore()

// 使用标准命名空间，简化代码
using namespace std;

void solve() {
    int n;
    // 循环处理多组测试数据，直到n为0
    while (cin >> n && n != 0) {
        // 在读取n后，必须忽略掉这一行剩下的内容，特别是换行符
        // 以便 getline 能够正确地从下一行开始读取
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        vector<char> resultsA;
        vector<char> resultsB;
        int scoreA = 0;
        int scoreB = 0;

        for (int i = 1; i <= n; ++i) {
            string line;
            getline(cin, line);

            // 通过检查字符串末尾来判断结果，这样可以正确处理带空格的姓名
            // " no good" 长度为 8, " good" 长度为 5
            bool isGood = (line.rfind(" good") != string::npos &&
                           line.rfind(" no good") == string::npos);

            // i为奇数，是Team A罚球
            if (i % 2 != 0) {
                if (isGood) {
                    resultsA.push_back('O');
                    scoreA++;
                } else {
                    resultsA.push_back('X');
                }
            }
            // i为偶数，是Team B罚球
            else {
                if (isGood) {
                    resultsB.push_back('O');
                    scoreB++;
                } else {
                    resultsB.push_back('X');
                }
            }
        }

        // 如果Team A比Team B多罚了一轮，为Team B补上占位符
        if (resultsA.size() > resultsB.size()) {
            resultsB.push_back('-');
        }

        // --- 开始输出比分板 ---

        int numRounds = resultsA.size();

        // 1. 输出标题行
        for (int i = 1; i <= numRounds; ++i) {
            cout << i << " ";
        }
        cout << "Score" << endl;

        // 2. 输出Team A（先罚球队）的结果
        for (char res : resultsA) {
            cout << res << " ";
        }
        cout << scoreA << endl;

        // 3. 输出Team B（后罚球队）的结果
        for (char res : resultsB) {
            cout << res << " ";
        }
        cout << scoreB << endl;
    }
}

int main() {
    // 优化C++标准流的IO性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    solve();

    return 0;
}