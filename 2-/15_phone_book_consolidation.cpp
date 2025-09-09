/*
*15. 电话号码
【问题描述】

Vasya有几本电话簿，记录了他的朋友们的电话号码，每一个朋友都可以有一或几个电话号码。

Vasya决定整理关于朋友电话号码的信息。给定n个字符串，来自于Vasya的电话簿中的条目。每一条都以朋友的姓名开头，然后跟着当前条目中的电话号码个数，然后是本人的电话号码。有可能几个相同的电话被记录在同一个记录中。

Vasya还认为，如果电话号码a是电话号码b的后缀（也就是说，号码b以a结尾），这两个号码被当作同一个电话号码，那么a被认为是无城市代码，它不应该被考虑。

输出整理后Vasya朋友的电话号码信息。有可能两个不同的人有相同的号码。如果一个人有两个电话号码x和y，x是y的后缀（即y以x结尾），则不输出x。

如果Vasya的电话簿中的某些朋友记录了几次，那么只需要记录一次。

【输入形式】

输入第一行一个整数n(1<=n<=20)，Vasya的电话簿上的条目数。

以下n行后面是描述中的格式记录。 朋友的姓名中不包含空字符，长度不超过10位，由小写英文字母组成。电话号码个数在1~10之间。每个电话号码的长度范围在1~10之间，可以包含前导0。

【输出形式】

输出Vasya的朋友的电话号码的有序信息。首先输出电话簿中的朋友数目m。

接下来的m行，包含以格式“姓名 电话号码个数 电话号码1 ... 电话号码k"的条目，号码间以空格分隔。每个记录包含当前朋友的所有电话号码。

每个条目输出按照姓名字母序进行排序，电话号码按照从小到大的顺序排列（注意电话号码比较规则："1"<"01"、"12"<"012"，依此类推）

【样例输入】

4
ivan 3 123 123 456
ivan 2 456 456
ivan 8 789 3 23 6 56 9 89 2
dasha 2 23 789

【样例输出】

2
dasha 2 23 789
ivan 4 2 123 456 789

【样例说明】
【评分标准】
*/

/**
 * @file phone_book_consolidation_optimized.cpp
 * @brief 解决方案：整理并去重电话簿条目（优化版）
 *
 * 核心思路:
 * 1.  数据聚合：与前一版本类似，我们首先将同一个人的所有电话号码汇总到一起。
 * `std::map<string, vector<string>>` 是一个理想的数据结构。
 * -   `map` 的键 `string` 存储朋友的姓名，利用其自动按键排序的特性满足输出要求。
 * -   `map` 的值 `vector<string>` 存储该朋友的所有电话号码（暂时包含重复和冗余）。
 *
 * 2.  后缀过滤（优化算法）:
 * -   为了高效地找出并剔除作为其他号码后缀的“短号码”，我们采用一个非常巧妙的排序策略。
 * a.  首先，对每个人的号码列表进行初步的排序和去重 (`sort` + `unique`)。
 * b.  接着，将去重后的号码列表按照“长度降序，长度相同时按字典序升序”的规则重新排序。
 * 这样做的好处是，任何可能成为后缀的“短号码”都会出现在所有可能包含它的“长号码”之后。
 * c.  创建一个空的 `keptNumbers` 列表。遍历长度排序后的号码列表，对于每个号码 `s`，
 * 检查它是否是 `keptNumbers` 中任何一个已存号码的后缀。因为 `keptNumbers` 中的
 * 号码都比 `s` 长，所以 `s` 不可能是它们的后缀，只可能是 `s` 是某个长号码的后缀。
 * 如果 `s` 不是任何已保留号码的后缀，则将 `s` 加入 `keptNumbers`。
 * d.  这个过程结束后，`keptNumbers` 中就是所有非冗余的号码。
 * e.  最后，将 `keptNumbers` 列表按照题目要求的输出顺序（长度升序，长度相同时按字典序升序）
 * 进行最终排序。
 *
 * 3.  输出格式化：
 * -   所有朋友的号码都处理完毕后，按格式输出最终结果。
 *
 * 编译警告修复:
 * -   您提供的代码中使用了C++17的“结构化绑定”特性（如 `for (auto& [name, nums] : ...)`），
 * 这在较旧的编译器或未指定C++17标准的模式下会产生警告或错误。
 * -   本代码已将其替换为兼容C++11/14的传统迭代器或键值对(pair)的访问方式
 * （如 `for (auto& pair : ...)`，然后使用 `pair.first` 和 `pair.second`），
 * 从而消除了编译警告。
 *
 * 时间复杂度: O(N*C*L + U * K*logK*L)
 * N: 输入条目数, C: 每个条目的号码数, U: 唯一的朋友数, K: 每个朋友的号码总数, L: 号码平均长度。
 * 主要开销在于对每个朋友的号码列表进行多次排序。
 *
 * 空间复杂度: O(U*K*L)
 * 主要用于存储整个电话簿的数据。
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <functional> // For std::equal

// 使用标准命名空间，简化代码
using namespace std;

/**
 * @brief 对单个朋友的号码集合进行去重、去后缀冗余和最终排序。
 * @param numbers 原始的电话号码列表（可能包含重复）。
 * @return 一个清理和排序好的电话号码列表。
 */
