/*
 *
* 4. 亲和数

【问题描述】

古希腊数学家毕达哥拉斯在自然数研究中发现，220 的所有真约数(即不是自身的约数)之和为：   

                              1+2+4+5+10+11+20+22+44+55+110＝284。

而 284 的所有真约数为 1、2、4、71、 142，加起来恰好为 220。人们对这样的数感到很惊奇，并称之为亲和数。一般地讲，如果两个数中任何一个数都是另一个数的真约数之和，则这两个数就是亲和数。

你的任务就编写一个程序，判断给定的两个数是否是亲和数。

【输入形式】

输入若干行数据（大于0），每行一个实例,包含两个整数A,B； 其中 0 <= A,B <= 600000 ;

【输出形式】

对于每个测试实例，如果 A 和 B 是亲和数的话输出 YES，否则输出 NO

【样例输入】

220 284

100 200

【样例输出】

YES

NO
 */
/**
 * @file amicable_numbers.cpp
 * @brief 解决方案：判断两个数是否为亲和数
 *
 * 核心思路:
 * 1.  亲和数的定义是：如果正整数A的真约数之和等于B，并且正整数B的真约数之和等于A，
 * 且A不等于B，那么A和B就是一对亲和数。
 *
 * 2.  关键在于高效地计算一个数的“真约数之和”。我们可以编写一个辅助函数
 * `getSumOfProperDivisors(int n)` 来完成这个任务。
 *
 * 3.  `getSumOfProperDivisors` 函数的实现逻辑：
 * a. 一个数的真约数之和等于其所有约数之和减去它本身。
 * b. 为了找到所有约数，我们不需要从1遍历到n。一个更高效的方法是遍历从2到`sqrt(n)`。
 * c. 初始化约数和`sum`为1（因为1是所有大于1的数的约数）。
 * d. 遍历`i`从2到`sqrt(n)`，如果`i`能整除`n`（即`n % i == 0`），那么`i`和`n/i`都是`n`的约数。
 * e. 特殊情况：如果`n`是一个完全平方数，那么在`i = sqrt(n)`时，`i`和`n/i`是同一个数，
 * 我们只加一次即可。
 * f. 遍历结束后，`sum`就是所有约数（包括自身）的和。函数返回`sum - n`即为真约数之和。
 *
 * 4.  主函数逻辑：
 * a. 循环读取输入的两个整数A和B。
 * b. 调用辅助函数分别计算出A的真约数之和`sumA`和B的真约数之和`sumB`。
 * c. 根据亲和数的定义，当 `sumA == B` 且 `sumB == A` 且 `A != B` 三个条件同时满足时，
 * 输出"YES"，否则输出"NO"。
 *
 * 时间复杂度: O(sqrt(max(A, B)))
 * 对于每组输入的A和B，主要的时间开销是计算它们的真约数之和。
 * 计算一个数n的真约数之和的时间复杂度为 O(sqrt(n))。因此，处理一组输入的时间复杂度
 * 取决于A和B中较大的那个数。
 *
 * 空间复杂度: O(1)
 * 算法在计算过程中仅使用了有限的几个变量，所需内存空间是固定的，与输入数值的大小无关。
 */

#include <iostream> // 用于标准输入输出 (cin, cout)
#include <cmath>    // 用于使用 sqrt 函数

// 使用标准命名空间，简化代码
using namespace std;

/**
 * @brief 计算一个正整数的真约数之和。
 * @param n 需要计算的整数。
 * @return n的所有真约数（不包括n自身）的和。
 */
int getSumOfProperDivisors(int n) {
    // 根据定义，0和1没有真约数，其和为0。
    if (n <= 1) {
        return 0;
    }

    // 初始化和为1，因为1是所有大于1的整数的真约数。
    int sum = 1;
    // 计算n的平方根，用于优化循环
    int limit = sqrt(n);

    // 从2开始遍历到sqrt(n)
    for (int i = 2; i <= limit; ++i) {
        // 如果i是n的约数
        if (n % i == 0) {
            // 将约数i加入和中
            sum += i;

            // 获取与i配对的另一个约数
            int correspondingDivisor = n / i;

            // 如果配对的约数不等于i本身（即n不是完全平方数的情况），也将其加入和中
            if (correspondingDivisor != i) {
                sum += correspondingDivisor;
            }
        }
    }

    return sum;
}

int main() {
    // 优化C++标准流的IO性能，对于大量输入输出的场景能提升速度
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int numA, numB;

    // 循环读取每行的一对整数，直到输入结束
    while (cin >> numA >> numB) {
        // 分别计算两个数的真约数之和
        int sumA = getSumOfProperDivisors(numA);
        int sumB = getSumOfProperDivisors(numB);

        // 检查是否满足亲和数的三个条件：
        // 1. A的真约数之和等于B
        // 2. B的真约数之和等于A
        // 3. A和B不相等
        if (sumA == numB && sumB == numA && numA != numB) {
            cout << "YES" << endl;
        } else {
            cout << "NO" << endl;
        }
    }

    // 样例测试
    // 【样例输入】
    // 220 284
    // 100 200
    // 【预期输出】
    // YES
    // NO

    return 0;
}