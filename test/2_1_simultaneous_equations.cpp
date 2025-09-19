/*
 * 核心思路:
 * 本题要求解N个二元一次方程两两组成的方程组的交点。对于任意两个方程：
 * A1*x + B1*y + C1 = 0
 * A2*x + B2*y + C2 = 0
 * 我们可以通过克莱姆法则或代数消元法求得其交点坐标的精确解。
 * 通过消元法可以推导出公式：
 * x = (B1*C2 - B2*C1) / (A1*B2 - A2*B1)
 * y = (C1*A2 - C2*A1) / (A1*B2 - A2*B1)
 *
 * 题目提示使用浮点数可能存在精度误差，并要求输出整数部分。因此，最稳健的方法是全程使用整型进行计算。
 * 我们将上述公式的分子和分母分别计算出来，然后利用C++的整型除法特性（结果会自动向零取整）来得到坐标的整数部分，
 * 这恰好满足题目的输出要求。
 *
 * 题目保证任意两条直线有且仅有一个交点，这意味着公式中的分母 (A1*B2 - A2*B1) 永远不为零，我们无需处理除以零的异常情况。
 *
 * 时间复杂度: O(N^2)
 * 我们需要遍历所有方程对，其中N是方程的数量。这需要一个嵌套循环，因此时间复杂度为 O(N^2)。
 *
 * 空间复杂度: O(N)
 * 我们需要存储N个方程的系数，每个方程有3个系数。因此，空间复杂度为 O(N)。
 */

#include <iostream>
#include <vector>

// 题目允许，直接使用标准命名空间简化代码
using namespace std;

// 定义一个结构体来清晰地表示一个方程 Ax + By + C = 0 的三个系数
struct Equation {
    long long A, B, C;
};

int main() {
    // 优化C++标准流的输入输出速度，在处理大量数据时效果显著
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int equationCount; // 方程的数量N
    cin >> equationCount;

    // 使用vector容器存储所有的方程对象
    vector<Equation> equations(equationCount);
    for (int i = 0; i < equationCount; ++i) {
        cin >> equations[i].A >> equations[i].B >> equations[i].C;
    }

    // 外层循环遍历第 i 个方程 (i从0到N-2)
    // 这个循环控制输出的行数，总共输出 N-1 行
    for (int i = 0; i < equationCount - 1; ++i) {
        // 内层循环遍历第 j 个方程，其中 j > i
        // 这个循环负责计算第 i 个方程与它后面所有方程的交点
        for (int j = i + 1; j < equationCount; ++j) {
            // 从vector中获取两个待求解方程的系数
            // 使用long long类型确保中间计算过程不会发生整数溢出，这是一个良好的编程习惯
            long long A1 = equations[i].A;
            long long B1 = equations[i].B;
            long long C1 = equations[i].C;

            long long A2 = equations[j].A;
            long long B2 = equations[j].B;
            long long C2 = equations[j].C;

            // 根据公式计算行列式，即交点坐标公式中的分母
            long long determinant = A1 * B2 - A2 * B1;

            // 根据公式计算x坐标的分子
            long long numeratorX = B1 * C2 - B2 * C1;

            // 根据公式计算y坐标的分子
            long long numeratorY = C1 * A2 - C2 * A1;

            // 通过整型除法直接得到坐标的整数部分，结果会自动向零取整
            long long intersectionX = numeratorX / determinant;
            long long intersectionY = numeratorY / determinant;

            // 输出交点坐标
            cout << intersectionX << " " << intersectionY;

            // 控制输出格式：如果当前不是该行最后一组坐标，则在其后输出一个空格
            if (j < equationCount - 1) {
                cout << " ";
            }
        }
        // 第 i 个方程与它之后所有方程的交点均已输出，换行
        cout << endl;
    }

    return 0;
}
