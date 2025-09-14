/*
19. 相等的多项式
问题描述

小明现在在学习多项式的展开：就是把一个形如

（x+a1） （x+a2） ... （x+an）

展开成如下形式：

xn + b1xn-1 + b2xn-2 + ... + bn-1x + bn

比如 （x+1）（x+2）=x2 + 3x + 2

      (x+1)3 = x3 +3x2 +3x + 1

小明做了很多练习，但是不知道对错，现在请求你的帮助，判断小明的展开式是否正确。



输入格式

有多组测试数据。

每组测试数据有三行，第一行是一个正整数N，表示多项式最高指数。N=0表示输入结束，并且不需要处理。

第二行N个整数ai，用空格隔开，i=1，...,N(-100≤ai≤100)

第三行N个整数bi，用空格隔开，i=1，...,N，(-109≤bi≤109)

40%的测试数据 1 ≤ N < 5；

30%的测试数据 5 ≤ N < 10；

20%的测试数据10 ≤ N < 15；

10%的测试数据 15 ≤N≤ 20；

输出格式

  　　对于每组测试数据，输出一行一个字符‘Y'如果展开式是正确的，输出‘N’如果展开式错误。

样例输入


2
1 2
3 2
3
1 1 1
3 3 1
4
0 0 0 1
0 0 0 1
0


样例输出


Y
Y
N
*/
/*
 * 核心思路:
 * 本题要求验证一个多项式展开式是否正确。
 * (x+a₁)(x+a₂)...(x+aₙ) = xⁿ + b₁xⁿ⁻¹ + ... + bₙ
 *
 * 我们可以通过编程模拟多项式乘法来计算出左侧展开式的真实系数，然后与右侧给定的 bᵢ 系数进行比较。
 *
 * 1. 多项式表示:
 * 一个 N 次多项式可以用一个包含 N+1 个元素的 vector 来表示其系数。例如，
 * x² + 3x + 2 可以表示为 {1, 3, 2}。
 *
 * 2. 迭代乘法:
 * 我们可以从最简单的多项式 P(x) = 1 (系数为 {1}) 开始，然后依次乘以 (x+a₁), (x+a₂), ..., (x+aₙ)。
 *
 * 假设我们已经计算出 P(x) = c₀xᵏ + c₁xᵏ⁻¹ + ... + cₖ 的系数 {c₀, ..., cₖ}，
 * 现在要乘以一个新的项 (x+a)。
 * P(x) * (x+a) = x * P(x) + a * P(x)
 *
 * - x * P(x) 相当于将 P(x) 的所有系数向左移动一位（次数增加1）。
 * - a * P(x) 相当于将 P(x) 的所有系数都乘以 a。
 *
 * 将这两部分相加，可以得到新系数的递推关系。这可以通过一个高效的 in-place 算法实现：
 * - 假设当前系数 vector 为 `coeffs`。
 * - 乘以 (x+a) 时，先在 `coeffs` 末尾增加一个0，使其长度增加1以容纳新的最高次项。
 * - 从后向前遍历 `coeffs`，更新 `coeffs[j] = coeffs[j] + coeffs[j-1] * a`。
 * 这样做可以保证在计算 `coeffs[j]` 时，`coeffs[j-1]` 还是上一轮迭代的值，避免了使用临时数组。
 *
 * 3. 数据类型:
 * N 最大为 20，aᵢ 最大为 100。系数可能会变得非常大，例如 bₙ = a₁*...*aₙ 可能达到 100²⁰，
 * 这会超出标准64位整型 `long long` 的范围。
 * 但是，题目给定的 bᵢ 的范围是 [-10⁹, 10⁹]，这是一个非常重要的提示。这暗示在所有答案为 'Y'
 * 的测试用例中，真实的系数都不会超出这个范围。因此，使用 `long long` 来存储计算过程中的
 * 系数是足够且必要的，可以防止中间过程溢出，并能正确处理在 `long long` 范围内的比较。
 *
 * 时间复杂度: O(N²)
 * 外层循环遍历 N 个 aᵢ，内层循环遍历当前多项式的系数，长度从 1 增长到 N。
 * 总操作次数约为 1+2+...+N = N*(N+1)/2。
 *
 * 空间复杂度: O(N)
 * 主要用于存储多项式的系数。
 */

#include <iostream>
#include <vector>
#include <numeric>

// 遵循规范，直接使用 std 命名空间
using namespace std;

int main() {
    // 提高IO性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    // 循环处理多组测试数据，直到输入 N=0 结束
    while (cin >> n && n != 0) {
        // 读取 a_i 和 b_i
        vector<int> a(n);
        vector<long long> b(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        for (int i = 0; i < n; ++i) cin >> b[i];

        // `calculatedCoeffs` 用于存储 (x+a₁)...(x+aₙ) 展开后的系数
        // calculatedCoeffs[k] 是 x^(n-k) 的系数
        // 初始化为多项式 P(x)=1，其系数为 {1}
        vector<long long> calculatedCoeffs = {1};

        // 迭代地乘以每一个 (x + val_a)
        for (int val_a : a) {
            // 每次乘法，多项式的次数增加1，所以系数向量长度加1
            calculatedCoeffs.push_back(0);

            // 从后向前更新系数，以避免在同一次迭代中使用已经更新过的值
            // 新系数 new_c[j] = 旧系数 old_c[j] * val_a + 旧系数 old_c[j-1]
            for (int j = calculatedCoeffs.size() - 1; j >= 1; --j) {
                calculatedCoeffs[j] += calculatedCoeffs[j - 1] * val_a;
            }
        }

        // 比较计算出的系数和输入的 b_i
        bool isCorrect = true;
        for (int i = 0; i < n; ++i) {
            // calculatedCoeffs[0] 是 x^n 的系数 (恒为1)
            // calculatedCoeffs[i+1] 是 b_{i+1} 对应的系数
            if (calculatedCoeffs[i + 1] != b[i]) {
                isCorrect = false;
                break;
            }
        }

        // 输出结果
        if (isCorrect) {
            cout << 'Y' << endl;
        } else {
            cout << 'N' << endl;
        }
    }

    return 0;
}