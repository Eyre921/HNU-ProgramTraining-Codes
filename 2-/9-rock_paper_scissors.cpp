/*9. 锤子剪刀布
【问题描述】

大家应该都会玩“锤子剪刀布”的游戏。现给出两人的交锋记录，请统计双方的胜、平、负次数，并且给出双方分别出什么手势的胜算最大。

【输入形式】

输入第1行给出正整数N（<=105），即双方交锋的次数。随后N行，每行给出一次交锋的信息，即甲、乙双方同时给出的的手势。C代表“锤子”、J代表“剪刀”、B代表“布”，第1个字母代表甲方，第2个代表乙方，中间有1个空格。

【输出形式】

输出第1、2行分别给出甲、乙的胜、平、负次数，数字间以1个空格分隔。第3行给出两个字母，分别代表甲、乙获胜次数最多的手势，中间有1个空格。如果解不唯一，则输出按字母序最小的解。

【样例输入】

10
C J
J B
C B
B B
B C
C C
C B
J B
B C
J J
【样例输出】

5 3 2
2 3 5
B B
*/

/**
 * @file rock_paper_scissors.cpp
 * @brief 解决方案：统计“锤子剪刀布”游戏结果
 *
 * 核心思路:
 * 1.  数据结构：
 * -   使用三个整数 `winsA`, `ties`, `lossesA` 来分别记录甲的胜、平、负次数。
 * 乙的胜平负次数可以由甲的直接推导出来（乙的胜 = 甲的负，以此类推）。
 * -   使用两个 `std::map<char, int>` (或普通数组) `winMovesA` 和 `winMovesB`
 * 来分别记录甲和乙使用每种手势（'B', 'C', 'J'）获胜的次数。
 *
 * 2.  处理逻辑：
 * -   循环N次，每次读取甲乙双方的手势。
 * -   在循环中，判断当前一局的胜负关系：
 * a.  如果双方手势相同，则为平局，`ties` 增加。
 * b.  通过 `if/else if` 判断所有甲胜的情况（C > J, J > B, B > C）。
 * 如果甲胜，则 `winsA` 增加，并且在 `winMovesA` 中记录甲获胜时
 * 使用的手势次数加一。
 * c.  剩余情况则为乙胜，`lossesA` 增加，并且在 `winMovesB` 中记录乙
 * 获胜时使用的手势次数加一。
 *
 * 3.  结果输出：
 * -   直接输出甲的胜平负次数。
 * -   根据甲的结果，计算并输出乙的胜平负次数。
 * -   编写一个辅助函数 `findBestMove` 来找出获胜次数最多的手势。
 * 这个函数需要处理“解不唯一时，输出按字母序最小的解”的规则。
 * 我们可以通过比较 'B', 'C', 'J' 的获胜次数来实现。由于 'B' < 'C' < 'J'，
 * 我们可以按B, C, J的顺序检查，优先选择获胜次数最多的手势。
 *
 * 时间复杂度: O(N)
 * 我们需要遍历N次交锋记录，循环内部的操作（判断、计数）都是常数时间。
 *
 * 空间复杂度: O(1)
 * 我们使用了固定数量的变量和两个固定大小的map来存储统计数据，
 * 所需空间不随输入N的增长而增长。
 */

#include <iostream> // 用于标准输入输出 (cin, cout)
#include <string>   // 用于使用string类
#include <vector>   // 虽然不是必须，但包含进来是好习惯
#include <map>      // 用于使用map来统计获胜手势

// 使用标准命名空间，简化代码
using namespace std;

/**
 * @brief 判断一局游戏中甲是否获胜
 * @param moveA 甲的手势
 * @param moveB 乙的手势
 * @return 如果甲获胜返回true，否则返回false
 */
bool doesAWin(char moveA, char moveB) {
    return (moveA == 'C' && moveB == 'J') ||
           (moveA == 'J' && moveB == 'B') ||
           (moveA == 'B' && moveB == 'C');
}

/**
 * @brief 从记录获胜次数的map中找出最佳手势
 * @param winMoves 记录了'B', 'C', 'J'各自获胜次数的map
 * @return 获胜次数最多且字母序最小的手势字符
 */
char findBestMove(const map<char, int>& winMoves) {
    // 默认最佳手势为B，因为字母序最小
    char bestMove = 'B';

    // 比较C和当前最佳手势B
    // 如果C的胜场更多，则更新最佳手势为C
    if (winMoves.at('C') > winMoves.at(bestMove)) {
        bestMove = 'C';
    }

    // 比较J和当前最佳手势
    // 如果J的胜场更多，则更新最佳手势为J
    if (winMoves.at('J') > winMoves.at(bestMove)) {
        bestMove = 'J';
    }

    return bestMove;
}

int main() {
    // 优化C++标准流的IO性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    // 初始化甲的胜、平、负计数
    int winsA = 0;
    int ties = 0;
    int lossesA = 0;

    // 初始化记录甲乙双方获胜手势的map
    map<char, int> winMovesA = {{'B', 0}, {'C', 0}, {'J', 0}};
    map<char, int> winMovesB = {{'B', 0}, {'C', 0}, {'J', 0}};

    for (int i = 0; i < n; ++i) {
        char moveA, moveB;
        cin >> moveA >> moveB;

        if (moveA == moveB) {
            // 情况一：平局
            ties++;
        } else if (doesAWin(moveA, moveB)) {
            // 情况二：甲获胜
            winsA++;
            winMovesA[moveA]++;
        } else {
            // 情况三：乙获胜（甲失败）
            lossesA++;
            winMovesB[moveB]++;
        }
    }

    // --- 输出结果 ---

    // 第1行: 甲的胜、平、负次数
    cout << winsA << " " << ties << " " << lossesA << endl;

    // 第2行: 乙的胜、平、负次数
    cout << lossesA << " " << ties << " " << winsA << endl;

    // 第3行: 甲、乙获胜次数最多的手势
    char bestMoveA = findBestMove(winMovesA);
    char bestMoveB = findBestMove(winMovesB);
    cout << bestMoveA << " " << bestMoveB << endl;

    return 0;
}