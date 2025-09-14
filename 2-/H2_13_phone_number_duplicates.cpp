/*
*13. 487-3279
每个人都喜欢有令人难忘的电话号码。要想让电话号码变得令人难忘的一种方法是拼出一个令人难忘的单词或短语。例如，你可以拨打滑铁卢大学的电话，拨打令人难忘的电话号码TUT-GLOP。

有时只有一部分号码被用来拼写一个单词，例如，你可以拨打310-gino从Gino's订购披萨。

要使电话号码令人难忘的另一种方法是以一种令人难忘的方式对数字进行分组。你可以从比萨饼小屋中订购比萨饼，方法是拨打他们的“3个10”，即号码3-10-10-10。

电话号码的标准格式是七位的十进制数字，第三和第四位之间包含连字符（例如888-1200）。电话的键盘提供字母到数字的映射，如下所示：

A, B, C映射到2

D, E, F映射到3

G, H, I映射到4

J, K, L映射到5

M, N, O映射到6

P, R, S映射到7

T, U, V映射到8

W, X, Y映射到9

Q和Z没有映射。连接符不拨号，必要时可加上或去除。TUT-GLOP的标准格式是888-4567，310-GINO的标准格式是310-4466，3-10-10-10的标准格式是310-1010。

当两个电话号码有相同的标准格式时是等价的（拨同样的号码）。

你的公司正在编制本地企业的电话号码目录，作为质量控制的一部分，你需要检查没有两个（或多个）企业具有相同的电话号码。


【输入形式】

输入包括一个案例。输入的第一行为一个正整数，指定目录中电话号码的数目(最多100，000)。其余的各行列出目录中的电话号码，每个号码单独占一行。每个电话号码都是一个由十进制数字、大写字母(不包括Q和z)和连字符组成的字符串。字符串中的七个字符或是数字或是字母。

【输出形式】

对于出现超过一次的每个号码，按照标准格式及字典序每个输出一行，然后是空格，接着输出出现的次数。只出现1次的电话号码不输出。

【样例输入】

12
4873279
ITS-EASY
888-4567
3-10-10-10
888-GLOP
TUT-GLOP
967-11-11
310-GINO
F101010
888-1200
-4-8-7-3-2-7-9-
487-3279

【样例输出】

310-1010 2
487-3279 4
888-4567 3

【样例说明】
【评分标准】
*/
/**
 * @file phone_number_duplicates.cpp
 * @brief 解决方案：标准化电话号码并找出重复项及其出现次数
 *
 * 核心思路:
 * 1.  标准化：问题的核心是将各种格式的电话号码字符串转换成一个统一的、标准的
 * `XXX-XXXX` 格式。为此，我们创建一个辅助函数 `normalizePhoneNumber`。
 * -   该函数遍历输入字符串，忽略所有连字符'-'。
 * -   遇到数字时，直接采纳。
 * -   遇到字母时，根据电话键盘的映射规则（A,B,C -> 2等）将其转换为对应的数字。
 * -   为了高效地进行字母到数字的映射，可以预先构建一个大小为26的映射数组。
 * -   函数收集处理后的7个数字，然后格式化成 `XXX-XXXX` 的形式。
 *
 * 2.  统计与排序：
 * -   我们需要统计每个标准格式电话号码出现的次数，并按字典序输出结果。
 * -   `std::map<string, int>` 是完成此项任务的完美数据结构。
 * -   它的 `key` (string) 用来存储标准格式的电话号码，`value` (int) 用来计数。
 * -   `std::map` 的一个关键特性是它内部基于红黑树实现，所有键都会自动按字典序
 * (lexicographical order) 排序。这使得我们无需在最后进行额外的排序操作。
 *
 * 3.  算法流程：
 * a.  读取电话号码的总数 N。
 * b.  创建一个 `map<string, int> phoneCounts` 来存储号码及其频率。
 * c.  循环 N 次，每次读取一个原始电话号码字符串。
 * d.  调用 `normalizePhoneNumber` 函数将其转换为标准格式。
 * e.  将标准化后的号码作为键，在 `phoneCounts` map 中对应的计数值加一。
 * f.  所有号码处理完毕后，遍历 `phoneCounts` map。
 * g.  对于每一个键值对，如果计数值（value）大于1，则按格式输出号码（key）和
 * 它的计数值。
 *
 * 时间复杂度: O(N * L * log M)
 * N是电话号码总数，L是号码字符串的平均长度，M是不同标准号码的数量 (M <= N)。
 * 处理每个号码的标准化需要O(L)时间。将其插入或更新到map中需要O(log M)时间。
 *
 * 空间复杂度: O(M * L_std)
 * M是不同标准号码的数量，L_std是标准号码的长度（固定为8）。主要空间开销是map。
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cctype> // For isdigit and isupper

// 使用标准命名空间，简化代码
using namespace std;

// 预先构建字母到数字的映射表，提高转换效率
const int letterMap[] = {
    2, 2, 2,    // A, B, C
    3, 3, 3,    // D, E, F
    4, 4, 4,    // G, H, I
    5, 5, 5,    // J, K, L
    6, 6, 6,    // M, N, O
    7, -1, 7, 7, // P, Q(skip), R, S
    8, 8, 8,    // T, U, V
    9, 9, 9, -1 // W, X, Y, Z(skip)
};

/**
 * @brief 将原始电话号码字符串转换为 XXX-XXXX 的标准格式。
 * @param rawNumber 包含数字、大写字母和连字符的原始字符串。
 * @return 标准格式的电话号码字符串。
 */
string normalizePhoneNumber(const string& rawNumber) {
    string digits;
    digits.reserve(7); // 预分配空间以提高性能

    for (char c : rawNumber) {
        if (digits.length() == 7) break; // 已集齐7位数字

        if (isdigit(c)) {
            digits += c;
        } else if (isupper(c)) {
            int mappedDigit = letterMap[c - 'A'];
            if (mappedDigit != -1) { // 忽略Q和Z
                digits += to_string(mappedDigit);
            }
        }
        // 连字符'-'被自动忽略
    }

    // 插入连字符形成标准格式
    digits.insert(3, "-");
    return digits;
}

int main() {
    // 优化C++标准流的IO性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    // 使用map来自动统计频率并按字典序排序
    map<string, int> phoneCounts;

    for (int i = 0; i < n; ++i) {
        string rawNumber;
        cin >> rawNumber;
        string standardForm = normalizePhoneNumber(rawNumber);
        phoneCounts[standardForm]++;
    }

    // bool foundDuplicates = false;
    // 遍历map，输出所有出现次数大于1的号码
    for (const auto& pair : phoneCounts) {
        if (pair.second > 1) {
            cout << pair.first << " " << pair.second << endl;
            // foundDuplicates = true;
        }
    }

    // 如果没有重复的号码，根据题意不输出任何内容。
    // 如果需要明确提示，可以取消下面代码的注释。
    // if (!foundDuplicates) {
    //     cout << "No duplicates found." << endl;
    // }

    return 0;
}