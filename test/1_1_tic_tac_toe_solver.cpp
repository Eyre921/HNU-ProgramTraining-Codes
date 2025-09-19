/*
 * @file tic_tac_toe_solver.cpp
 * @brief 解决井字棋胜负判断问题
 *
 * 核心思路:
 * 1.  主要逻辑分为两个部分：首先判断出棋盘上的获胜棋子是'1'还是'2'，然后根据该局的先手方确定最终的获胜玩家。
 * 2.  为了找出获胜棋子，我们编写一个辅助函数 `findWinningPiece`。该函数会检查所有8种可能的获胜组合：3行、3列和2条对角线。
 * 3.  一旦在某条线上发现三个相同的非'0'棋子，该函数就立即返回这个棋子的字符（'1'或'2'）。
 * 4.  在主函数中，我们循环处理n个对局。对于每个对局，读取棋盘布局和先手玩家信息。
 * 5.  调用 `findWinningPiece` 得到获胜棋子。
 * 6.  根据映射关系：先手执'1'，后手执'2'。如果获胜棋子是'1'，则先手方获胜；如果是'2'，则后手方获胜。由此确定并输出最终的赢家。
 *
 * 时间复杂度: O(N)
 * 对于N个对局中的每一个，我们都需要读取一个3x3的棋盘并检查8条线来判断胜负。这是一个常数时间的操作。因此，总的时间复杂度与对局数量N成正比。
 *
 * 空间复杂度: O(1)
 * 除了存储输入的字符串和棋盘（每次一个），我们没有使用额外的随输入规模增长的存储空间。判断胜负的函数仅使用常数大小的空间。
 */

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// 函数声明，用于寻找获胜的棋子
char findWinningPiece(const vector<string>& board);

int main() {
    // 优化输入输出流的性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int numGames; // 对局的总次数
    cin >> numGames;

    string firstMovers; // 记录每场对局先手者的字符串
    cin >> firstMovers;

    // 循环处理每一次对局
    for (int i = 0; i < numGames; ++i) {
        vector<string> board(3); // 使用vector<string>存储3x3的棋盘
        // 读取当前对局的棋盘状态
        for (int j = 0; j < 3; ++j) {
            cin >> board[j];
        }

        // 找出获胜的棋子是 '1' 还是 '2'
        char winningPiece = findWinningPiece(board);

        // 获取当前对局的先手玩家
        char firstPlayer = firstMovers[i];

        char finalWinner; // 最终的获胜者

        // 判断最终胜者
        // 如果获胜棋子是'1'，那么先手者获胜
        if (winningPiece == '1') {
            finalWinner = firstPlayer;
        }
        // 如果获胜棋子是'2'，那么后手者获胜
        else {
            finalWinner = (firstPlayer == 'A') ? 'B' : 'A';
        }

        // 输出当前对局的获胜者
        cout << finalWinner << endl;
    }

    return 0;
}

/**
 * @brief 分析给定的3x3井字棋棋盘，找出获胜的棋子。
 * @param board 一个3x3的棋盘，用`vector<string>`表示。
 * @return 返回获胜棋子的字符 ('1' 或 '2')。题目保证一定有唯一胜者。
 */
char findWinningPiece(const vector<string>& board) {
    // 检查所有行是否构成胜利条件
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] != '0' && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            return board[i][0];
        }
    }

    // 检查所有列是否构成胜利条件
    for (int i = 0; i < 3; ++i) {
        if (board[0][i] != '0' && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            return board[0][i];
        }
    }

    // 检查主对角线 (左上到右下)
    if (board[0][0] != '0' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        return board[0][0];
    }

    // 检查副对角线 (右上到左下)
    if (board[0][2] != '0' && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        return board[0][2];
    }

    // 根据题目保证，总会有一个胜者，所以理论上不会执行到这里
    return '0';
}
