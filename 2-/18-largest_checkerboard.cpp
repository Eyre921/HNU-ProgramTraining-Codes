/*
*【问题描述】

棋盘是指一个行和列编号从1~N的NxN的二进制矩阵，当行号和列号之和为偶数时该矩阵对应位置为黑色的(1)，否则为白色的(0)。以下图示为N=1、2、3时的棋盘。

給出一个NxN的二进制矩阵，请找出位于该矩阵内的最大尺寸的完整棋盘，以及最大尺寸棋盘的数量（棋盘可以交叠）。

【输入形式】

每个测试用例的第一行是一个正整数N(1<=N<=2000)，表示給定矩阵的行数和列数，接下来的N行描述了这个矩阵：每行有N个字符，既可以是“1”（代表黑块），也可以是“0”（代表白块）。矩阵至少包含一个“1”字符。

【输出形式】

输出最大尺寸棋盘的行列的大小，以及最大棋盘的个数，以空格分隔。

【样例输入】

5
00101
11010
00101
01010
11101
【样例输出】

3 3
*/
/**
 * @file largest_checkerboard_user_impl.cpp
 * @brief 解决方案：寻找矩阵内最大尺寸的完整棋board及其数量（原始代码注释版）
 *
 * 核心思路:
 * 本代码采用一种独特的动态规划思想来解决此问题。其核心在于，它定义并寻找一种特殊的
 * “1-对角线棋盘”——即主对角线（从左上到右下）元素全为1的棋盘。
 *
 * 1.  状态定义:
 * `dp[i][j]` 代表以 `(i, j)` 为右下角，且 `(i, j)` 的值为1的“1-对角线棋盘”的最大边长。
 *
 * 2.  初始化:
 * -   首先对整个矩阵进行一次遍历，读取数据。
 * -   在此过程中，对 `dp` 表进行初始化：如果 `matrix[i][j]` 为1，则 `dp[i][j]`
 * 初始化为1，表示一个1x1的棋盘。如果为0，则 `dp[i][j]` 为0。
 * -   同时，`maxSize` 初始化为1，`cnt` 初始化为矩阵中1的总数，这作为找到
 * 更大棋盘之前的默认解。
 *
 * 3.  状态转移:
 * -   算法的核心在于一个双层循环，它检查是否能从一个已知的、以 `(i-1, j-1)` 为右下角
 * 的棋盘扩展出一个更大的、以 `(i, j)` 为右下角的棋盘。
 * -   扩展的**前提条件**是 `matrix[i-1][j-1]` 和 `matrix[i][j]` 都必须为1，
 * 这符合“1-对角线棋盘”的定义。
 * -   如果满足前提，算法会基于 `dp[i-1][j-1]` 的已知大小，确定一个搜索边界。
 * -   随后，通过两个精巧的 `while` 循环，从 `(i,j)` 开始，同时向左和向上检查新扩展
 * 的“臂”是否满足交错的颜色模式。
 * -   `cnt1` 和 `cnt2` 分别计算了这两条“臂”上满足两种不同棋盘模式（一种对角线为1，
 * 另一种反对角线为1）的最大延伸长度。
 * -   `dp[i][j]` 的值最终由这两条臂的长度决定。
 * -   在计算出每个 `dp[i][j]` 后，实时更新全局的最大尺寸 `maxSize` 和对应的计数 `cnt`。
 *
 * 时间复杂度: O(N^3)
 * 在双层 for 循环内部，存在 while 循环，其迭代次数与 dp 值相关，最坏情况下接近 O(N)。
 *
 * 空间复杂度: O(N^2)
 * 用于存储原始矩阵和DP表。
 */
#include <iostream>
#include <algorithm> // 包含 max 函数

using namespace std;

