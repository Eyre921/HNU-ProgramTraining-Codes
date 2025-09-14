 
/*12. Maya历法
【问题描述】

在学术休假期间，M.A. Ya教授在古老的Maya历法上有一个惊人的发现。从一个古老的令人棘手的信息中，教授发现Maya文明以365天为一年，称为Haab，包含19个月。前18个月每月有20天，月份名字为：pop、no、zip、zotz、tzec、xul、yoxkin、mol、chen、yax、zac、ceh、mac、kankin、muan、pax、koyab、cumhu。每月的天数使用数字来表示，从0~19，而不是用名字。Haab的最后一个月叫做uayet，有5天，表示为0、1、2、3、4。玛雅人认为这个月是不吉利的，法院不开庭，贸易停止了，人们甚至停止清扫地板。

       出于宗教的目的，Maya人使用另外一套历法，叫做Tzolkin（冬青年）。一年被分为13个期间，每个期间20天。每天被表示为由数字和日期名表示的数对。使用20个名字：imix、ik、akbal、kan、chicchan、cimi、manik、lamat、muluk、ok、chuen、eb、ben、ix、mem、cib、caban、eznab、canac、ahau，以及13个数字，双循环使用。

       请注意，每一天都有一个明确的描述。例如，在年初的日子被描述如下：

       1 imix, 2 ik, 3 akbal, 4 kan, 5 chicchan, 6 cimi, 7 manik, 8 lamat, 9 muluk, 10 ok, 11 chuen, 12 eb, 13 ben, 1 ix, 2 mem, 3 cib, 4 caban, 5 eznab, 6 canac, 7 ahau, 在下一个期间开始为 8 imix, 9 ik, 10 akbal . . .

       年份（包含Haab和Tzolkin)用数字0、1、...来表示，数字0是世界的开始。因此，第一天表示为：

       Haab: 0. pop 0

      Tzolkin: 1 imix 0

      请帮M.A.Ya教授写一个程序，将Haab日历转换为Tzolkin日历。


【输入形式】

       在Haab中日期用以下形式表示：

               NumberOfTheDay. Month Year

       输入文件的第一行包含文件中输入日期的数目。接下来的n行包含Haab日历格式的n个日期，年份小于5000。


【输出形式】

        Tzolkin日期用一下格式：

               Number NameOfTheDay Year

        输出包括n行，按照与输入日期对应的顺序，输出tzolkin日历格式日期。


【样例输入】

3
10.zac 0
0.pop 0
10.zac 1995

【样例输出】

3 chuen 0
1 imix 0
9 cimi 2801
*/
/**
 * @file 12.Maya_Calendar.cpp
 * @brief Maya历法转换问题的C++实现
 *
 * 核心思路:
 * 本题的核心是实现两种不同历法之间的转换。最直接有效的方法是设立一个“绝对日”基准，
 * 将所有日期都转换成从创世第一天开始算起的总天数。
 *
 * 1. 哈布历 (Haab) -> 总天数:
 * 首先，将输入的哈布历日期转换为一个绝对天数。哈布历一年365天，前18个月每月20天，
 * 最后一个月(uayet)5天。总天数的计算公式为：
 * totalDays = haabYear * 365 + (月份序号 * 20) + haabDay
 * 为了高效地根据月份名称找到其序号，我们预先构建一个从月份字符串到整数索引的
 * 哈希表 (std::unordered_map)，使得查找操作的平均时间复杂度为 O(1)。
 *
 * 2. 总天数 -> 卓尔金历 (Tzolkin):
 * 然后，将这个绝对天数再转换回卓尔金历的表示形式。卓尔金历以260天为一个周期（年）。
 * - 卓尔金历年份: totalDays / 260
 * - 卓尔金历周期内天数: totalDays % 260
 * - 日期数字 (1-13循环): (周期内天数 % 13) + 1
 * - 日期名称 (20个名称循环): 名称列表[周期内天数 % 20]
 *
 * 时间复杂度: O(N)
 * 其中 N 是输入的日期数量。对于每个日期，其转换过程仅涉及几次算术运算和一次哈希表查找，
 * 这些都是常数时间操作。因此，总时间复杂度与输入日期的数量成线性关系。
 *
 * 空间复杂度: O(1)
 * 我们使用了哈希表和向量来存储历法的基本信息（月份名、日期名）。
 * 这些数据结构的大小是固定的，不随输入规模 N 的变化而变化，因此空间复杂度为常数。
 */

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

