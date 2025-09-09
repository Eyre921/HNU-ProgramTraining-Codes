/*
* 17. 飞行棋
【问题描述】

       大家当年一定都下过飞行棋吧。现在Lele和Yueyue要下的棋和这个很相似，只是更简单一点而已。

       棋盘由N个格子组成，分别标记为第0格到第N-1格。格子分为两种，一种是普通格子，即表示在该格可以停留。否则是特殊的格子，一旦走到上面，就要根据上面标记的数飞到相应的格子上。如果飞到一个特殊的格子上，则可以继续飞。

       除了第0格外，其他格子都只能容纳一个玩家。即一旦A玩家已经在某个格子上，B玩家又走到这里，A玩家则会被踢回第0格，而B玩家留在这个格子上面。

       第N-1个格子是终点，一旦一个玩家走到这个格子上，该玩家获胜，游戏结束。

       刚刚开始时，两个玩家都站在第0格上，依次扔骰子，根据骰子显示的点数走相应的格子数。比如，玩家在第0格，扔出了5点，则会走到第5个格子上。如果玩家走得超出了棋盘的范围，则要往回走一定的步数。比如，棋盘一共有7(0~6)个格子,玩家在第4格上，扔出了6点，最终他会走到第2格上(4->5->6->5->4->3->2)。

       根据观察，骰子扔出来的数也是有规律的。
       对于每一盘棋，扔出的第一个点数为 F0=(A*C+B)%6+1,第二个点数为 F1=(A*F0+B)%6+1,第三个点数为 F2=(A*F1+B)%6+1 ....依此类推。

       每一盘棋都是由Lele先走，现在就请你当裁判，看谁能获胜。

【输入形式】

      本题目包含多组测试，请处理到文件结束。
      每组数据占两行。
      第一行有4个整数N,A,B,C(含义见题目描述，6<N<200,0<=A,B,C<=2^31)。
      第二行有N个字符串，分别表示棋盘上第0个到第N-1个格子的内容。两个字符串之间用一个空格分隔开。

      如果字符串为"N",则表示这个格子为普通格子。否则字符串为"GX"(X为0到N-1之间的整数)的形式，其中X表示玩家走到这个格子时，要马上飞到第X个格子。
      数据保证第0个和第N-1个格子一定为"N"。

【输出形式】

      对于每组数据，在一行内输出结果。
      如果Lele能赢这盘棋，则输出"Lele",如果Yueyue赢的话，就输出"Yueyue"。

【样例输入】

7 1 0 6
N G3 N N N N N
7 1 0 6
N G4 N N N N N
【样例输出】

Lele
Yueyue
【样例说明】

测试用例保证能有确定结果。
【评分标准】


 */
/**
 * @file flying_chess_simulator.cpp
 * @brief 解决方案：模拟一个双人飞行棋游戏并判断胜者
 *
 * 核心思路:
 * 这是一个纯粹的模拟问题，关键在于精确地实现游戏的所有规则，并清晰地管理游戏状态。
 *
 * 1.  游戏状态管理:
 * -   棋盘: 使用一个 `std::vector<Square>` 来表示。其中 `Square` 是一个自定义结构体，
 * 包含格子的类型（普通或特殊）以及特殊格子需要飞往的目标位置。
 * -   玩家位置: 用两个整型变量 `posLele` 和 `posYueyue` 记录。
 * -   骰子状态: 骰子点数序列是确定的。用一个 `long long` 类型的变量 `diceState`
 * 来存储上一轮的点数（或初始值C），用于生成下一轮的点数。
 *
 * 2.  核心逻辑封装:
 * -   为了避免代码冗余并提高可读性，将一个玩家走一步的完整逻辑封装成一个辅助函数
 * `takeTurn()`。
 * -   `takeTurn()` 函数负责：
 * a.  根据 `diceState`、A 和 B 生成新的骰子点数。
 * b.  计算棋子移动后的临时位置。
 * c.  处理超出棋盘范围的情况（“反弹”规则）。
 * d.  处理特殊格子导致的连续飞行，直到棋子落在一个普通格子上。
 * e.  更新玩家的最终位置。
 *
 * 3.  主模拟循环:
 * -   在主函数中，为每组测试数据设置一个 `while(true)` 循环来模拟游戏进程。
 * -   循环内部，轮流调用 `takeTurn()` 函数让Lele和Yueyue走棋。
 * -   每当一个玩家完成移动后，立即检查：
 * a.  该玩家是否到达终点 (N-1)，如果是，则宣布胜者并结束本局游戏。
 * b.  该玩家是否与另一玩家在同一个非起始格子上，如果是，则将被占位的玩家“踢”回
 * 起点 (0)。
 *
 * 4.  注意事项:
 * -   输入参数A, B, C的范围较大，在计算骰子点数时必须使用 `long long` 以防止溢出。
 * -   反弹规则的计算公式为：`finalPos = (N-1) - (rawNewPos - (N-1))`。
 *
 * 时间复杂度: O(T)
 * T是游戏结束所需的总回合数。每个回合的计算（掷骰子、移动、飞行）都是常数时间或
 * 很快收敛（飞行链不会无限长）。
 *
 * 空间复杂度: O(N)
 * 主要用于存储N个格子的棋盘信息。
 */