int main() {
    // 优化C++标准流的IO性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // dimension: 矩阵的维度 N
    // globalMaxSize: 全局记录的最大棋盘尺寸
    // countOfMaxSize: 最大尺寸棋盘的数量
    int dimension, globalMaxSize = 1, countOfMaxSize = 0;
    char inputChar; // 用于临时存储输入的字符 '0' 或 '1'

    cin >> dimension;

    // 警告: 使用变长数组(VLA)不是标准的C++特性，可能在某些编译器上无法编译。
    // boardSizeDP: 动态规划表, boardSizeDP[i][j] 存储以 (i,j) 为右下角的棋盘尺寸
    // matrix: 存储输入的二进制矩阵
    int boardSizeDP[dimension][dimension], matrix[dimension][dimension];

    // --- 阶段一：读取输入并初始化DP表 ---
    for (int i = 0; i < dimension; ++i) {
        for (int j = 0; j < dimension; ++j) {
            cin >> inputChar;
            matrix[i][j] = inputChar - '0'; // 将字符 '0'/'1' 转换为整数 0/1

            // 初始化dp表：只有值为1的格子才能作为1x1的“1-对角线棋盘”
            if (inputChar == '1') {
                boardSizeDP[i][j] = 1;
                // 初始时，假设最大尺寸为1，统计所有1x1棋盘（即所有'1'）的数量
                countOfMaxSize++;
            } else {
                boardSizeDP[i][j] = 0;
            }
        }
    }

    // --- 阶段二：核心动态规划过程 ---
    // 从 (1,1) 开始遍历，因为算法依赖于左上方的格子 (i-1, j-1)
    for (int i = 1; i < dimension; ++i) {
        for (int j = 1; j < dimension; ++j) {

            // 扩展的前提条件：尝试扩展的2x2区域的左上角和右下角都必须是1
            if (matrix[i - 1][j - 1] == 1 && matrix[i][j] == 1) {
                // searchRowBound / searchColBound: 搜索的边界，由左上角已知棋盘大小决定
                int searchRowBound = i - boardSizeDP[i - 1][j - 1];
                int searchColBound = j - boardSizeDP[i - 1][j - 1];

                // diagonalPatternArmLength: 检查新扩展的“主对角线”颜色臂的长度
                // antiDiagonalPatternArmLength: 检查新扩展的“反对角线”颜色臂的长度
                int diagonalPatternArmLength = 1;
                int antiDiagonalPatternArmLength = 0;

                // 检查指针，从 (i-2, j-2) 开始沿对角线向左上方移动
                int checkRow = i - 2, checkCol = j - 2;

                // 第一个while循环：检查“主对角线”模式臂（臂上应为1）
                while (checkRow >= searchRowBound && checkCol >= searchColBound) {
                    // 检查新行和新列的对应位置是否都为1
                    if (matrix[checkRow][j] == 0 || matrix[i][checkCol] == 0) break;
                    checkRow -= 2; // 跳格检查，以匹配棋盘的交错模式
                    checkCol -= 2;
                    diagonalPatternArmLength += 2;
                }

                // 重置检查指针
                checkRow = i - 1, checkCol = j - 1;

                // 第二个while循环：检查“反对角线”模式臂（臂上应为0）
                while (checkRow >= searchRowBound && checkCol >= searchColBound) {
                    // 检查新行和新列的对应位置是否都为0
                    if (matrix[checkRow][j] == 1 || matrix[i][checkCol] == 1) break;
                    checkRow -= 2; // 跳格检查
                    checkCol -= 2;
                    antiDiagonalPatternArmLength += 2;
                }

                // 根据两种模式臂的有效长度，更新当前格子的dp值
                if (antiDiagonalPatternArmLength > 0) {
                    boardSizeDP[i][j] = max(diagonalPatternArmLength, antiDiagonalPatternArmLength);
                } else {
                    boardSizeDP[i][j] = 1;
                }

                // 实时更新全局最大尺寸和计数
                if (boardSizeDP[i][j] > globalMaxSize) {
                    globalMaxSize = boardSizeDP[i][j];
                    countOfMaxSize = 1; // 发现新的最大尺寸，计数重置为1
                } else if (boardSizeDP[i][j] == globalMaxSize) {
                    countOfMaxSize++; // 发现一个同样大小的棋盘，计数加一
                }
            }
        }
    }

    // 输出最终结果
    cout << globalMaxSize << " " << countOfMaxSize;

    return 0;
}