vector<string> filterAndSortNumbers(vector<string>& numbers) {
    // 1. 值去重：先按字典序排序，然后使用 unique-erase 手法
    sort(numbers.begin(), numbers.end());
    numbers.erase(unique(numbers.begin(), numbers.end()), numbers.end());

    // 2. 为便于后缀判断，按长度降序（同长按字典序升序）排序
    sort(numbers.begin(), numbers.end(), [](const string& a, const string& b) {
        if (a.length() != b.length()) {
            return a.length() > b.length(); // 长度长的排在前面
        }
        return a < b; // 长度相同时按字典序
    });

    vector<string> keptNumbers; // 用于存储最终保留的号码

    // 3. 遍历每个号码，若它是某个已保留更长号码的后缀，则丢弃；否则加入保留集
    for (const string& currentNum : numbers) {
        bool isASuffix = false;
        // 只需检查 currentNum 是否是已保留号码的后缀
        // 因为 keptNumbers 中的号码都比 currentNum 长或等长
        for (const string& keptNum : keptNumbers) {
            // 检查 currentNum 是否以 keptNum 结尾
            if (keptNum.length() > currentNum.length() &&
                keptNum.rfind(currentNum) == (keptNum.length() - currentNum.length())) {
                isASuffix = true;
                break;
            }
        }
        if (!isASuffix) {
            keptNumbers.push_back(currentNum);
        }
    }

    // 4. 按最终输出要求排序：长度升序，其次字典序升序
    sort(keptNumbers.begin(), keptNumbers.end(), [](const string& a, const string& b) {
        if (a.length() != b.length()) {
            return a.length() < b.length();
        }
        return a < b;
    });

    return keptNumbers;
}

int main() {
    // 优化C++标准流的IO性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    // Phase 1: 数据聚合
    map<string, vector<string>> phoneBook;
    for (int i = 0; i < n; ++i) {
        string name;
        int count;
        cin >> name >> count;
        for (int j = 0; j < count; ++j) {
            string number;
            cin >> number;
            phoneBook[name].push_back(number);
        }
    }

    // Phase 2: 对每个朋友的号码列表进行过滤和排序
    // 使用 C++11/14 兼容的 map 迭代方式
    for (auto& pair : phoneBook) {
        // pair.second 是与姓名关联的 vector<string>
        pair.second = filterAndSortNumbers(pair.second);
    }

    // Phase 3: 输出结果
    cout << phoneBook.size() << endl;
    // 使用 C++11/14 兼容的 const map 迭代方式
    for (const auto& pair : phoneBook) {
        const string& name = pair.first;
        const vector<string>& numbers = pair.second;

        cout << name << " " << numbers.size();
        for (const string& num : numbers) {
            cout << " " << num;
        }
        cout << endl;
    }

    return 0;
}