#include <iostream>
#include <vector>
#include <string>

// 使用标准命名空间，简化代码
using namespace std;

// 定义棋盘格子的结构体，增加代码可读性
struct Square {
    bool isSpecial = false;
    int flyTo = 0;
};

/**
 * @brief 执行一个玩家的回合
 * @param N 棋盘大小
 * @param A, B 骰子生成参数
 * @param board 棋盘布局
 * @param currentPos 当前玩家的位置 (引用，将被修改)
 * @param diceState 上一轮的骰子状态 (引用，将被修改)
 */
void takeTurn(int N, long long A, long long B, const vector<Square>& board,
              int& currentPos, long long& diceState) {
    // 1. 生成本次骰子点数
    diceState = (A * diceState + B) % 6 + 1;
    int diceRoll = static_cast<int>(diceState);

    // 2. 计算初步移动位置
    int newPos = currentPos + diceRoll;

    // 3. 处理超出棋盘范围的反弹情况
    if (newPos >= N) {
        newPos = (N - 1) - (newPos - (N - 1));
    }

    // 4. 处理特殊格子的连续飞行
    while (board[newPos].isSpecial) {
        newPos = board[newPos].flyTo;
    }

    // 5. 更新玩家最终位置
    currentPos = newPos;
}


void solve() {
    int N;
    long long A, B, C;

    // 处理到文件结束 (End-Of-File)
    while (cin >> N >> A >> B >> C) {
        // --- 初始化棋盘 ---
        vector<Square> board(N);
        for (int i = 0; i < N; ++i) {
            string cell_info;
            cin >> cell_info;
            if (cell_info[0] == 'G') {
                board[i].isSpecial = true;
                // 从字符串"GX"中解析出整数X
                board[i].flyTo = stoi(cell_info.substr(1));
            }
        }

        // --- 初始化游戏状态 ---
        int posLele = 0;
        int posYueyue = 0;
        long long diceState = C; // F0的计算需要C作为"F-1"

        // --- 开始游戏模拟 ---
        while (true) {
            // Lele's turn
            takeTurn(N, A, B, board, posLele, diceState);
            if (posLele == N - 1) {
                cout << "Lele" << endl;
                break;
            }
            // 检查是否踢掉对方
            if (posLele != 0 && posLele == posYueyue) {
                posYueyue = 0;
            }

            // Yueyue's turn
            takeTurn(N, A, B, board, posYueyue, diceState);
            if (posYueyue == N - 1) {
                cout << "Yueyue" << endl;
                break;
            }
            // 检查是否踢掉对方
            if (posYueyue != 0 && posYueyue == posLele) {
                posLele = 0;
            }
        }
    }
}


int main() {
    // 优化C++标准流的IO性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    solve();

    return 0;
}