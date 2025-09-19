/*1. 凯蒂的歌
【问题描述】

  音乐是变化与不变的艺术。凯蒂认为，相邻两个音符间的和谐值为它们音调之差的绝对值，一首歌的和谐值为所有相邻音符和谐值的和。

  现在，凯蒂已经写好了一首歌。这首歌由 n 个音符组成，第 i 个音符有音调 ai。他希望再完善一下这首歌，具体而言，他需要修改一个音符的音调，使其音调的值提高 k 或降低 k 。

  凯蒂想知道，在进行完善后，这首歌和谐值的最大值和最小值是多少。


【输入形式】

  第 1 行包含 2 个正整数 n、k，含义如题。

  接下来 1 行，每行包含 n 个整数，第 i 个数表示第 i 个音符的音调 ai。

  对于100%的数据，保证1 <= n、k <= 1,000, | ai |  <= 1,000

【输出形式】

  输出 2 个整数，分别表示这首歌和谐值的最大值和最小值。

【样例输入】


3 5
4 1 6
【样例输出】

18 2
【样例说明】

【评分标准】

【出题人】

拔尖班2022级张世东


*/
/*
 * 核心思路:
 * 本题要求在对单个音符进行一次修改（增加或减少k）后，计算整首歌和谐值的最大和最小值。
 * 1.  **基准计算**: 首先，我们计算出未经任何修改的原始歌曲的和谐值。这个值将作为后续计算的基准。
 *
 * 2.  **影响分析**: 当我们修改第 `i` 个音符的音调时，只有与这个音符直接相关的和谐值部分会发生变化。具体来说：
 * - `|a[i] - a[i-1]|` (如果 `i > 0`)
 * - `|a[i+1] - a[i]|` (如果 `i < n-1`)
 * 歌曲中其他部分的和谐值保持不变。
 *
 * 3.  **增量更新**: 为了高效地计算每次修改后的新和谐值，我们采用增量更新的策略。新和谐值的计算公式为：
 * `新总和谐值 = 原始总和谐值 - 受影响的原始局部和谐值 + 新的局部和谐值`
 * 这样，我们无需在每次修改后都重新计算整个歌曲的和谐值，大大提高了效率。
 *
 * 4.  **遍历所有可能**: 我们遍历歌曲中的每一个音符（从索引0到n-1）。对于每一个音符，我们都尝试两种可能的修改：音调加`k`和音调减`k`。对这 `2*n` 种可能性，我们都计算出其产生的新和谐值，并用它们来实时更新我们所记录的最大值和最小值。
 *
 * 5.  **边界处理**: 特别注意处理第一个音符（只影响 `|a[1]-a[0]|`）和最后一个音符（只影响 `|a[n-1]-a[n-2]|`）的特殊情况。我们的增量更新方法能够自然地处理这些边界情况。
 *
 * 时间复杂度: O(N)，其中 N 是音符的数量。我们首先用 O(N) 的时间计算原始和谐值，然后用一个 O(N) 的循环来遍历所有可能的修改，循环体内的操作是 O(1) 的。
 * 空间复杂度: O(N)，用于存储输入的音符序列。
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <limits>

using namespace std;

// 主函数
int main() {
    // 禁用 C++ 标准流与 C 标准流的同步，提高 IO 效率
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int noteCount;
    long long pitchChangeValue;
    cin >> noteCount >> pitchChangeValue;

    vector<long long> pitches(noteCount);
    for (int i = 0; i < noteCount; ++i) {
        cin >> pitches[i];
    }

    // 边界情况：如果只有一个或没有音符，和谐值为0
    if (noteCount <= 1) {
        cout << 0 << " " << 0 << endl;
        return 0;
    }

    // --- 步骤 1: 计算原始和谐值 ---
    long long originalHarmony = 0;
    for (int i = 0; i < noteCount - 1; ++i) {
        originalHarmony += abs(pitches[i + 1] - pitches[i]);
    }

    // 初始化最终结果的最小值和最大值
    long long minResultHarmony = numeric_limits<long long>::max();
    long long maxResultHarmony = numeric_limits<long long>::min();

    // --- 步骤 2: 遍历每一个音符，尝试所有可能的修改 ---
    for (int i = 0; i < noteCount; ++i) {
        // 计算修改前，与 pitches[i] 相关的局部和谐值
        long long originalLocalHarmony = 0;
        if (i > 0) { // 如果不是第一个音符，则计算与前一个音符的和谐值
            originalLocalHarmony += abs(pitches[i] - pitches[i - 1]);
        }
        if (i < noteCount - 1) { // 如果不是最后一个音符，则计算与后一个音符的和谐值
            originalLocalHarmony += abs(pitches[i + 1] - pitches[i]);
        }

        // --- 可能性 1: 将当前音符的音调增加 k ---
        long long newPitchPlus = pitches[i] + pitchChangeValue;
        long long newLocalHarmonyPlus = 0;
        if (i > 0) {
            newLocalHarmonyPlus += abs(newPitchPlus - pitches[i - 1]);
        }
        if (i < noteCount - 1) {
            newLocalHarmonyPlus += abs(pitches[i + 1] - newPitchPlus);
        }
        // 使用增量更新计算新的总和谐值
        long long currentHarmonyPlus = originalHarmony - originalLocalHarmony + newLocalHarmonyPlus;
        minResultHarmony = min(minResultHarmony, currentHarmonyPlus);
        maxResultHarmony = max(maxResultHarmony, currentHarmonyPlus);

        // --- 可能性 2: 将当前音符的音调减少 k ---
        long long newPitchMinus = pitches[i] - pitchChangeValue;
        long long newLocalHarmonyMinus = 0;
        if (i > 0) {
            newLocalHarmonyMinus += abs(newPitchMinus - pitches[i - 1]);
        }
        if (i < noteCount - 1) {
            newLocalHarmonyMinus += abs(pitches[i + 1] - newPitchMinus);
        }
        // 使用增量更新计算新的总和谐值
        long long currentHarmonyMinus = originalHarmony - originalLocalHarmony + newLocalHarmonyMinus;
        minResultHarmony = min(minResultHarmony, currentHarmonyMinus);
        maxResultHarmony = max(maxResultHarmony, currentHarmonyMinus);
    }

    // 输出最终结果
    cout << maxResultHarmony << " " << minResultHarmony << endl;

    return 0;
}
