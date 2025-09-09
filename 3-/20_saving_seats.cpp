 
/*
20. 占座位
【问题描述】sun所在学校的教室座位每天都是可以预占的。
一个人可以去占多个座位，而且一定是要连续的座位，如果占不到他所要求的这么多座位，那么他就一个座位也不要了。为了降低难度，每次分配座位按座位号从小到大查找，采用最先适配法分配座位。

【输入形式】输入有多组数据。
每组数据输入座位排数n，0<n<=100（座位的排列数相等，座位是按每行从左到右依次排序的, 第1行的最右边一个座位与第二行的第一个座位视为连续座位），m（ 0<m<=min(100,n*n) ）个人。
然后输入k（0<k<=100），最后输入k个命令。
命令只有两种：
1.in id num（代表id,0<=id<m,要占num个座位，若占不到连续的num(0<num<=20)个座位表示该命令无效）
2.out id（代表id要释放他之前占的所有座位）
注意：如果id之前占过座还没释放那么之后他的in命令都是无效的，
如果id之前没占过座位那么他的out命令也是无效的。

【输出形式】对每个命令输出yes或者no，如果命令有效则输出yes，无效则输出no。
在yes no后面只带有回车，不带其他任何字符。

【样例输入】

4 10

9

in 1 7

in 2 3

in 3 3

in 3 3

in 4 3

out 2

in 5 6

out 3

in 5 6

【样例输出】

yes
yes

yes

no

yes

yes

no

yes

yes
 */
/**
 * @file Saving_Seats.cpp
 * @brief 模拟一个基于最先适配法的连续座位预占系统。
 *
 * 核心思路:
 * 1.  数据结构设计：
 * -   座位状态：整个校区的座位（n*n个）被视为一个一维的连续长条。
 * 我们使用一个 `std::vector<int> seats` 来表示，大小为 n*n。
 * `seats[i] = -1` 表示座位为空，`seats[i] = id` 表示该座位被 `id` 号的人占据。
 * -   用户状态：为了高效地处理 `in` 和 `out` 命令的有效性，我们需要跟踪每个人
 * 的预占情况。我们使用两个 `std::vector`：
 * -   `personStartSeat[id]`：记录 `id` 号的人所占座位的起始编号。-1 表示未占座。
 * -   `personSeatCount[id]`：记录 `id` 号的人所占座位的数量。
 * 这两个向量组合起来，可以完整地描述每个人的占座状态，使得 `out` 操作可以快速定位并释放座位。
 *
 * 2.  命令处理逻辑：
 * -   `in id num` 命令：
 * a.  有效性检查：首先检查 `personStartSeat[id]` 是否不为-1。如果是，说明该用户已占座，
 * 此 `in` 命令无效，输出 "no"。
 * b.  座位查找（最先适配法）：从座位编号0开始，遍历整个 `seats` 向量，寻找第一个长度为
 * `num` 的连续空闲座位块（即连续 `num` 个值为-1的元素）。
 * c.  结果处理：如果找到这样的座位块，则命令成功。输出 "yes"，并更新 `seats`、
 * `personStartSeat` 和 `personSeatCount` 这三个状态向量。如果遍历完都找不到，
 * 则命令失败，输出 "no"。
 *
 * -   `out id` 命令：
 * a.  有效性检查：检查 `personStartSeat[id]` 是否为-1。如果是，说明该用户未占座，
 * 此 `out` 命令无效，输出 "no"。
 * b.  座位释放：如果命令有效，输出 "yes"。根据 `personStartSeat[id]` 和
 * `personSeatCount[id]` 找到该用户占据的座位区间，并将 `seats` 向量中对应区间的
 * 值重置为-1。同时，重置该用户的状态向量。
 *
 * 时间复杂度: O(K * N^2 * NUM_MAX)
 * K是命令数，N是排数，NUM_MAX是单次占座的最大数量。对于每个`in`命令，最坏情况下需要遍历
 * 整个座位数组（N*N个座位）来寻找长度为NUM_MAX的连续空位。
 *
 * 空间复杂度: O(N^2 + M)
 * 需要 O(N^2) 空间存储所有座位状态，以及 O(M) 空间存储所有人的状态。
 */

#include <iostream>
#include <vector>
#include <string>
#include <numeric>

// 使用标准命名空间，简化代码
using namespace std;

int main() {
    // 加速C++ I/O操作
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    // 处理多组测试数据
    while (cin >> n >> m) {
        int k;
        cin >> k;

        int totalSeats = n * n;
        // -1 表示座位为空，否则存储占座人的id
        vector<int> seats(totalSeats, -1);
        // 记录每个人占座的起始位置，-1表示未占座
        vector<int> personStartSeat(m, -1);
        // 记录每个人占座的数量
        vector<int> personSeatCount(m, 0);

        for (int i = 0; i < k; ++i) {
            string command;
            cin >> command;

            if (command == "in") {
                int id, num;
                cin >> id >> num;

                // 规则：如果id之前占过座还没释放，那么之后他的in命令都是无效的
                if (personStartSeat[id] != -1) {
                    cout << "no" << endl;
                    continue;
                }

                // 最先适配法查找连续座位
                int startIdx = -1;
                // 遍历所有可能的起始点
                for (int j = 0; j <= totalSeats - num; ++j) {
                    bool isBlockFree = true;
                    // 检查从j开始的num个座位是否都为空
                    for (int l = 0; l < num; ++l) {
                        if (seats[j + l] != -1) {
                            isBlockFree = false;
                            break;
                        }
                    }
                    // 如果找到了一个完整的空闲块
                    if (isBlockFree) {
                        startIdx = j;
                        break;
                    }
                }

                // 根据查找结果进行处理
                if (startIdx != -1) {
                    // 占座成功
                    cout << "yes" << endl;
                    personStartSeat[id] = startIdx;
                    personSeatCount[id] = num;
                    for (int l = 0; l < num; ++l) {
                        seats[startIdx + l] = id;
                    }
                } else {
                    // 占座失败（找不到足够的连续座位）
                    cout << "no" << endl;
                }

            } else { // command == "out"
                int id;
                cin >> id;

                // 规则：如果id之前没占过座位，那么他的out命令也是无效的
                if (personStartSeat[id] == -1) {
                    cout << "no" << endl;
                    continue;
                }

                // 释放座位成功
                cout << "yes" << endl;
                int startIdx = personStartSeat[id];
                int num = personSeatCount[id];

                // 释放座位
                for (int j = 0; j < num; ++j) {
                    seats[startIdx + j] = -1;
                }

                // 重置用户状态
                personStartSeat[id] = -1;
                personSeatCount[id] = 0;
            }
        }
    }
    return 0;
}