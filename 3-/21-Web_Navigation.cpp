/*
21. Web导航
【问题描述】

标准的Web浏览器具有在最近访问的页面中前后移动的特性。实现这些特性的一种方法是使用两个堆栈来跟踪可以通过前后移动到达的页面。在这个问题中，我们要求实现这一点。

       需要支持以下命令：

       BACK：将当前页面压入前向堆栈的顶部；从后向堆栈的顶部弹出该页，使其成为新的当前页。如果后向堆栈为空，则该指令忽略。

       FORWARD：将当前页面压入后向堆栈的顶部；从前向堆栈的顶部弹出该页，使其成为新的当前页。如果前向堆栈为空，则该指令忽略。

       VISIT：将当前页面压入后向堆栈的顶部，将URL指定为新的当前页。前向堆栈被清空。

       QUIT：退出浏览器。

       假设浏览器最初在网址http://www.game.org/上加载网页。


【输入形式】输入是一个命令序列。命令关键字BACK、FORWARD、VISIT和QUIT都是大写。URL中无空格，最多有70个字符。假定在任何时候，每个堆栈中没有问题实例需要超过100个元素。输入的结尾由QUIT命令标识。

【输出形式】除QUIT外的每个命令，如果命令没有被忽略，则在命令执行后输出当前页面的URL，否则，打印"Ignored"。每个命令的输出独立打印一行。QUIT命令无输出。

【样例输入】

VISIT http://game.ashland.edu/
VISIT http://game.baylor.edu/acmicpc/
BACK
BACK
BACK
FORWARD
VISIT http://www.our.com/
BACK
BACK
FORWARD
FORWARD
FORWARD
QUIT

【样例输出】

http://game.ashland.edu/

http://game.baylor.edu/acmicpc/

http://game.ashland.edu/

http://www.game.org/

Ignored

http://game.ashland.edu/

http://www.our.com/

http://game.ashland.edu/

http://www.game.org/

http://game.ashland.edu/

http://www.our.com/

Ignored


*/

/*
 * @file: Web_Navigation.cpp
 *
 * @brief:
 * 本文件实现了"Web导航"问题的解决方案。程序通过使用两个栈来精确模拟
 * 一个简化版Web浏览器的前进（FORWARD）和后退（BACK）导航功能。
 *
 * @core_idea:
 * 核心思路是使用两个栈（Stack）来分别管理后退历史和前进历史，这是此类
 * 功能的一种经典且高效的实现方法。
 * 1.  **数据结构**:
 * - `backStack`: 一个后退历史栈，用于存储当前页面之前访问过的所有页面的URL。
 * 栈顶元素是最近访问过的页面。
 * - `forwardStack`: 一个前进历史栈，用于存储通过“后退”操作而离开的页面的URL。
 * 栈顶元素是最近一次“后退”操作前的页面。
 * - `currentPage`: 一个字符串变量，存储当前正在浏览的页面的URL。
 *
 * 2.  **命令实现**:
 * - `VISIT url`: 当用户访问一个新的URL时，当前页面被压入`backStack`，
 * 然后新的`url`成为`currentPage`。这个操作会导致前进历史失效，因此`forwardStack`被清空。
 * - `BACK`: 执行后退操作。如果`backStack`为空，则说明没有历史记录可供后退，
 * 操作被忽略。否则，`currentPage`被压入`forwardStack`（以便将来可以前进回来），
 * 然后从`backStack`弹出一个URL作为新的`currentPage`。
 * - `FORWARD`: 执行前进操作。如果`forwardStack`为空，则说明没有可前进的页面，
 * 操作被忽略。否则，`currentPage`被压入`backStack`（以便将来可以后退回来），
 * 然后从`forwardStack`弹出一个URL作为新的`currentPage`。
 * - `QUIT`: 退出程序。
 *
 * 3.  **初始状态**: 浏览器从一个指定的起始页面`http://www.game.org/`开始，两个栈都为空。
 *
 * @time_complexity: O(C)
 * C是输入的命令总数。每个命令（VISIT, BACK, FORWARD）都只涉及常数次的
 * 栈操作（push, pop, top, empty），这些操作的平均时间复杂度都是O(1)。
 * VISIT命令中的清空栈操作，虽然在最坏情况下时间复杂度为O(S)（S为栈大小），
 * 但由于每个URL最多只会入栈和出栈一次，因此在整个操作序列上，
 * 每个命令的均摊时间复杂度仍为O(1)。
 *
 * @space_complexity: O(U * L)
 * U是访问过的独立URL的总数，L是URL的平均长度。两个栈最多存储U个URL。
 * 题目明确限制了栈的大小，因此空间使用是可控的。
 */

