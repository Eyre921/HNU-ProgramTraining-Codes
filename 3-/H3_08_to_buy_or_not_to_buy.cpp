/*
 *
8. 到底买不买
小红想买些珠子做一串自己喜欢的珠串。卖珠子的摊主有很多串五颜六色的珠串，但是不肯把任何一串拆散了卖。于是小红要你帮忙判断一下，某串珠子里是否包含了全部自己想要的珠子？如果是，那么告诉她有多少多余的珠子；如果不是，那么告诉她缺了多少珠子。

为方便起见，我们用[0-9]、[a-z]、[A-Z]范围内的字符来表示颜色。例如在图1中，第3串是小红想做的珠串；那么第1串可以买，因为包含了全部她想要的珠子，还多了8颗不需要的珠子；第2串不能买，因为没有黑色珠子，并且少了一颗红色的珠子。

pimg1030_1.jpg

【输入形式】

每个输入包含1个测试用例。每个测试用例分别在2行中先后给出摊主的珠串和小红想做的珠串，两串都不超过1000个珠子。

【输出形式】

如果可以买，则在一行中输出“Yes”以及有多少多余的珠子；如果不可以买，则在一行中输出“No”以及缺了多少珠子。其间以1个空格分隔。

【样例输入】

ppRYYGrrYBR2258
YrR8RrY

【样例输出】

Yes 8

【样例说明】
【评分标准】
 */
/*
 * @file: To_Buy_or_Not_to_Buy.cpp
 *
 * @brief:
 * 本文件实现了"到底买不买"问题的解决方案。程序通过比较摊主拥有的珠子
 * 和顾客想购买的珠子种类与数量，来判断摊主的珠子是否足够。
 *
 * @core_idea:
 * 核心思路是使用哈希映射（在本例中，用一个频率数组更高效）来统计每种颜色珠子的数量。
 * 1. 使用一个大小为128的整型数组 `shopCounts` 作为频率映射表。数组的下标对应
 * 字符的ASCII码，值对应珠子的数量。这个大小足以覆盖所有可能的珠子字符
 * '[0-9]', '[a-z]', '[A-Z]'。
 * 2. 第一次遍历：遍历摊主的珠串，统计每种颜色珠子的数量，填充 `shopCounts` 数组。
 * 3. 第二次遍历：遍历小红想要的珠串。对于她想要的每一颗珠子：
 * a. 检查 `shopCounts` 中对应的珠子数量是否大于0。
 * b. 如果大于0，说明摊主有这颗珠子，就将 `shopCounts` 中该珠子的数量减1，表示“用掉”一颗。
 * c. 如果等于0，说明摊主没有这颗珠子了，就将 `missingCount` (缺失珠子数) 加1。
 * 4. 遍历结束后，检查 `missingCount` 的值。
 * a. 如果 `missingCount` 为 0，说明所有想要的珠子都能满足。输出 "Yes" 和多余的珠子数。
 * 多余的珠子数就是摊主珠串总长度减去小红珠串总长度。
 * b. 如果 `missingCount` 大于 0，说明珠子不够。输出 "No" 和 `missingCount` 的值。
 *
 * @time_complexity: O(N + M)
 * 其中 N 是摊主珠串的长度，M 是小红珠串的长度。算法需要分别完整遍历两个字符串一次，
 * 因此时间复杂度是线性的。
 *
 * @space_complexity: O(C)
 * C 是字符集的大小。在这里我们使用了一个大小固定的数组 (128) 来存储频率，
 * 其空间占用不随输入字符串的长度变化，因此空间复杂度是常数级的，即 O(1)。
 */

#include <iostream>
#include <string>
#include <vector>

// 为了代码简洁，直接使用 std 命名空间
using namespace std;

int main() {
    // 提高C++输入输出流的效率，在处理大量IO时有益
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string shopBeads, desiredBeads;
    // 从标准输入读取摊主的珠串和小红想要的珠串
    cin >> shopBeads >> desiredBeads;

    // 使用一个定长数组作为频率哈希表，存储摊主拥有的每种珠子的数量。
    // 数组大小128足以覆盖所有ASCII码从0到127的字符。
    // vector在此处比原生数组更具C++风格，且能方便地初始化所有计数为0。
    vector<int> shopCounts(128, 0);

    // 第一次遍历：统计摊主珠串中每种珠子的可用数量
    for (char bead : shopBeads) {
        shopCounts[bead]++;
    }

    int missingCount = 0; // 初始化缺失的珠子数量为0

    // 第二次遍历：检查小红想要的珠子是否都能从摊主处获得
    for (char bead : desiredBeads) {
        // 检查摊主是否有当前这颗珠子
        if (shopCounts[bead] > 0) {
            // 如果有，就消耗一颗，即将该珠子的可用数量减1
            shopCounts[bead]--;
        } else {
            // 如果摊主没有这种颜色的珠子了，则缺失数量加一
            missingCount++;
        }
    }

    // 根据是否存在缺失的珠子来决定最终输出
    if (missingCount == 0) {
        // 如果没有缺失的珠子，说明可以买
        // 多余的珠子数量 = 摊主原有的总珠子数 - 小红想要的总珠子数
        int extraCount = shopBeads.length() - desiredBeads.length();
        cout << "Yes " << extraCount << endl;
    } else {
        // 如果有缺失的珠子，说明不能买，并输出缺少的珠子总数
        cout << "No " << missingCount << endl;
    }

    return 0;
}

/*
 * ========================================
 * 测试用例与执行分析
 * ========================================
 *
 * 【样例输入】
 * ppRYYGrrYBR2258
 * YrR8RrY
 *
 * 【代码执行流程】
 * 1. 读取 shopBeads = "ppRYYGrrYBR2258", desiredBeads = "YrR8RrY"。
 * 2. 初始化 `shopCounts` 数组 (大小为128，所有元素为0)。
 * 3. 遍历 `shopBeads` 以填充 `shopCounts`:
 * - shopCounts['p'] = 2
 * - shopCounts['R'] = 2
 * - shopCounts['Y'] = 3
 * - shopCounts['G'] = 1
 * - shopCounts['r'] = 2
 * - shopCounts['B'] = 1
 * - shopCounts['2'] = 2
 * - shopCounts['5'] = 1
 * - shopCounts['8'] = 1
 * (其余字符计数仍为0)
 * 4. 初始化 `missingCount = 0`。
 * 5. 遍历 `desiredBeads` ("YrR8RrY") 并更新 `shopCounts`:
 * - 珠子 'Y': shopCounts['Y'] (3 > 0) -> 变为 2。
 * - 珠子 'r': shopCounts['r'] (2 > 0) -> 变为 1。
 * - 珠子 'R': shopCounts['R'] (2 > 0) -> 变为 1。
 * - 珠子 '8': shopCounts['8'] (1 > 0) -> 变为 0。
 * - 珠子 'R': shopCounts['R'] (1 > 0) -> 变为 0。
 * - 珠子 'r': shopCounts['r'] (1 > 0) -> 变为 0。
 * - 珠子 'Y': shopCounts['Y'] (2 > 0) -> 变为 1。
 * 6. `desiredBeads` 遍历结束，`missingCount` 始终为 0。
 * 7. 判断 `missingCount` 为 0，程序进入 "Yes" 分支。
 * 8. 计算 extraCount = shopBeads.length() (15) - desiredBeads.length() (7) = 8。
 * 9. 输出 "Yes 8"。
 *
 * 【最终输出】
 * Yes 8
 */