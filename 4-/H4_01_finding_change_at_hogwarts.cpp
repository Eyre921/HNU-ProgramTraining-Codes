/*
1. 在霍格沃茨找零钱
【问题描述】

如果你是哈利·波特迷，你会知道魔法世界有它自己的货币系统 —— 就如海格告诉哈利的：“十七个银西可(Sickle)兑一个加隆(Galleon)，二十九个纳特(Knut)兑一个西可，很容易。”现在，给定哈利应付的价钱P和他实付的钱A，你的任务是写一个程序来计算他应该被找的零钱。

【输入形式】

输入在1行中分别给出P和A，格式为“Galleon.Sickle.Knut”，其间用1个空格分隔。这里Galleon是[0, 107]区间内的整数，Sickle是[0, 17)区间内的整数，Knut是[0, 29)区间内的整数。

【输出形式】

在一行中用与输入同样的格式输出哈利应该被找的零钱。如果他没带够钱，那么输出的应该是负数。

【样例输入1】

10.16.27 14.1.28
【样例输出1】

3.2.1
【样例输入2】

14.1.28 10.16.27
【样例输出1】

-3.2.1
*/
/*
 * 核心思路:
 * 题目的核心是处理一种特殊的进制计算（1 Galleon = 17 Sickles, 1 Sickle = 29 Knuts）。
 * 为了简化加减运算，最直接和健壮的方法是“单位统一化”。
 * 1. 将所有输入的货币（应付价格P和实付金额A）完全转换为最小单位“Knut”。
 * 2. 在最小单位上进行减法运算（实付金额的Knut总数 - 应付价格的Knut总数），得到以Knut为单位的找零差额。
 * 3. 将计算出的Knut差额再转换回“Galleon.Sickle.Knut”的格式进行输出。
 * 4. 特别注意，由于Galleon的最大值可达10^7，转换后的总Knut数会超过32位整型的范围（约2.1*10^9），
 * 因此必须使用64位整型（long long）来存储总Knut数，以防止数据溢出。
 * 5. 如果找零差额为负数，需要先输出一个负号，然后对差额的绝对值进行单位转换。
 *
 * 时间复杂度: O(1)
 * 整个过程只涉及固定次数的读入、乘法、加减法和除法/取模运算，与输入数值的大小无关。
 *
 * 空间复杂度: O(1)
 * 仅使用了有限的几个变量来存储输入的数值和计算结果，不随输入规模变化。
 */

#include <iostream>
#include <cmath> // 用于 std::abs

// 遵循规范，直接使用 std 命名空间
using namespace std;

// 定义进制常量，增强代码的可读性和可维护性
const int SICKLES_PER_GALLEON = 17;
const int KNUTS_PER_SICKLE = 29;
// 预先计算好 Galleon 到 Knut 的转换率，避免在循环或函数中重复计算
const long long KNUTS_PER_GALLEON = SICKLES_PER_GALLEON * KNUTS_PER_SICKLE;

int main() {
    // 对于IO密集型任务，设置cin/cout以提高性能，是一个良好的编程习惯
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 使用清晰、自解释的驼峰命名法定义变量
    long long priceGalleon, priceSickle, priceKnut;
    long long paidGalleon, paidSickle, paidKnut;

    // C-style I/O (scanf) 在处理特定格式的输入时通常比 C++ streams 更简洁高效
    scanf("%lld.%lld.%lld %lld.%lld.%lld", &priceGalleon, &priceSickle, &priceKnut, &paidGalleon, &paidSickle, &paidKnut);

    // 步骤 1: 将应付价格 P 和实付金额 A 全部转换为最小单位 Knut
    // 使用 long long 防止整数溢出
    long long totalPriceInKnuts = priceGalleon * KNUTS_PER_GALLEON + priceSickle * KNUTS_PER_SICKLE + priceKnut;
    long long totalPaidInKnuts = paidGalleon * KNUTS_PER_GALLEON + paidSickle * KNUTS_PER_SICKLE + paidKnut;

    // 步骤 2: 计算以 Knut 为单位的找零差额
    long long changeInKnuts = totalPaidInKnuts - totalPriceInKnuts;

    // 步骤 3: 处理找零为负数的情况
    if (changeInKnuts < 0) {
        cout << "-";
        // 对负数取绝对值，方便后续统一进行单位换算
        changeInKnuts = -changeInKnuts;
    }

    // 步骤 4: 将总 Knut 数转换回 Galleon.Sickle.Knut 的标准格式
    // 首先计算出总共有多少 Galleon
    long long changeGalleon = changeInKnuts / KNUTS_PER_GALLEON;
    // 计算扣除 Galleon 后剩余的 Knut
    long long remainder = changeInKnuts % KNUTS_PER_GALLEON;
    // 在剩余的 Knut 中计算出有多少 Sickle
    long long changeSickle = remainder / KNUTS_PER_SICKLE;
    // 最后剩余的就是 Knut 的数量
    long long changeKnut = remainder % KNUTS_PER_SICKLE;

    // 步骤 5: 按指定格式输出最终结果
    cout << changeGalleon << "." << changeSickle << "." << changeKnut << endl;

    // main 函数的测试用例说明：
    // 本程序设计为从标准输入读取数据，可以直接用于在线评测系统。
    // 以下为手动测试时可以使用的输入及对应的预期输出：
    //
    // --- 测试用例 1 (来自题目样例) ---
    // 输入: 10.16.27 14.1.28
    // 预期输出: 3.2.1
    //
    // --- 测试用例 2 (来自题目样例) ---
    // 输入: 14.1.28 10.16.27
    // 预期输出: -3.2.1
    //
    // --- 测试用例 3 (刚好付清) ---
    // 输入: 50.10.20 50.10.20
    // 预期输出: 0.0.0
    //
    // --- 测试用例 4 (大额交易，测试 long long 的必要性) ---
    // 输入: 10000000.16.28 10000001.0.0
    // 预期输出: 0.0.1

    return 0;
}