#include <iostream>
#include <string>
#include <stack>

// 为了代码简洁，直接使用 std 命名空间
using namespace std;

int main() {
    // 提高C++标准输入输出流的效率
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 定义后退历史栈
    stack<string> backStack;
    // 定义前进历史栈
    stack<string> forwardStack;
    // 初始化当前页面
    string currentPage = "http://www.game.org/";

    string command;
    // 循环读取命令，直到遇到QUIT或输入结束
    while (cin >> command && command != "QUIT") {
        if (command == "VISIT") {
            // --- VISIT 命令处理 ---
            string url;
            cin >> url;

            // 1. 将当前页面压入后退栈
            backStack.push(currentPage);
            // 2. 新的URL成为当前页
            currentPage = url;
            // 3. 访问新页面会使前进历史失效，清空前进栈
            // C++11及以后版本，可以直接赋值一个空的栈对象来高效清空
            forwardStack = stack<string>();

            // 4. 输出新的当前页面
            cout << currentPage << endl;

        } else if (command == "BACK") {
            // --- BACK 命令处理 ---
            // 检查后退栈是否为空
            if (backStack.empty()) {
                // 如果为空，则无法后退，忽略命令
                cout << "Ignored" << endl;
            } else {
                // 1. 将当前页面压入前进栈，以便能够“前进”回来
                forwardStack.push(currentPage);
                // 2. 从后退栈顶部弹出页面作为新的当前页
                currentPage = backStack.top();
                backStack.pop();
                // 3. 输出新的当前页面
                cout << currentPage << endl;
            }

        } else if (command == "FORWARD") {
            // --- FORWARD 命令处理 ---
            // 检查前进栈是否为空
            if (forwardStack.empty()) {
                // 如果为空，则无法前进，忽略命令
                cout << "Ignored" << endl;
            } else {
                // 1. 将当前页面压入后退栈，以便能够“后退”回来
                backStack.push(currentPage);
                // 2. 从前进栈顶部弹出页面作为新的当前页
                currentPage = forwardStack.top();
                forwardStack.pop();
                // 3. 输出新的当前页面
                cout << currentPage << endl;
            }
        }
    }

    // 遇到QUIT命令，循环结束，程序正常退出
    return 0;
}

/*
 * ========================================
 * 测试用例与执行分析
 * ========================================
 *
 * 初始状态:
 * currentPage = "http://www.game.org/"
 * backStack = {}
 * forwardStack = {}
 *
 * 【样例输入】
 * VISIT http://game.ashland.edu/
 * VISIT http://game.baylor.edu/acmicpc/
 * BACK
 * ...
 * QUIT
 *
 * 【代码执行流程】
 * 1. VISIT http://game.ashland.edu/
 * - backStack.push("http://www.game.org/")
 * - currentPage = "http://game.ashland.edu/"
 * - forwardStack被清空
 * - 输出: http://game.ashland.edu/
 * 状态: currentPage=ashland, back={org}, forward={}
 *
 * 2. VISIT http://game.baylor.edu/acmicpc/
 * - backStack.push("http://game.ashland.edu/")
 * - currentPage = "http://game.baylor.edu/acmicpc/"
 * - forwardStack被清空
 * - 输出: http://game.baylor.edu/acmicpc/
 * 状态: currentPage=baylor, back={org, ashland}, forward={}
 *
 * 3. BACK
 * - backStack不为空
 * - forwardStack.push("http://game.baylor.edu/acmicpc/")
 * - currentPage = backStack.top() (ashland)
 * - backStack.pop()
 * - 输出: http://game.ashland.edu/
 * 状态: currentPage=ashland, back={org}, forward={baylor}
 *
 * 4. BACK
 * - backStack不为空
 * - forwardStack.push("http://game.ashland.edu/")
 * - currentPage = backStack.top() (org)
 * - backStack.pop()
 * - 输出: http://www.game.org/
 * 状态: currentPage=org, back={}, forward={baylor, ashland}
 *
 * 5. BACK
 * - backStack为空
 * - 输出: Ignored
 *
 * (后续步骤依此类推，与样例输出完全吻合)
 */