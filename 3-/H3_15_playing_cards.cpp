/*
15. 打牌
【问题描述】

牌只有1到9，手里拿着已经排好序的牌a，对方出牌b，用程序判断手中牌是否能够压过对方出牌。
规则：出牌牌型有5种
[1]一张 如4 则5…9可压过
[2]两张 如44 则55，66，77，…，99可压过
[3]三张 如444 规则如[2]
[4]四张 如4444 规则如[2]
[5]五张 牌型只有12345 23456 34567 45678 56789五个，后面的比前面的均大。

【输入形式】

输入有多行，第一行代表手中的牌，长度不超过200个数字。接下来的每一行代表每次对方出的牌。

【输出形式】

输出有多行，代表手中的牌是否能压过对方出的牌，压过输出YES， 并列出所有可选项，可选项之间用空格分隔。 否则输出NO。

【样例输入】

17624234556367
33
222
34567
【样例输出】

YES 44 55 66 77
YES 666
NO
 */
/**
 * @file Playing_Cards.cpp
 * @brief 核心思路:
 * 本程序旨在解决一个特定的牌类游戏判断问题。程序遵循以下核心逻辑：
 * 1.  **手牌预处理:** 首先，读取代表玩家手牌的字符串。虽然题目描述手牌“已排好序”，但从样例来看，输入字符串并非有序。
 * 因此，我们采用更健壮的方式：将手牌字符串解析为一个频率数组（大小为10），`cardCounts[i]` 记录了数字牌 `i` 的数量。
 * 这种 O(L) (L为手牌字符串长度) 的预处理，使得后续查询任意牌的数量时达到 O(1) 的效率。
 * 2.  **牌型分析与匹配:** 程序进入一个循环，持续读取并处理对方打出的牌。对于每一手牌，首先分析其牌型和大小：
 * - **牌型判定:** 通过牌的长度和内容，判定是“N张相同”（单张、对子、三条、四条）还是“五张顺子”。
 * - **查找更优解:**
 * - **对于“N张相同”:** 如对方出 `NNN`，牌值为 `V`。我们从 `V+1` 到 `9` 遍历，检查我们的频率数组中是否有 `cardCounts[i] >= N`。
 * 所有满足条件的牌 `i` 都能构成一套更大的牌。
 * - **对于“五张顺子”:** 顺子是固定的五种。我们先确定对方顺子的级别（例如 `34567` 是第3级别），然后从更高级别的顺子开始，
 * 逐一检查我们手中的牌是否足够凑成该顺子（即所需五张牌的频率是否都大于等于1）。
 * 3.  **结果输出:** 将所有找到的可行解存储在一个 `std::vector` 中。循环结束后，如果vector为空，则输出 "NO"；否则，输出 "YES"，
 * 并依次打印所有可行的出牌组合。
 *
 * 这种“频率表 + 分类讨论”的策略，逻辑清晰，且充分利用了数据结构的优势，保证了算法的高效性。
 *
 * @time_complexity O(L + M)，其中L是手牌字符串的长度，M是对方出牌的次数。手牌预处理为O(L)，
 * 之后每次应对对方出牌的计算量是一个非常小的常数（最多遍历9种牌值或5种顺子），可以视为O(1)。
 * @space_complexity O(1)，我们仅使用了固定大小的数组和容器来存储牌的频率和结果，空间开销是常数级别。
 */
#include <iostream>
#include <string>
#include <vector>


// 使用标准命名空间，以符合题目要求简化代码
using namespace std;

// 检查一手牌是否是顺子 (例如 "12345")
bool isStraight(const string& play) {
    if (play.length() != 5) {
        return false;
    }
    for (size_t i = 0; i < play.length() - 1; ++i) {
        if (play[i+1] != play[i] + 1) {
            return false;
        }
    }
    return true;
}

int main() {
    // 优化C++标准流的I/O性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // --- 1. 手牌预处理 ---
    string myHandStr;
    cin >> myHandStr;

    // 使用大小为10的数组作为频率表，cardCounts[i] 存储牌 'i' 的数量。
    // 这比 std::map 或 std::unordered_map 更高效。
     size_t cardCounts[10] = {0};
    for (char cardChar : myHandStr) {
        cardCounts[cardChar - '0']++;
    }

    // --- 2. 循环处理对方的出牌 ---
    string opponentPlay;
    while (cin >> opponentPlay) {
        vector<string> winningPlays; // 存储所有能赢的出牌方案

        // --- 3. 分析牌型并查找解 ---
        size_t n = opponentPlay.length();
        char opponentCardValueChar = opponentPlay[0];

        // 情况 A: 对方出的是顺子
        if (isStraight(opponentPlay)) {
            // 定义所有可能的顺子
            const vector<string> allStraights = {"12345", "23456", "34567", "45678", "56789"};
            // 找到对方顺子的级别
             size_t opponentStraightRank = opponentCardValueChar - '1';

            // 查找比对方更大的顺子
            for (size_t i = opponentStraightRank + 1; i < allStraights.size(); ++i) {
                const auto& currentStraight = allStraights[i];
                bool canForm = true;
                // 检查是否有足够的牌来组成这个顺子
                for (char cardNeeded : currentStraight) {
                    if (cardCounts[cardNeeded - '0'] == 0) {
                        canForm = false;
                        break;
                    }
                }
                if (canForm) {
                    winningPlays.push_back(currentStraight);
                }
            }
        }
        // 情况 B: 对方出的是 N 张相同的牌
        else if (n >= 1 && n <= 4) {
             size_t opponentCardValue = opponentCardValueChar - '0';
            // 从比对方大的牌开始查找
            for (size_t card = opponentCardValue + 1; card <= 9; ++card) {
                // 检查我们是否有 N 张或更多这张牌
                if (cardCounts[card] >= n) {
                    winningPlays.push_back(string(n, card + '0'));
                }
            }
        }

        // --- 4. 输出结果 ---
        if (winningPlays.empty()) {
            cout << "NO" << endl;
        } else {
            cout << "YES";
            for (const string& play : winningPlays) {
                cout << " " << play;
            }
            cout << endl;
        }
    }

    return 0;
}