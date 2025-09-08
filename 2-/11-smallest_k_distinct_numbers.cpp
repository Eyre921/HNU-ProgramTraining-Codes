/*11. 最小的K个数
【问题描述】

输入n个整数，找出其中最小的k（k<=n）个不同数。例如输入4,5,1,6,1,7,3,8这8个数字，则最小的4个数字是1,3,4,5。

【输入形式】

每个测试案例包括2行：

第一行为2个整数n，k(1<=n，k<=200000)，表示数组的长度。

第二行包含n个整数，表示这n个数，数组中的数的范围是[0,1000 000 000]。

【输出形式】

对应每个测试案例，输出最小的k个数，并按从小到大顺序打印(如果不存在k个不同的数，则按照实际数量进行输出)。

【样例输入】

8 4
4 5 1 6 2 7 3 8
【样例输出】

1 2 3 4
【训练提示】

1、数的范围从0到1000000000，使用数组记录那些数出现过就不是太合适

2、需要去除重复的数，需要从小到大排序----set就是一个不错的选择
*/
/**
 * @file smallest_k_distinct_numbers.cpp
 * @brief 解决方案：找出n个整数中最小的k个不同数
 *
 * 核心思路:
 * 题目要求找出最小的k个“不同”的数，并按升序输出。这与C++ STL中 `std::set`
 * 数据结构的特性完美契合。
 *
 * 1.  `std::set` 的特性：
 * -   自动去重：`set` 内部存储的元素都是唯一的。当我们向 `set` 中插入一个
 * 已经存在的元素时，插入操作会被忽略，从而自然地实现了去重的需求。
 * -   自动排序：`set` 内部是基于红黑树实现的，元素在插入时会自动按照从小到大
 * 的顺序进行排序。这省去了我们手动排序的步骤。
 *
 * 2.  算法流程：
 * a.  读取输入的整数 n 和 k。
 * b.  创建一个 `std::set<int>` 容器。
 * c.  循环 n 次，读取 n 个整数，并将它们逐个插入到 `set` 中。
 * d.  当所有整数都插入完毕后，`set` 中就包含了所有输入数据中的不重复元素，
 * 并且它们是自动排好序的。
 * e.  从 `set` 的开头开始遍历，依次输出前 k 个元素。需要注意，如果 `set`
 * 中的元素总数少于 k，则只需输出 `set` 中的所有元素。
 *
 * 时间复杂度: O(N log N)
 * 向 `set` 中插入 N 个元素，每次插入操作的时间复杂度为 O(log M)，其中 M 是 `set`
 * 中当前的元素数量 (M <= N)。因此，总的时间复杂度为 O(N log N)。
 * 遍历输出 k 个元素的时间复杂度为 O(k)。总体复杂度由插入操作决定。
 *
 * 空间复杂度: O(U)
 * 其中 U 是输入数据中不同整数的数量 (U <= N)。在最坏情况下，所有 N 个数都不同，
 * 空间复杂度为 O(N)。
 */

#include <iostream> // 用于标准输入输出 (cin, cout)
#include <set>      // 用于使用set容器
#include <vector>   // 用于演示另一种解法，但本解法主要用set
#include <algorithm>// 用于使用std::min

// 使用标准命名空间，简化代码
using namespace std;

void solve() {
    int n, k;
    cin >> n >> k;

    // 使用set来自动去重和排序
    set<int> distinctNumbers;

    // 读取n个整数并插入set
    for (int i = 0; i < n; ++i) {
        int num;
        cin >> num;
        distinctNumbers.insert(num);
    }

    // 确定需要输出的元素个数
    // 如果set中的元素总数小于k，则全部输出
    int countToPrint = min(k, (int)distinctNumbers.size());

    // 使用迭代器遍历set并输出前countToPrint个元素
    auto it = distinctNumbers.begin();
    for (int i = 0; i < countToPrint; ++i) {
        cout << *it;
        // 控制输出格式，最后一个数字后没有空格
        if (i < countToPrint - 1) {
            cout << " ";
        }
        ++it; // 移动迭代器到下一个元素
    }
    cout << endl;
}

int main() {
    // 优化C++标准流的IO性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 调用解决问题的函数
    // 题目描述暗示只有一个测试用例，如果需要处理多个，可以放在while循环中
    solve();

    // 样例测试
    // 【样例输入】
    // 8 4
    // 4 5 1 6 2 7 3 8
    // 【预期输出】
    // 1 2 3 4

    return 0;
}