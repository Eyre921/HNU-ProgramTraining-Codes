/*8. 数圈
【问题描述】

以1为中心，用2,3,4, ..., n, ..., n*n的数字围绕着中心输出数圈， 如若n=4，则

7 8 9 10

6 1 2 11

5 4 3 12

16 15 14 13


【输入形式】

一个整数n(1<=n<=10)

【输出形式】

数圈矩阵

【样例输入】

5
【样例输出】

21 22 23 24 25
20 7 8 9 10
19 6 1 2 11
18 5 4 3 12
17 16 15 14 13
*/

/**
 * @file number_spiral.cpp
 * @brief 解决方案：生成以1为中心的螺旋数阵（数圈）
 *
 * 核心思路:
 * 这个问题可以通过模拟数字填充路径来解决。数字从中心点'1'开始，
 * 以逆时针螺旋的方式向外扩展。
 *
 * 1.  初始化：
 * a. 创建一个 N x N 的二维数组（或`vector<vector<int>>`），并用0填充。
 * b. 确定中心点'1'的坐标。对于一个 N x N 的矩阵，中心点的索引
 * 总是 `((N-1)/2, (N-1)/2)`，这对于N为奇数或偶数都适用。
 * c. 初始化当前坐标 `(x, y)` 为中心点坐标，当前待填充数字 `currentNum` 为1。
 *
 * 2.  螺旋填充：
 * a. 将'1'放入中心点。
 * b. 观察螺旋路径，它遵循一个“右-下-左-左-上-上-右-右-右...”的模式。
 * 更具体地说，它遵循“右(1步)-下(1步)-左(2步)-上(2步)-右(3步)-下(3步)...”的规律。
 * c. 我们可以用一个循环来控制这个过程。循环的每一轮包含四个方向的移动。
 * 移动的步数 `steps` 在每完成“右-下”或“左-上”一组移动后增加。
 * d. 循环不断进行，直到填充的数字 `currentNum` 超过 `N*N` 为止。
 * 在填充过程中，要时刻检查 `currentNum` 是否已超出范围，以处理矩阵填满的边界情况。
 *
 * 3.  输出：
 * a. 填充完毕后，遍历整个二维数组。
 * b. 使用`<iomanip>`库中的`setw`来设置固定的字段宽度，确保输出的数字
 * 能够整齐对齐，形成美观的矩阵。
 *
 * 时间复杂度: O(N^2)
 * 我们需要填充一个 N x N 的矩阵，每个单元格仅被访问和赋值一次。
 *
 * 空间复杂度: O(N^2)
 * 需要一个 N x N 的二维数组来存储整个数圈矩阵。
 */

#include <iostream>   // 用于标准输入输出 (cin, cout)
#include <vector>     // 用于使用vector容器
#include <iomanip>    // 用于使用setw等格式化输出工具

// 使用标准命名空间，简化代码
using namespace std;

int main() {
    // 优化C++标准流的IO性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    // 如果输入为0或负数，则不进行任何操作
    if (n <= 0) {
        return 0;
    }

    // 创建一个 n x n 的二维vector，并初始化所有值为0
    vector<vector<int>> matrix(n, vector<int>(n, 0));

    // --- 螺旋填充逻辑 ---

    // 确定中心起始坐标
    int currentY = (n - 1) / 2;
    int currentX = (n - 1) / 2;

    int currentNum = 1; // 当前要填充的数字
    int steps = 1;      // 当前方向要走的步数

    // 首先填充中心点'1'
    matrix[currentY][currentX] = currentNum++;

    // 循环直到所有数字填充完毕
    while (currentNum <= n * n) {
        // 向右移动 steps 步
        for (int i = 0; i < steps && currentNum <= n * n; ++i) {
            currentX++;
            matrix[currentY][currentX] = currentNum++;
        }

        // 向下移动 steps 步
        for (int i = 0; i < steps && currentNum <= n * n; ++i) {
            currentY++;
            matrix[currentY][currentX] = currentNum++;
        }

        // 增加步数，因为螺旋圈扩大了
        steps++;

        // 向左移动 steps 步
        for (int i = 0; i < steps && currentNum <= n * n; ++i) {
            currentX--;
            matrix[currentY][currentX] = currentNum++;
        }

        // 向上移动 steps 步
        for (int i = 0; i < steps && currentNum <= n * n; ++i) {
            currentY--;
            matrix[currentY][currentX] = currentNum++;
        }

        // 再次增加步数
        steps++;
    }

    // --- 格式化输出矩阵 ---

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << matrix[i][j] << ' ';
        }
        cout << endl; // 每行结束后换行
    }

    return 0;
}