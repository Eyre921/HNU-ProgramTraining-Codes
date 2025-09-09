/*
* 11. 数码管
【问题描述】

液晶数码管用七笔阿拉数字表示的十个数字，把横和竖的一 个短划都称为一笔，即７有３笔，８有７笔等。对于十个数字一种排列，要做到两相邻数字都可以由另一个数字加上几笔或减去几笔组成，但不能又加又减。比如 ７→３是允许的，７→２不允许。任意输入一组数，判断是否符合上述规则，注意，1在右边。

【输入形式】

每行输入一个0~9的排列，数字之间用空格分隔，以-1作为输入结束

【输出形式】

输出YES或NO

【样例输入】

4 1 0 7 3 9 5 6 8 2
3 5 1 6 2 7 9 0 4 8
-1
【样例输出】

YES
NO

 */
/*
 * @file: Digital_Tube.cpp
 *
 * @brief:
 * 本文件实现了"数码管"问题的解决方案。程序能够判断一个给定的0-9数字排列
 * 是否满足一个特殊的变换规则：任意相邻的两个数字，其七段数码管的字形
 * 变换只能是单纯的增加笔划或减少笔划，不能同时存在增加和减少的笔划。
 *
 * @core_idea:
 * 核心思路是使用“位掩码（Bitmask）”来表示每个数字的七段数码管笔划，并通过
 * 高效的位运算来判断变换规则。
 * 1.  **笔划表示**: 为七段数码管的七个段（a-g）分别指定一个二进制位（0-6）。
 * 一个数字的字形就由一个7位的整数（掩码）表示，其中亮起的段对应的位为1，否则为0。
 * 例如，数字'1'由b,c两段构成，其掩码为 0b0000110。
 * 2.  **规则判断**: "不能又加又减"的规则，在集合论上等价于两个相邻数字A和B的
 * 笔划集合必须是包含关系，即A的笔划集合是B的子集（增笔划），或者B是A的子集（减笔划）。
 * - 设A和B的位掩码为 maskA 和 maskB。
 * - A的笔划是B的子集，用位运算表示为: (maskA & maskB) == maskA。
 * - B的笔划是A的子集，用位运算表示为: (maskA & maskB) == maskB。
 * - 因此，只要满足 `((maskA & maskB) == maskA) || ((maskA & maskB) == maskB)`
 * 之一，该步变换就是合法的。
 * 3.  **实现**:
 * a. 预先计算并存储0-9这十个数字的位掩码到一个常量数组中，便于快速查找。
 * b. 对输入的每一行数字排列，依次检查每对相邻的数字。
 * c. 从数组中取出对应的掩码，进行位运算判断。
 * d. 如果所有相邻对都满足规则，则该排列有效，输出YES；否则，只要有一对不满足，
 * 立即判定为NO，并可提前终止检查。
 *
 * @time_complexity: O(L * K)
 * 其中 L 是输入的总行数，K 是每行排列的数字个数 (在此题中 K=10)。
 * 对每一行，我们最多做 K-1 次常数时间的位运算检查。因此，处理每行的时间复杂度为 O(K)。
 *
 * @space_complexity: O(K)
 * 需要一个大小约为 K 的向量来存储每行的数字排列。存储掩码的数组是常数空间。
 */

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

// 为了代码简洁，直接使用 std 命名空间
using namespace std;

// 使用常量数组存储0-9数字的七段数码管位掩码
// 笔划段与位的对应关系 (abcdefg -> bit 0..6):
//    --0--
//   |     |
//   5     1
//   |     |
//    --6--
//   |     |
//   4     2
//   |     |
//    --3--
const int SEGMENT_MASKS[10] = {
    0x3F, // 0: a,b,c,d,e,f
    0x06, // 1: b,c
    0x5B, // 2: a,b,d,e,g
    0x4F, // 3: a,b,c,d,g
    0x66, // 4: b,c,f,g
    0x6D, // 5: a,c,d,f,g
    0x7D, // 6: a,c,d,e,f,g
    0x07, // 7: a,b,c
    0x7F, // 8: a,b,c,d,e,f,g
    0x6F  // 9: a,b,c,d,f,g
};

