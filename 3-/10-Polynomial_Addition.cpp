/*10. 多项式加法
【问题描述】

一个多项式可以表示为一组数对，数对中第一个数始终为整数，且唯一，表示多项式的次数，另一数表示为对应的系数且不为0。输入两组数对，每组以0 0作为结束，实现对两个多项式的加法并按降幂输出结果数对

【输入形式】

每行输入一个数对，以空格为分隔符，以0 0结束

【输出形式】

每行输出一个数对，以空格为分隔符

【样例输入】

5 12
3 8
1 2
15 5
0 10
0 0
3 12
30 1
15 5
0 0
【样例输出】

30 1
15 10
5 12
3 20
1 2
0 10
*/
/**
 * @file Polynomial_Addition.cpp
 * @brief 实现两个多项式的加法运算。
 *
 * 核心思路:
 * 1.  数据结构选择：多项式的核心是“指数 -> 系数”的映射关系，并且最终需要按指数降序输出。
 * `std::map<int, int>` 是理想的数据结构。它的键(key)可以存储指数，值(value)可以存储系数。
 * `std::map`具有两个关键特性：
 * a. 键是唯一的，完美符合题目中“次数唯一”的描述。
 * b. 它会自动根据键（指数）进行升序排序。
 * 2.  多项式相加：我们可以创建一个`std::map`来存储最终结果。然后，依次读入两个多项式的项。
 * 对于读入的每一个项（指数`e`, 系数`c`），我们执行 `resultPoly[e] += c;` 操作。
 * `map`的 `[]` 操作符会自动处理所有情况：
 * - 如果指数`e`已存在，就将新的系数`c`累加到原有系数上。
 * - 如果指数`e`不存在，`map`会创建一个新条目，其系数默认初始化为0，然后再进行累加。
 * 这种方式极其优雅地实现了两个多项式的合并与相加。
 * 3.  按降幂输出：`std::map`内部是按键（指数）升序排列的。为了实现降序输出，我们只需使用反向迭代器
 * (`rbegin()` 到 `rend()`) 遍历`map`即可。
 * 4.  处理零系数：在遍历输出时，需要检查每个项的系数是否为0。如果系数因加法抵消而变为0，则不应输出该项。
 *
 * 时间复杂度: O(K log K)
 * 其中 K = N + M，N和M分别是两个多项式的项数。每次向`map`中插入或更新一个元素的时间复杂度是 O(log S)，
 * S是`map`的当前大小。总的时间复杂度主要由构建`map`决定。
 *
 * 空间复杂度: O(K)
 * 在最坏情况下（两个多项式的指数均不相同），`map`需要存储 K 个项。
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

// 使用标准命名空间，简化代码
using namespace std;

/**
 * @brief 读取一个多项式的所有项并将其累加到给定的map中
 * @param polyMap 用于存储和累加多项式项的map引用
 */
void readAndAddPolynomial(map<int, int>& polyMap) {
    int exponent, coefficient;

    // 持续读取数对，直到遇到结束标志 (0, 0)
    while (cin >> exponent >> coefficient && (exponent != 0 || coefficient != 0)) {
        // 将当前项的系数累加到对应指数的项上
        // 这是本算法的核心，简洁地处理了新项的添加和同类项的合并
        polyMap[exponent] += coefficient;
    }
}

int main() {
    // 加速C++ I/O操作
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 使用 map 存储最终的多项式。键: 指数, 值: 系数
    map<int, int> resultPoly;

    // 读取第一个多项式并合并到 resultPoly 中
    readAndAddPolynomial(resultPoly);

    // 读取第二个多-项式并合并到 resultPoly 中
    readAndAddPolynomial(resultPoly);

    // 按指数降序输出结果
    // 使用反向迭代器 (rbegin, rend) 可以方便地从大到小遍历 map
    for (auto it = resultPoly.rbegin(); it != resultPoly.rend(); ++it) {
        // 如果某项的系数在相加后变为0，则不输出
        if (it->second != 0) {
            // it->first 是键(指数), it->second 是值(系数)
            cout << it->first << " " << it->second << endl;
        }
    }

    return 0;
}