// 按照规范，直接使用 std 命名空间
using namespace std;

int main() {
    // 优化C++标准流的输入输出性能，这在处理大量输入时尤其重要
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // --- 数据预处理: 初始化历法常量 ---
    // 使用 unordered_map 存储哈布历月份名到其索引的映射，实现 O(1) 级别的快速查找
    // 声明为 static const 确保数据在程序的整个生命周期中只初始化一次，且内容不可修改
    static const unordered_map<string, int> haabMonthToIndex = {
        {"pop", 0}, {"no", 1}, {"zip", 2}, {"zotz", 3}, {"tzec", 4},
        {"xul", 5}, {"yoxkin", 6}, {"mol", 7}, {"chen", 8}, {"yax", 9},
        {"zac", 10}, {"ceh", 11}, {"mac", 12}, {"kankin", 13}, {"muan", 14},
        {"pax", 15}, {"koyab", 16}, {"cumhu", 17}, {"uayet", 18}
    };

    // 使用 vector 存储卓尔金历的20个日期名称，便于通过索引访问
    static const vector<string> tzolkinDayNames = {
        "imix", "ik", "akbal", "kan", "chicchan", "cimi", "manik", "lamat",
        "muluk", "ok", "chuen", "eb", "ben", "ix", "mem", "cib", "caban",
        "eznab", "canac", "ahau"
    };

    // --- 定义历法周期常量，避免在代码中使用“魔数”，增强可读性和可维护性 ---
    const int HAAB_YEAR_DAYS = 365;
    const int HAAB_REGULAR_MONTH_DAYS = 20;
    const int TZOLKIN_CYCLE_DAYS = 260;
    const int TZOLKIN_NUMBER_CYCLE = 13;
    const int TZOLKIN_NAME_CYCLE = 20;

    int numTestCases;
    cin >> numTestCases; // 读取测试用例的数量

    while (numTestCases--) {
        int haabDay, haabYear;
        string haabMonth;
        char dotSeparator; // 用于读取并优雅地忽略日和月之间的点号

        // --- 输入与核心转换逻辑 ---
        cin >> haabDay >> dotSeparator >> haabMonth >> haabYear;

        // 步骤1: 将哈布历日期精确转换为总天数
        // 总天数 = 已经过去的完整年份天数 + 当年内已经过去的完整月份天数 + 当月的日数
        int totalDays = haabYear * HAAB_YEAR_DAYS +
                        haabMonthToIndex.at(haabMonth) * HAAB_REGULAR_MONTH_DAYS +
                        haabDay;

        // 步骤2: 将总天数转换为卓尔金历日期
        // 计算卓尔金历的年份
        int tzolkinYear = totalDays / TZOLKIN_CYCLE_DAYS;

        // 计算在当前卓尔金历周期中的是第几天 (索引从0开始，范围 0-259)
        int dayInTzolkinCycle = totalDays % TZOLKIN_CYCLE_DAYS;

        // 计算卓尔金历的日期数字 (周期为1-13)
        int tzolkinDayNumber = dayInTzolkinCycle % TZOLKIN_NUMBER_CYCLE + 1;

        // 获取卓尔金历的日期名称 (周期为20天)
        const string& tzolkinDayName = tzolkinDayNames[dayInTzolkinCycle % TZOLKIN_NAME_CYCLE];

        // --- 输出结果 ---
        // 遵循题目要求的格式: Number NameOfTheDay Year
        cout << tzolkinDayNumber << " " << tzolkinDayName << " " << tzolkinYear << endl;
    }

    return 0;
}