int main() {
    // 提高C++标准输入输出流的效率
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string line;
    // 循环读取输入的每一行
    while (getline(cin, line)) {
        // 使用字符串流来方便地解析以空格分隔的数字
        stringstream ss(line);

        int firstNum;
        ss >> firstNum;

        // 检查输入结束标志
        if (firstNum == -1) {
            break;
        }

        // 将一行中的所有数字读入一个向量中
        vector<int> permutation;
        permutation.push_back(firstNum);
        int currentNum;
        while (ss >> currentNum) {
            permutation.push_back(currentNum);
        }

        // 标志位，用于记录当前排列是否有效，默认为有效
        bool isPermutationValid = true;

        // 遍历排列中的每一对相邻数字
        // 循环到 size() - 1，因为每个元素需要和它的下一个元素比较
        for (size_t i = 0; i < permutation.size() - 1; ++i) {
            int digitA = permutation[i];
            int digitB = permutation[i+1];

            // 从常量数组中获取两个数字对应的位掩码
            int maskA = SEGMENT_MASKS[digitA];
            int maskB = SEGMENT_MASKS[digitB];

            // 核心判断逻辑：检查一个数字的笔划集合是否是另一个的子集
            // 如果二者AND运算的结果不等于其中任意一个，则说明发生了“又加又减”
            bool isTransformationValid = ((maskA & maskB) == maskA) || ((maskA & maskB) == maskB);

            if (!isTransformationValid) {
                isPermutationValid = false;
                break; // 若有一对不满足，则整个排列无效，无需继续检查
            }
        }

        // 根据标志位输出最终结果
        if (isPermutationValid) {
            cout << "YES" << endl;
        } else {
            cout << "NO" << endl;
        }
    }

    return 0;
}

/*
 * ========================================
 * 测试用例与执行分析
 * ========================================
 *
 * 【样例输入】
 * 4 1 0 7 3 9 5 6 8 2
 *
 * 【代码执行流程】
 * 1. 读入第一行，存入向量 {4, 1, 0, 7, 3, 9, 5, 6, 8, 2}。
 * 2. 检查 4 -> 1:
 * mask(4) = 0x66 (0b1100110), mask(1) = 0x06 (0b0000110)
 * (0x66 & 0x06) = 0x06, 等于 mask(1)。合法（减笔划）。
 * 3. 检查 1 -> 0:
 * mask(1) = 0x06, mask(0) = 0x3F (0b0111111)
 * (0x06 & 0x3F) = 0x06, 等于 mask(1)。合法（加笔划）。
 * 4. ...以此类推，检查所有相邻对...
 * 5. 检查 8 -> 2:
 * mask(8) = 0x7F (0b1111111), mask(2) = 0x5B (0b1011011)
 * (0x7F & 0x5B) = 0x5B, 等于 mask(2)。合法（减笔划）。
 * 6. 所有对都合法，isPermutationValid 保持 true。输出 "YES"。
 *
 *
 * 【样例输入】
 * 3 5 1 6 2 7 9 0 4 8
 *
 * 【代码执行流程】
 * 1. 读入第二行，存入向量 {3, 5, ...}。
 * 2. 检查 3 -> 5:
 * mask(3) = 0x4F (0b1001111), 代表笔划 {a,b,c,d,g}
 * mask(5) = 0x6D (0b1101101), 代表笔划 {a,c,d,f,g}
 * (0x4F & 0x6D) = 0b1001101
 * 该结果既不等于 0x4F 也不等于 0x6D。变换非法。
 * (具体分析：从3到5，笔划'b'消失了，同时笔划'f'出现了，是“又加又减”)
 * 3. isPermutationValid 被设为 false，循环 break。
 * 4. 输出 "NO"。
 */