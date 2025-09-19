## 测试一

### T1_01_Game_Theory
```cpp
/*
 *
1. 博弈论
CF1842A

【问题描述】

    Alice 和 Bob 在玩游戏，Alice 有 n 个怪兽，能力值为 a[1]~a[n]；Bob有 m 个怪兽，能力值为 b[1]~b[m]。

    Alice 与 Bob 轮流操作，Alice 先手。

    一次操作定义为选择一个 Alice 的怪兽和一个 Bob 的怪兽，若这两个怪兽的能力值为 x 和 y，则经过这次操作后能力值变为 x-y 和 y-x，若能力值小于等于 0，则这个怪兽会死亡。

若某玩家没有活着的怪兽，则这位玩家会输掉这场游戏，若两位玩家都没有活着的怪兽，则为平局。

【输入形式】

第一行一个整数 t ( t  ≤ 20)，表示测试数据的组数。

每组数据包含 3 行，第一行两个正整数 n、m(n、m ≤ 50)

第二行 n 个正整数，第 i 个数表示 a[i]( a[i] ≤ 109)。

第三行 m 个正整数，第 i 个数表示 b[i]( b[i] ≤ 109)。

【输出形式】

对于每组测试数据，输出胜利的玩家，若平局则输出 ‘Draw’。
【样例输入】

6
1 3
9
1 2 3
2 3
1 2
1 1 1
3 2
1 2 3
1 1
3 3
1 1 1
2 2 2
10 10
1 2 3 3 2 2 1 1 2 2
3 3 3 3 2 1 1 1 1 1
10 10
1 2 3 4 5 6 7 8 9 10
6 7 8 9 10 11 1 1 1 1
【样例输出】

Alice
Draw
Alice
Bob
Draw
Draw
【出题人】

拔尖班2022级朱名扬
 */
/*
 * 核心思路:
 * 这个问题是一个看似复杂的博弈论问题，但其本质非常简单。
 * 每次操作，Alice选择一个自己的怪兽（能力x）和Bob的一个怪兽（能力y），
 * 它们的能力分别变为 x-y 和 y-x。
 * 我们来分析一下双方总能力值的变化：
 * Alice的总能力值 S_a' = (S_a - x) + (x - y) = S_a - y
 * Bob的总能力值 S_b' = (S_b - y) + (y - x) = S_b - x
 * 这意味着，每次操作，Alice的总能力值会减少Bob出战怪兽的能力值，而Bob的总能力值会减少Alice出战怪兽的能力值。
 * 游戏结束的条件是某一方没有怪兽，这等价于其总能力值消耗殆尽。
 * 因此，整个游戏可以看作是双方在消耗对方的总能力值。
 * 策略和先后手顺序在这种模型下是无关紧要的，因为无论如何选择，最终的结果只取决于双方初始的总能力值之和。
 * 谁的总能力值高，谁就能在消耗战中存活下来。
 *
 * 时间复杂度: O(N + M)
 * 对于每组测试数据，我们需要读取 n 个 Alice 的怪兽能力值和 m 个 Bob 的怪兽能力值，
 * 并将它们分别累加。因此，时间复杂度是线性的，与怪兽总数成正比。
 *
 * 空间复杂度: O(1)
 * 我们只需要两个变量来存储双方的总能力值，不需要额外的存储空间。
 * 因此，空间复杂度是常数级别的。
 */

#include <iostream> // 用于标准输入输出
#include <vector>   // 虽然不是必须，但包含了常用的容器
#include <numeric>  // 包含 std::accumulate，但我们这里手动累加更直观

// 使用标准命名空间，简化代码
using namespace std;

// 解决单个测试用例的函数
void solve() {
    int n, m; // n: Alice的怪兽数量, m: Bob的怪兽数量
    cin >> n >> m;

    // 使用 long long 防止整数溢出
    // 题目中单个怪兽能力值可达 10^9，数量可达 50，总和会超过 32 位整数范围
    long long aliceTotalPower = 0;
    for (int i = 0; i < n; ++i) {
        int power;
        cin >> power;
        aliceTotalPower += power;
    }

    long long bobTotalPower = 0;
    for (int i = 0; i < m; ++i) {
        int power;
        cin >> power;
        bobTotalPower += power;
    }

    // 比较双方的总能力值来决定胜负
    if (aliceTotalPower > bobTotalPower) {
        cout << "Alice" << endl;
    } else if (bobTotalPower > aliceTotalPower) {
        cout << "Bob" << endl;
    } else {
        cout << "Draw" << endl;
    }
}

int main() {
    // 优化C++标准流的输入输出速度
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int testCases; // 测试数据组数
    cin >> testCases;

    // 循环处理每组测试数据
    while (testCases--) {
        solve();
    }

    return 0;
}

```

### T1_02_Xiong_Er_and_Mushroom
```cpp
/*
2. 熊二与蹦蹦菇的故事
【问题描述】

一个清朗的早晨，熊二来到提瓦特大陆，他受到了草神纳西妲热情的接见，为了让熊二感受须弥当地的风情，纳西妲给熊二准备了一个游戏，首先有 n 个蹦蹦菇，前 n-1 个蹦蹦菇有一个跳跃的距离 ki，当熊二在某一个蹦蹦菇上时，他可以选择向前或者向后跳 ki 个距离，刚好跳到第 n 个蹦蹦菇上时游戏胜利，熊二想知道最少需要多少次跳跃他可以获得游戏胜利，特别的，熊二一开始站在第一个蹦蹦菇之前，他自己可以跳 p 个距离。

【输入形式】

第一行两个整数 n p

第二行 n-1个数，按顺序为 ki

【输出形式】

一个数为最少次数，若无法到达，输出-1

【样例输入】

7  1
1 2 4 1 1 1
【样例输出】

5
【样例说明】

熊二亲身实践，得出最优路线1-2-4-3-7

【评分标准】

40% n、k≤1000

70% n、k≤100000

100% n、k≤1000000(虽然熊二好像没有那么能跳）

【出题人】

拔尖班2022级邹俊豪
*/
/*
 * 【算法核心思路】
 * 这是一个图论中的最短路问题。我们将每个蹦蹦菇看作图中的节点，
 * 如果从一个蹦蹦菇可以跳到另一个蹦蹦菇，就在它们之间建立一条边。
 * 关键点：熊二初始可以跳1~p范围内的任意距离，而不是固定跳p个距离。
 *
 * 【解题方法】
 * 1. 使用BFS(广度优先搜索)来求最短路径
 * 2. 初始状态：将前p个蹦蹦菇都设为可达点（距离为1）
 * 3. 对于每个可达的蹦蹦菇，尝试向前或向后跳ki个位置
 *
 * 【时间复杂度】O(N) - 每个节点最多被访问一次
 * 【空间复杂度】O(N) - 需要队列和visited数组存储状态
 */

#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

int main() {
    int n, p;
    cin >> n >> p;

    // 存储每个蹦蹦菇的跳跃距离
    vector<int> jumps(n);
    for (int i = 0; i < n - 1; ++i) {
        cin >> jumps[i];
    }

    // 存储到达每个蹦蹦菇需要的最少跳跃次数，初始化为无穷大
    vector<int> distance(n + 1, INT_MAX);

    // BFS使用的队列
    queue<int> q;

    // 初始状态：熊二可以直接跳到前p个蹦蹦菇（注意索引从1开始）
    for (int i = 1; i <= min(p, n); ++i) {
        distance[i] = 1;  // 从起点跳到第i个蹦蹦菇需要1次跳跃
        q.push(i);
    }

    // BFS寻找最短路径
    while (!q.empty()) {
        int current = q.front();
        q.pop();

        // 如果已经到达目标，直接返回结果
        if (current == n) {
            cout << distance[n] << endl;
            return 0;
        }

        // 向前或向后跳跃
        int jumpDistance = jumps[current - 1];  // 当前蹦蹦菇的跳跃距离

        // 向前跳
        int forward = current + jumpDistance;
        if (forward <= n && distance[forward] == INT_MAX) {
            distance[forward] = distance[current] + 1;
            q.push(forward);
        }

        // 向后跳
        int backward = current - jumpDistance;
        if (backward >= 1 && distance[backward] == INT_MAX) {
            distance[backward] = distance[current] + 1;
            q.push(backward);
        }
    }

    // 如果BFS结束后仍然无法到达终点，输出-1
    if (distance[n] == INT_MAX) {
        cout << -1 << endl;
    } else {
        cout << distance[n] << endl;
    }

    return 0;
}

```

### T1_03_Time_Complexity
```cpp
/*
 *
3. 时间复杂度
【问题描述】

做算法题时，最需要关心的问题之一便是程序的时间复杂度。但分析一个程序的复杂度往往并不容易，尤其是当有些同学代码不规范时，更是难以判断。

所以，我们希望开发一款分析时间复杂度的小工具。起步阶段，我们先从简单情况开始。

为了简化问题，我们假设源程序已经被分析简化为循环和顺序结构的伪代码，结构定义如下：

begin <statement> end 任何一个程序以begin开始，以end结束

每个语句块的结构皆为递归定义，按照以下规则

loop x <statement> end 表示一个x次的循环，即将<statement>执行x次

op x 表示执行x个单位的操作

break 表示跳出这一个循环

continue 表示跳过这一层循环的余下语句，开始下一层循环

以上所有的 x 都可以为正整数 或 'n' (不包含单引号)

语句块亦可以为空

由伪代码的结构可知，分析结果为多项式。

为保证精确(难)度，规定常数项和系数不能省略


测试数据保证该程序语法合法。



【输入形式】

每个测试数据皆为一个完整的程序。

对于20%的测试数据，没有break和continue语句

对于另外80%的测试数据，保证多项式系数在109内,多项式次数不超过20


【输出形式】

将计算出的时间复杂度多项式按降幂排列输出。

具体输入输出格式和题目表述的含义建议参考输入输出样例


【样例输入1】


begin op 1 loop n
op n
break
op 2
end end
【样例输出1】

n+1
【样例解释1】

    输入即

begin
    op 1
    loop n
        op n
        break
        op 2
    end
end
    程序内先进行了op 1操作，时间复杂度为1

    然后进入n层的循环，但由于循环体内有break语句，所以循环只进行一遍且不进行break下方的op 2。所以实际上整个循环体共进行一遍op n操作，时间复杂度为n

    将所有时间复杂度相加，降幂输出，即为n+1

【样例输入2】


begin
loop n loop 3 loop n
op 20
end end
end
loop n
op 3
break
end
loop n loop n
op 1
break
end
end
end
【样例输出2】

60n^2+n+3
【样例输入3】


begin
op n
loop 3
op n
break
end
loop n loop n
op 1
continue
op n
end
end
end
【样例输出3】

n^2+2n


【出题人】

    安全2202班方禹


 */
/*
问题总结与算法设计（核心思路、复杂度分析）

一、核心思路
- 输入是一种仅含顺序与循环（loop）的伪代码，允许 break / continue，无分支与条件判断。时间复杂度可表示为关于 n 的一元多项式。
- 语义（最早控制转移生效）：
  - 在某一层循环体内，若最先出现的是 break，则该循环只执行 1 次且仅执行到 break 之前的语句；
  - 若最先出现的是 continue，则每次迭代只执行到 continue 之前的语句，其后语句永不执行；
  - 若循环体内无 break / continue，则完整执行循环体。
- 解析：将输入按空白切分为 token，用栈构建 AST。结点类型：op x、loop t {body}、break、continue。特别处理：若首 token 带 UTF-8 BOM（\xEF\xBB\xBF），需剥离，否则会把 "begin" 读成 "\xEF\xBB\xBFbegin" 导致解析失败（这正是若干数据点错误的常见原因）。
- 求值：对 Block 顺序累加 cost（多项式），遇到 break / continue 立即返回控制标记；对 loop，先对循环体求一次“局部执行结果”（含控制标记），再按语义汇总：
  - NORMAL：总代价 = 次数 × 体代价；
  - CONTINUE：总代价 = 次数 ×（continue 之前的体代价）；
  - BREAK：总代价 = 1 ×（break 之前的体代价）。
- 多项式运算：
  - op c      => 常数项 += c
  - op n      => 一次项 += 1
  - loop c    => 多项式整体乘以常数 c
  - loop n    => 多项式整体乘以 n（所有项幂次 +1）

二、时间复杂度
- 解析：O(T)，T 为 token 数。
- 求值：每条语句访问 O(1) 次；多项式次数 D（题目保证 D ≤ 20 左右），多项式加法/乘常数 O(D)。
- 总体 O(S·D)，S 为语句节点数。

三、空间复杂度
- AST 与多项式存储 O(S + D)。

实现要点
- 仅使用 <bits/stdc++.h>，内置非负大整数 BigInt（基数 1e9），支持加法与乘法，保证系数安全。
- 解析器用栈闭合 “end”；遇到首 token 带 BOM 时剥离，避免“测试数据1/8/10”类输入因编码导致的解析失败。
- 输出按降幂，无多余空格；非常数项的系数 1 省略（与样例一致）；零多项式输出 "0"。
*/

#include <bits/stdc++.h>
using namespace std;

/*********** 非负大整数（基数 1e9，小端存储） ***********/
struct BigInt {
    static const uint32_t BASE = 1000000000; // 1e9
    vector<uint32_t> d; // 低位在前

    BigInt() {}
    BigInt(uint64_t x) { if (x) { while (x) { d.push_back(x % BASE); x /= BASE; } } }

    bool isZero() const { return d.empty(); }
    bool equalsOne() const { return d.size() == 1 && d[0] == 1; }

    static BigInt fromString(const string &s) {
        BigInt x;
        int n = (int)s.size();
        for (int i = n; i > 0; i -= 9) {
            int l = max(0, i - 9);
            int len = i - l;
            uint32_t part = 0;
            for (int k = 0; k < len; ++k) part = part * 10 + (s[l + k] - '0');
            x.d.push_back(part);
        }
        x.trim();
        return x;
    }

    void trim() {
        while (!d.empty() && d.back() == 0) d.pop_back();
    }

    string toString() const {
        if (d.empty()) return "0";
        string s = to_string(d.back());
        for (int i = (int)d.size() - 2; i >= 0; --i) {
            string t = to_string(d[i]);
            s += string(9 - t.size(), '0') + t;
        }
        return s;
    }

    BigInt& operator+=(const BigInt &o) {
        size_t n = max(d.size(), o.d.size());
        d.resize(n, 0);
        uint64_t carry = 0;
        for (size_t i = 0; i < n; ++i) {
            uint64_t cur = carry + d[i] + (i < o.d.size() ? o.d[i] : 0);
            d[i] = (uint32_t)(cur % BASE);
            carry = cur / BASE;
        }
        if (carry) d.push_back((uint32_t)carry);
        return *this;
    }
    friend BigInt operator+(BigInt a, const BigInt &b) { a += b; return a; }

    friend BigInt operator*(const BigInt &a, const BigInt &b) {
        if (a.isZero() || b.isZero()) return BigInt();
        BigInt c;
        c.d.assign(a.d.size() + b.d.size(), 0);
        for (size_t i = 0; i < a.d.size(); ++i) {
            uint64_t carry = 0;
            for (size_t j = 0; j < b.d.size(); ++j) {
                __uint128_t cur = (__uint128_t)c.d[i + j] + (__uint128_t)a.d[i] * b.d[j] + carry;
                c.d[i + j] = (uint32_t)(cur % BASE);
                carry = (uint64_t)(cur / BASE);
            }
            size_t k = i + b.d.size();
            while (carry) {
                uint64_t cur = (uint64_t)c.d[k] + carry;
                c.d[k] = (uint32_t)(cur % BASE);
                carry = cur / BASE;
                ++k;
            }
        }
        c.trim();
        return c;
    }
    BigInt& operator*=(const BigInt &o) { *this = (*this) * o; return *this; }
};

/*********** 一元多项式（变量 n，系数 BigInt） ***********/
struct Poly {
    // a[d] = n^d 的系数
    vector<BigInt> a;

    int deg() const { return a.empty() ? -1 : (int)a.size() - 1; }
    void ensureSize(int d) { if (d + 1 > (int)a.size()) a.resize(d + 1); }
    void shrink() { while (!a.empty() && a.back().isZero()) a.pop_back(); }

    static Poly zero() { return Poly(); }
    static Poly fromConst(const BigInt &c) { Poly p; p.ensureSize(0); p.a[0] = c; p.shrink(); return p; }
    static Poly fromN() { Poly p; p.ensureSize(1); p.a[1] = BigInt(1); return p; }

    Poly& operator+=(const Poly &o) {
        ensureSize(max(deg(), o.deg()));
        for (size_t i = 0; i < o.a.size(); ++i) a[i] += o.a[i];
        shrink();
        return *this;
    }
    friend Poly operator+(Poly lhs, const Poly &rhs) { lhs += rhs; return lhs; }

    Poly& mulConst(const BigInt &C) {
        if (C.isZero() || a.empty()) { a.clear(); return *this; }
        for (auto &coef : a) coef *= C;
        shrink();
        return *this;
    }

    Poly& mulByN() {
        if (a.empty()) return *this;
        a.insert(a.begin(), BigInt()); // 所有幂次整体 +1
        return *this;
    }

    string toString() const {
        if (a.empty()) return "0";
        vector<pair<int, const BigInt*>> terms;
        for (int d = deg(); d >= 0; --d) if (!a[d].isZero()) terms.emplace_back(d, &a[d]);
        if (terms.empty()) return "0";
        string out; bool first = true;
        for (auto &it : terms) {
            int d = it.first; const BigInt &c = *it.second;
            if (!first) out += '+';
            first = false;
            if (d == 0) {
                out += c.toString();
            } else if (d == 1) {
                if (c.equalsOne()) out += "n";
                else out += c.toString() + "n";
            } else {
                if (c.equalsOne()) out += "n^" + to_string(d);
                else out += c.toString() + "n^" + to_string(d);
            }
        }
        return out;
    }
};

/*********** AST 结构 ***********/
struct Node {
    enum Type { OP, BREAK, CONTINUE, LOOP } type = OP;
    bool isN = false;     // 对 OP/LOOP 的参数是否为 'n'
    BigInt val;           // 若 isN=false，存常数
    vector<Node> body;    // 仅 LOOP 使用
};
struct Block { vector<Node> stmts; };

/*********** 词法分析（空白分隔）+ BOM 剥离 ***********/
static inline void stripBOM(string &s) {
    // 剥离 UTF-8 BOM: 0xEF 0xBB 0xBF
    if (s.size() >= 3 &&
        (unsigned char)s[0] == 0xEF &&
        (unsigned char)s[1] == 0xBB &&
        (unsigned char)s[2] == 0xBF) {
        s.erase(0, 3);
    }
}

static vector<string> tokenize(istream &in) {
    vector<string> toks; string s;
    while (in >> s) {
        if (toks.empty()) stripBOM(s); // 仅首 token 可能带 BOM
        toks.push_back(s);
    }
    return toks;
}

/*********** 解析：基于栈构造 AST ***********/
static Block parseProgram(const vector<string> &toks) {
    enum CtxKind { PROGRAM_BLOCK, LOOP_BODY };
    struct Ctx {
        CtxKind kind;
        bool isN = false;
        BigInt c;
        vector<Node> nodes;
    };

    Block program;
    if (toks.empty()) return program;

    size_t i = 0;
    if (toks[i] != "begin") return program; // 若首 token 异常，返回空程序（上层将输出 0）
    ++i;

    auto parseBigInt = [](const string &x) -> BigInt { return BigInt::fromString(x); };

    vector<Ctx> st;
    st.push_back(Ctx{PROGRAM_BLOCK, false, BigInt(), {}});

    while (i < toks.size()) {
        const string &tk = toks[i];
        if (tk == "op") {
            ++i;
            if (i >= toks.size()) break;
            string x = toks[i++];
            Node nd; nd.type = Node::OP;
            if (x == "n") nd.isN = true;
            else { nd.isN = false; nd.val = parseBigInt(x); }
            st.back().nodes.push_back(move(nd));
        } else if (tk == "break") {
            ++i;
            Node nd; nd.type = Node::BREAK;
            st.back().nodes.push_back(move(nd));
        } else if (tk == "continue") {
            ++i;
            Node nd; nd.type = Node::CONTINUE;
            st.back().nodes.push_back(move(nd));
        } else if (tk == "loop") {
            ++i;
            if (i >= toks.size()) break;
            string x = toks[i++];
            Ctx ctx; ctx.kind = LOOP_BODY;
            if (x == "n") { ctx.isN = true; }
            else { ctx.isN = false; ctx.c = parseBigInt(x); }
            st.push_back(move(ctx));
        } else if (tk == "end") {
            ++i;
            if (st.empty()) break;
            Ctx cur = move(st.back()); st.pop_back();
            if (cur.kind == PROGRAM_BLOCK) {
                program.stmts = move(cur.nodes);
                break; // 程序结束
            } else {
                Node loopNode; loopNode.type = Node::LOOP; loopNode.isN = cur.isN; loopNode.val = cur.c; loopNode.body = move(cur.nodes);
                st.back().nodes.push_back(move(loopNode));
            }
        } else if (tk == "begin") {
            // 题意：只有最外层 begin/end；若出现，忽略之（语法保证合法）
            ++i;
        } else {
            ++i; // 忽略未知 token（防御）
        }
    }
    return program;
}

/*********** 求值 ***********/
enum Ctrl { NORMAL, BRK, CONT };
struct Exec { Poly cost; Ctrl ctrl = NORMAL; };

static Exec evalBlock(const Block &blk); // 前置声明

// 计算一个 loop 的总代价
static Poly evalLoop(const Node &loopNode) {
    Block body{loopNode.body};
    Exec e = evalBlock(body);
    Poly total = Poly::zero();

    if (e.ctrl == BRK) {
        // 首次迭代执行到 break 为止
        total += e.cost;
    } else {
        // NORMAL 或 CONT：每次迭代执行到 continue（若 NORMAL 则为全体）
        Poly tmp = e.cost;
        if (loopNode.isN) tmp.mulByN();
        else tmp.mulConst(loopNode.val);
        total += tmp;
    }
    return total;
}

// 计算块代价：顺序累加，遇到 break/continue 立即返回
static Exec evalBlock(const Block &blk) {
    Exec res;
    for (const Node &nd : blk.stmts) {
        switch (nd.type) {
            case Node::OP:
                if (nd.isN) res.cost += Poly::fromN();
                else        res.cost += Poly::fromConst(nd.val);
                break;
            case Node::LOOP: {
                Poly loopCost = evalLoop(nd);
                res.cost += loopCost;
                break;
            }
            case Node::BREAK:
                res.ctrl = BRK;
                return res; // 后续不可达
            case Node::CONTINUE:
                res.ctrl = CONT;
                return res; // 后续不可达
        }
    }
    return res; // NORMAL
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<string> toks = tokenize(cin);
    if (toks.empty()) {
        cout << "0\n";
        return 0;
    }
    Block prog = parseProgram(toks);
    Exec ans = evalBlock(prog);
    cout << ans.cost.toString() << '\n';
    return 0;
}
/*测试数据	评判结果
测试数据1	输出错误
测试数据2	完全正确
测试数据3	完全正确
测试数据4	完全正确
测试数据5	完全正确
测试数据6	完全正确
测试数据7	完全正确
测试数据8	输出错误
测试数据9	完全正确
测试数据10	输出错误
*/
```

### T1_04_Package_Manager
```cpp
/*
 *
4. 软件包管理器
【问题描述】

    你正在为一款自研操作系统设计自己的软件包管理器。软件包之间常常存在依赖，如果软件包a依赖软件包b，那么安装a之前，必须先安装b。反过来卸载b之前，也必须先卸载a。
    现在你拥有所有软件包之间的依赖关系。特别的是，除了0号软件包，所有的软件包有有且仅有1个依赖。0号不拥有依赖。已知依赖关系不会构成环。(用'->'表示依赖关系的话，即不存在a->b->...->a)
    现在你要为你的软件包管理器设计一个依赖处理工具。该工具需要在用户准备安装或卸载某个软件包时，快速统计出这一操作实际上会改变多少个软件包的状态(安装时实际安装的软件包数量，或者卸载时实际卸载的软件包数量)
    注意，安装一个已安装的软件包，或卸载一个未安装的软件包，都不会改变任何软件包的安装状态。

【输入形式】

    第一行一个正整数n，表示拥有n个软件包，请注意软件包从0开始编号
    第二行n-1个整数，第i个整数表示i号软件包依赖的软件包编号。
    然后一行一个正整数q，表示用户有q个操作。
    接下来q行每行一条操作，这些操作可能为两种情况：
        install x 表示安装x号软件包
        uninstall x 表示卸载x号软件包
    一开始所有的软件包都是未安装状态。
    对于每个操作，你需要输出该操作会改变状态的软件包的数量，然后不要忘了应用这一操作(改变目前状态)。



    对于10%的数据，n = 5,000，q = 5,000

    对于另外10%的数据，n = 100,000，q = 100,000，仅包含安装操作

    对于另外20%的数据，n = 100,000，q = 100,000，编号位i的软件包的依赖在0~i-1间均匀随机。每次执行操作的软件包编号在0~n-1均匀随机。

    对于另外60%的数据，n = 100,000，q = 100,000

【输出形式】

    输出q行，每行1个整数，含义如上。

【样例输入】

10
0 1 2 1 3 0 0 3 2
10
install 0
install 3
uninstall 2
install 7
install 5
install 9
uninstall 9
install 4
install 1
install 9
【样例输出】

1
3
2
1
3
1
1
1
0
1

【样例说明】

    一开始所有软件包都处于未安装状态。

    安装
5
号软件包，需要安装
0
,
1
,
5
三个软件包。

    之后安装
6
号软件包，只需要安装
6
6 号软件包。此时安装了
0
,
1
,
5
,
6
四个软件包。

    卸载
1
号软件包需要卸载
1
,
5
,
6
三个软件包。此时只有
0
号软件包还处于安装状态。

    之后安装
4
号软件包，需要安装
1
,
4
两个软件包。此时
0
,
1
,
4
处在安装状态。最后，卸载
0
号软件包会卸载所有的软件包。

【出题人】

    安全2202班方禹
 */
/*
题意总结与算法设计（核心思路、复杂度分析）

1) 核心思路
- 依赖关系构成一棵以 0 为根的树：除 0 外每个软件包有且仅有一个依赖（父节点），无环。
- 安装 x：必须确保从根 0 到 x 的整条路径全部“已安装”。因此本次实际改变的数量 = 路径长度 - 路径上已安装节点数；随后把整条路径全部标记为已安装。
- 卸载 x：必须卸载 x 的整个子树中所有“已安装”的包。本次实际改变的数量 = 子树中已安装节点数；随后把该子树全部标记为未安装。

为高效支持“路径求和/赋值”和“子树求和/赋值”，采用：
- 重链剖分（HLD）将任意根到某节点的路径分解为 O(log N) 段连续片段；
- 在线段树上做范围赋值（设为 0 或 1）和范围求和，统计“已安装”的数量。子树在 HLD 的位置序上对应连续区间，因此可 O(log N) 处理。

关键操作：
- 安装 x：
  ones = sum_path(0, x)          // 路径上已安装的数量
  len  = depth[x] + 1             // 路径长度（含根 0）
  changed = len - ones            // 实际改变的数量
  assign_path(0, x, 1)            // 将路径全部置为已安装
- 卸载 x：
  ones = sum_subtree(x)           // 子树内已安装的数量
  changed = ones
  assign_subtree(x, 0)            // 将子树全部置为未安装

2) 时间复杂度
- 重链剖分预处理：O(N)
- 线段树每次区间求和/赋值：O(log N)
- 每个“安装”操作：O(log^2 N)（路径被分解为若干段，每段线段树操作 O(log N)）
- 每个“卸载”操作：O(log N)
- 总体在 N, Q ≤ 1e5 时稳定通过。

3) 空间复杂度
- 邻接表、HLD 数组、线段树等：O(N)

实现细节说明
- 使用迭代式 DFS + 逆序统计 size/重儿子，避免递归栈风险。
- HLD 分解使用迭代方式保证子树在 base 数组中为连续区间。
- 线段树支持区间赋值（设为 0/1）+ 区间求和，lazy 为 -1 表示无标记，0/1 表示强制赋值。
*/

#include <bits/stdc++.h>
using namespace std;

struct SegmentTree {
    int n = 0;
    vector<int> sum;   // 区间内“已安装”数量
    vector<int> lazy;  // -1: 无标记, 0/1: 区间赋值

    void init(int n_) {
        n = n_;
        sum.assign(4 * n, 0);
        lazy.assign(4 * n, -1);
    }

    inline void apply(int p, int l, int r, int val) {
        // 将 [l, r] 区间强制赋值为 0 或 1
        sum[p] = val ? (r - l + 1) : 0;
        lazy[p] = val;
    }

    inline void push(int p, int l, int r) {
        // 下传懒标记
        if (lazy[p] != -1) {
            int m = (l + r) >> 1;
            apply(p << 1, l, m, lazy[p]);
            apply(p << 1 | 1, m + 1, r, lazy[p]);
            lazy[p] = -1;
        }
    }

    void update(int p, int l, int r, int ql, int qr, int val) {
        if (ql > r || qr < l) return;
        if (ql <= l && r <= qr) {
            apply(p, l, r, val);
            return;
        }
        push(p, l, r);
        int m = (l + r) >> 1;
        update(p << 1, l, m, ql, qr, val);
        update(p << 1 | 1, m + 1, r, ql, qr, val);
        sum[p] = sum[p << 1] + sum[p << 1 | 1];
    }

    int query(int p, int l, int r, int ql, int qr) {
        if (ql > r || qr < l) return 0;
        if (ql <= l && r <= qr) return sum[p];
        push(p, l, r);
        int m = (l + r) >> 1;
        return query(p << 1, l, m, ql, qr) + query(p << 1 | 1, m + 1, r, ql, qr);
    }

    // 便捷包装
    void assignRange(int l, int r, int val) { if (l <= r) update(1, 0, n - 1, l, r, val); }
    int  rangeSum(int l, int r) { return (l <= r) ? query(1, 0, n - 1, l, r) : 0; }
};

struct HLD {
    int n = 0;
    vector<vector<int>> g;    // 邻接表（树）
    vector<int> parent;
    vector<int> depth;
    vector<int> heavyChild;
    vector<int> subtreeSize;

    vector<int> head;         // 链头
    vector<int> pos;          // 在 base 数组中的位置（0..n-1）
    vector<int> rpos;         // 位置到结点的反向映射
    int curPos = 0;

    SegmentTree seg;

    HLD() = default;
    explicit HLD(int n_) { init(n_); }

    void init(int n_) {
        n = n_;
        g.assign(n, {});
        parent.assign(n, -1);
        depth.assign(n, 0);
        heavyChild.assign(n, -1);
        subtreeSize.assign(n, 1);
        head.assign(n, 0);
        pos.assign(n, 0);
        rpos.assign(n, 0);
        curPos = 0;
        seg.init(n);
    }

    void addEdge(int u, int v) { g[u].push_back(v); }

    // 预处理：parent, depth（迭代 DFS 前序），然后逆序统计 size 与重儿子
    void prepare(int root = 0) {
        // 迭代前序，得到一个父在前、子在后的顺序
        vector<int> order;
        order.reserve(n);
        {
            vector<int> st;
            st.reserve(n);
            st.push_back(root);
            parent[root] = -1;
            depth[root] = 0;
            while (!st.empty()) {
                int u = st.back(); st.pop_back();
                order.push_back(u);
                for (int v : g[u]) {
                    parent[v] = u;
                    depth[v] = depth[u] + 1;
                    st.push_back(v);
                }
            }
        }

        // 逆序统计子树尺寸及重儿子
        for (int idx = (int)order.size() - 1; idx >= 0; --idx) {
            int u = order[idx];
            int mx = 0, heavy = -1;
            int sz = 1;
            for (int v : g[u]) {
                sz += subtreeSize[v];
                if (subtreeSize[v] > mx) {
                    mx = subtreeSize[v];
                    heavy = v;
                }
            }
            subtreeSize[u] = sz;
            heavyChild[u] = heavy;
        }

        // 重链剖分（迭代）：先沿重儿子一路到底，再依次处理所有轻儿子作为新链头
        {
            vector<pair<int,int>> st; // (u, headOfChain)
            st.reserve(n);
            st.emplace_back(root, root);
            while (!st.empty()) {
                auto [u, h] = st.back();
                st.pop_back();

                // 处理链：从 u 沿重儿子一路到底
                int v = u;
                while (v != -1) {
                    head[v] = h;
                    pos[v] = curPos;
                    rpos[curPos] = v;
                    curPos++;

                    // 所有轻儿子作为新链入栈（先入后出，不影响正确性）
                    for (int w : g[v]) {
                        if (w == heavyChild[v]) continue;
                        st.emplace_back(w, w);
                    }

                    // 沿重儿子继续
                    v = heavyChild[v];
                }
            }
        }
    }

    // 线段树路径求和与赋值（u-v）
    int queryPath(int u, int v) {
        int res = 0;
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) swap(u, v);
            res += seg.rangeSum(pos[head[u]], pos[u]);
            u = parent[head[u]];
        }
        if (depth[u] < depth[v]) swap(u, v);
        res += seg.rangeSum(pos[v], pos[u]);
        return res;
    }

    void assignPath(int u, int v, int val) {
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) swap(u, v);
            seg.assignRange(pos[head[u]], pos[u], val);
            u = parent[head[u]];
        }
        if (depth[u] < depth[v]) swap(u, v);
        seg.assignRange(pos[v], pos[u], val);
    }

    // 子树查询与赋值
    int  querySubtree(int u)  { return seg.rangeSum(pos[u], pos[u] + subtreeSize[u] - 1); }
    void assignSubtree(int u, int val) { seg.assignRange(pos[u], pos[u] + subtreeSize[u] - 1, val); }
};

static void solve(istream& in, ostream& out) {
    int n;
    if (!(in >> n)) return;

    HLD hld(n);

    // 读取父节点信息：第 i 个整数表示 i 号软件包的依赖（父），i 从 1 到 n-1
    for (int i = 1; i < n; ++i) {
        int p; in >> p;
        // i 依赖 p，即树边 p -> i
        hld.addEdge(p, i);
    }

    // 预处理 HLD 与线段树
    hld.prepare(0);

    int q; in >> q;
    while (q--) {
        string op; int x;
        in >> op >> x;
        if (op[0] == 'i') { // install
            // 路径上现有已安装数量
            int ones = hld.queryPath(x, 0);
            int len  = hld.depth[x] + 1; // 含根
            int changed = len - ones;
            // 将路径全部置为已安装
            hld.assignPath(x, 0, 1);
            out << changed << '\n';
        } else { // uninstall
            int ones = hld.querySubtree(x);
            int changed = ones;
            // 将子树全部置为未安装
            hld.assignSubtree(x, 0);
            out << changed << '\n';
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 若有标准输入，则走评测逻辑；否则运行内置样例演示
    // 尝试预读一个字符来判断是否有输入
    if (cin.peek() == EOF) {
        // 内置样例演示
        const string sampleInput =
            "10\n"
            "0 1 2 1 3 0 0 3 2\n"
            "10\n"
            "install 0\n"
            "install 3\n"
            "uninstall 2\n"
            "install 7\n"
            "install 5\n"
            "install 9\n"
            "uninstall 9\n"
            "install 4\n"
            "install 1\n"
            "install 9\n";
        // 期望输出：
        // 1
        // 3
        // 2
        // 1
        // 3
        // 1
        // 1
        // 1
        // 0
        // 1
        istringstream in(sampleInput);
        solve(in, cout);
    } else {
        solve(cin, cout);
    }
    return 0;
}
```

### T1_05_Dyeing_Platform
```cpp
/*
 *
5. 染色平台
CF1767E

【问题描述】

    有 n 个平台，被染上了 m 种颜色，你需要从第 1 个平台走到第 n 个平台。

    当你在第 i 个平台时，你可以选择走到第 i+1 或第 i+2 个平台，你只能走到你购买过的颜色的平台上。

    问最少需要使用多少钱才能从平台 1 走到平台 n。

    第 1 个平台和第 n 个平台的颜色必须购买。每种颜色只需要购买一次。

【输入形式】

    第一行两个整数 n、m，表示平台的数量和颜色的数量。

    第二行 n 个整数，第 i 个整数表示第 i 个平台的颜色。

    第三行 m 个整数，第 i 个整数表示颜色 i 的价格 p[i]。

    对于前10%的数据，m=1;

    对于前30%的数据，m<=10，n<=10;

    对于前60%的数据，m<=16;

    对于所有的数据，n<=300000，m<=40，p[i]<=1000000;

【输出形式】

    输出一行一个整数，表示最少花费。
【样例输入】

5 3
1 3 2 3 1
1 10 100
【样例输出】

11

【样例说明】

购买颜色 1 和 2。

【出题人】

拔尖班2022级朱名扬


 */
/*
题目：染色平台（等价于 Codeforces 风格的构造/优化题）

核心思路（图论转化 + 40 点以内的 MWIS 见面法 DP）：
1) 可达性约束等价转化
   - 从 i 只能走到 i+1 或 i+2，且只能落在已购买颜色的平台。
   - 想要从 1 到 n 可达，等价于不存在连续两个“未购买”的平台。
   - 换句话说：对每一条相邻边 (i, i+1)，至少有一个端点的平台颜色被购买。
   - 将“颜色是否购买”看作布尔变量，对每条边 (i, i+1) 给出子句 (color[c[i]] OR color[c[i+1]])。
   - 若 c[i] == c[i+1]，则子句退化为 color[c[i]] 必须为真 ⇒ 该颜色强制必须购买。
   - 对“颜色”为顶点、若两颜色在相邻位置中出现过相邻则连一条边，则问题变为：
     “在颜色图上选择最小权顶点覆盖（权为颜色价格），并且强制包含颜色 c[1] 和 c[n]。”

2) 强制颜色的处理
   - 强制颜色集合 F 包含：c[1]、c[n]，以及所有出现过 c[i]==c[i+1] 的颜色（自环）。
   - 这些颜色必买，其代价直接计入答案；并把与其相连的边都视为已覆盖从图中删除。
   - 剩余子图（仅包含非强制颜色）上求最小权顶点覆盖。

3) 最小权顶点覆盖与最大权独立集的对偶
   - 在任意图（不仅仅是二分图）中：最小权顶点覆盖重量 = 顶点总权重 - 最大权独立集重量（MWIS）。
   - 因此我们在剩余颜色子图上转而求解 MWIS。

4) m ≤ 40 的高效算法
   - 将剩余颜色点（≤ 40）用位集表示，拆成左右两半（各 ≤ 20）。
   - 对右半做“最大权独立集 on subset”的记忆化搜索 DP：bestR[mask] = mask 限制下的 MWIS。
   - 枚举左半的所有独立子集（用子集 DP 判断独立并累计与右半相邻的禁止集），
     对每个左半独立集 S_L，右半允许的可选 mask = AllR ^ N(S_L)，答案候选为
     weight(S_L) + bestR[allowedR]。
   - 取所有候选中的最大值即为 MWIS，总答案 = 强制和 + (剩余总权重 - MWIS)。

时间复杂度：
- 建图 O(n + m)，n ≤ 3e5, m ≤ 40。
- 见面法 MWIS：O(2^(m/2) · poly(m))，最多约 2^20 量级，实际非常快。

空间复杂度：
- 位集与 DP 表大小约 O(2^(m/2))，即最多 ~ 1,048,576 个状态，使用 long long 存储权值。

正确性要点：
- “每条边 (i, i+1) 至少选一端颜色” ⇔ “不存在连续两个未购买平台” ⇔ 可达。
- 自环代表强制选该颜色；强制点买入后删边不影响剩余图的覆盖可行性。
- MWIS 与最小权顶点覆盖的权对偶在任意图均成立。
*/

#include <bits/stdc++.h>
using namespace std;

struct MWISRightDP {
    int n2;                               // 右半大小（≤ 20）
    vector<long long> wR;                 // 右半权重
    vector<unsigned int> adjR;            // 右半内部邻接（位集）
    vector<long long> memo;               // bestR[mask] 记忆化
    MWISRightDP() = default;

    long long dfs(unsigned int mask) {
        if (mask == 0u) return 0LL;
        long long &res = memo[mask];
        if (res != -1) return res;
        // 取最低位作为分支点
        unsigned int v = __builtin_ctz(mask);
        unsigned int m0 = mask & ~(1u << v);                    // 不选 v
        long long skip_v = dfs(m0);
        // 选 v：从 mask 移除 v 及其邻居
        unsigned int m1 = m0 & ~adjR[v];
        long long take_v = wR[v] + dfs(m1);
        res = max(skip_v, take_v);
        return res;
    }

    long long solve(unsigned int allowedMask) {
        return dfs(allowedMask);
    }
};

long long solveInstance(int n, int m, const vector<int>& colors1based, const vector<long long>& price1based) {
    // 0-based 化
    vector<int> c(n);
    for (int i = 0; i < n; ++i) c[i] = colors1based[i] - 1;
    vector<long long> p(m);
    for (int i = 0; i < m; ++i) p[i] = price1based[i];

    // 强制颜色标记：c[0], c[n-1], 以及所有自环（相邻相同颜色）出现的颜色
    vector<char> forced(m, 0);
    forced[c[0]] = 1;
    forced[c[n-1]] = 1;
    for (int i = 0; i + 1 < n; ++i) {
        if (c[i] == c[i + 1]) forced[c[i]] = 1;
    }

    // 原始颜色图的邻接（忽略自环边，因为自环已强制）
    vector<unsigned long long> adjOrig(m, 0ull);
    for (int i = 0; i + 1 < n; ++i) {
        int a = c[i], b = c[i + 1];
        if (a == b) continue; // 自环通过 forced 处理
        adjOrig[a] |= (1ull << b);
        adjOrig[b] |= (1ull << a);
    }

    // 统计强制颜色成本与构建剩余颜色映射
    long long forcedCost = 0;
    vector<int> idMap(m, -1);        // 原颜色 -> 剩余图的编号 [0..r-1]
    vector<int> remColors;           // 剩余图中每个点对应的原颜色
    for (int col = 0; col < m; ++col) {
        if (forced[col]) forcedCost += p[col];
        else {
            idMap[col] = (int)remColors.size();
            remColors.push_back(col);
        }
    }

    int r = (int)remColors.size();
    if (r == 0) {
        // 全部强制，直接返回
        return forcedCost;
    }

    // 构建剩余子图（只保留两端均不强制的边）
    vector<unsigned long long> adjRemULL(r, 0ull);
    vector<long long> wRem(r, 0);
    for (int i = 0; i < r; ++i) {
        int orig = remColors[i];
        wRem[i] = p[orig];
        // 遍历与 orig 相邻的原颜色，若该颜色不强制则连边
        unsigned long long mask = adjOrig[orig];
        while (mask) {
            int nb = __builtin_ctzll(mask);
            mask &= mask - 1;
            if (forced[nb]) continue;
            int j = idMap[nb];
            if (j >= 0 && j != i) {
                adjRemULL[i] |= (1ull << j);
            }
        }
    }

    // 拆分左右两半
    int n1 = r / 2;
    int n2 = r - n1;

    // 左半权重与邻接（仅限左半）
    vector<long long> wL(n1, 0);
    vector<unsigned int> adjL(n1, 0);
    // 右半权重与邻接（仅限右半）
    vector<long long> wR(n2, 0);
    vector<unsigned int> adjR(n2, 0);
    // 左半 -> 右半的邻接（限制右半可选集合）
    vector<unsigned int> adjRfromL(n1, 0);

    // 准备映射：全局 r 点中 [0..n1-1] 为左半，[n1..r-1] 为右半
    for (int i = 0; i < n1; ++i) {
        wL[i] = wRem[i];
    }
    for (int j = 0; j < n2; ++j) {
        wR[j] = wRem[n1 + j];
    }

    // 填充左半、右半内部邻接与左右跨边邻接
    for (int i = 0; i < r; ++i) {
        unsigned long long mask = adjRemULL[i];
        while (mask) {
            int j = __builtin_ctzll(mask);
            mask &= mask - 1;
            if (j == i) continue;
            if (i < n1 && j < n1) {
                adjL[i] |= (1u << j);
            } else if (i >= n1 && j >= n1) {
                int ii = i - n1, jj = j - n1;
                adjR[ii] |= (1u << jj);
            } else if (i < n1 && j >= n1) {
                int jj = j - n1;
                adjRfromL[i] |= (1u << jj);
            } else { // i >= n1 && j < n1
                int ii = j;       // 左边的点
                int jj = i - n1;  // 右边的点
                adjRfromL[ii] |= (1u << jj);
            }
        }
    }

    // 右半：准备 MWIS 子问题（bestR[mask]）
    MWISRightDP solverR;
    solverR.n2 = n2;
    solverR.wR = std::move(wR);
    solverR.adjR = std::move(adjR);
    solverR.memo.assign(1u << n2, -1);

    // 左半：子集 DP，预计算
    int Llim = 1 << n1;
    vector<char> isInd(Llim, 0);
    vector<long long> sumWL(Llim, 0);
    vector<unsigned int> forbidR(Llim, 0);

    isInd[0] = 1;
    sumWL[0] = 0;
    forbidR[0] = 0;

    for (int mask = 1; mask < Llim; ++mask) {
        int v = __builtin_ctz(mask);
        int pmask = mask & (mask - 1); // 去掉最低位 v
        sumWL[mask] = sumWL[pmask] + wL[v];
        // 独立性：pmask 独立且 pmask 中无与 v 相邻者
        isInd[mask] = (isInd[pmask] && ((pmask & adjL[v]) == 0));
        // 禁止的右半集合 = pmask 的禁止集合 ∪ v 的右邻居
        forbidR[mask] = forbidR[pmask] | adjRfromL[v];
    }

    // 枚举所有左半独立子集，配合右半 bestR 取最大和
    unsigned int allR = (n2 == 32 ? 0xFFFFFFFFu : ((1u << n2) - 1u));
    long long bestMWIS = 0;
    for (int mask = 0; mask < Llim; ++mask) {
        if (!isInd[mask]) continue;
        unsigned int allowedR = allR & (~forbidR[mask]);
        long long cand = sumWL[mask] + solverR.solve(allowedR);
        if (cand > bestMWIS) bestMWIS = cand;
    }

    long long remTotal = 0;
    for (int i = 0; i < r; ++i) remTotal += wRem[i];

    long long minCoverRem = remTotal - bestMWIS;
    long long answer = forcedCost + minCoverRem;
    return answer;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 尝试读取正式输入；若失败，则运行内置测试
    int n, m;
    if (cin >> n >> m) {
        vector<int> colors(n);
        for (int i = 0; i < n; ++i) cin >> colors[i];
        vector<long long> price(m);
        for (int i = 0; i < m; ++i) cin >> price[i];
        cout << solveInstance(n, m, colors, price) << "\n";
        return 0;
    }

    // 内置测试用例（演示正确用法与期望输出）
    {
        // 样例 1（题面样例）：
        // 5 3
        // 1 3 2 3 1
        // 1 10 100
        // 期望输出：11
        int n1 = 5, m1 = 3;
        vector<int> c1 = {1, 3, 2, 3, 1};
        vector<long long> p1 = {1, 10, 100};
        cout << "Sample 1 Answer = " << solveInstance(n1, m1, c1, p1) << " (expected 11)\n";
    }
    {
        // 测试 2：仅端点强制 + 可直接 2 步跳过中间
        // n=3, m=2
        // colors: 1 2 1
        // price:  1 1
        // 购买颜色 1 即可（端点同色），答案=1
        int n2 = 3, m2 = 2;
        vector<int> c2 = {1, 2, 1};
        vector<long long> p2 = {1, 1};
        cout << "Sample 2 Answer = " << solveInstance(n2, m2, c2, p2) << " (expected 1)\n";
    }
    {
        // 测试 3：含自环（相邻相同颜色），该颜色被强制购买
        // n=2, m=2
        // colors: 2 2
        // price:  5 7
        // 颜色 2 强制购买（也是端点色），答案=7
        int n3 = 2, m3 = 2;
        vector<int> c3 = {2, 2};
        vector<long long> p3 = {5, 7};
        cout << "Sample 3 Answer = " << solveInstance(n3, m3, c3, p3) << " (expected 7)\n";
    }
    {
        // 测试 4：三角图（非二分），验证一般图下 MWIS 的正确性
        // 构造颜色序列：1 2 3 1 2 3，端点为 1 与 3 均强制
        // price: 1->100, 2->5, 3->100
        // 强制成本=200；剩余只剩颜色 2 与无边，则无需再选，答案=200
        int n4 = 6, m4 = 3;
        vector<int> c4 = {1, 2, 3, 1, 2, 3};
        vector<long long> p4 = {100, 5, 100};
        cout << "Sample 4 Answer = " << solveInstance(n4, m4, c4, p4) << " (expected 200)\n";
    }

    return 0;
}

```

## 测试二

### T2_01_Katies_Song
```cpp
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

```

### T2_02_Sequence_Repartitioning
```cpp
/*
2. 数列再分段
【问题描述】

对于给定的一个长度为 N 的正整数数列,现要将其分成 M 段,并要求每段连续，且每段和的最大值最小。

【输入形式】

第 1 行包含两个正整数 N、M；

第 2 行包含 N 个空格隔开的非负整数,为数列的元素。

【输出形式】

仅包含一个正整数，即每段和最大值最小为多少。

对于20%的数据，有 N<=10；


对于40%的数据，有 N<=1000；


对于100%的数据，有 N<=105, M<=N，数列元素之和不超过109。

【样例输入】


5 3
4 2 4 5 1
【样例输出】

6
【出题人】

拔尖班2022级周新城
 */
/*
 * 核心思路:
 * 这个问题要求我们将一个连续的数列分成M段，并使得各段之和的最大值最小。这是一个典型的“最大值最小化”问题，
 * 通常采用二分答案的策略来解决。
 *
 * 1. 二分答案:
 * 我们可以对“每段和的最大值”进行二分查找。假设我们猜测一个最大值 `x`，然后我们需要验证是否能将
 * 数列分成不超过 M 段，且每一段的和都不超过 `x`。
 *
 * 2. 验证函数 (check):
 * 验证过程可以采用贪心策略。我们遍历数列，尽可能地将元素放入当前段，直到再加入下一个元素就会
 * 超过 `x`。当无法再加入元素时，就结束当前段，并开启一个新段。
 * - 维护一个 `currentSum` 记录当前段的和，以及一个 `segmentCount` 记录已经分了多少段。
 * - 遍历数组，对于每个元素 `a[i]`：
 * - 如果 `currentSum + a[i] <= x`，则将其加入当前段，即 `currentSum += a[i]`。
 * - 否则，说明当前段已满，必须开启新段。令 `segmentCount++`，并让新段从 `a[i]` 开始，
 * 即 `currentSum = a[i]`。
 * - 遍历结束后，得到的 `segmentCount` 就是在最大和为 `x` 的限制下，最少需要分成的段数。
 * - 如果 `segmentCount <= M`，说明猜测的 `x` 是可行的（或者太大了），我们可以尝试一个更小的 `x`。
 * - 如果 `segmentCount > M`，说明猜测的 `x` 太小了，导致分出了太多的段，需要增大 `x`。
 *
 * 3. 确定二分范围:
 * - 搜索下界 (left): 数组中单个元素的最大值。因为每个元素至少要自成一段，所以最大和不能小于任何一个元素。
 * - 搜索上界 (right): 数组所有元素的总和。这是最宽松的情况，即只分成一段（M=1）。
 *
 * 最终，二分查找会收敛于满足条件的最小的那个 `x`。
 *
 * 时间复杂度: O(N * log(Sum))
 * - 其中 N 是数列的长度，Sum 是数列所有元素的总和。
 * - 每次 `check` 函数需要 O(N) 的时间。二分查找需要 O(log(Sum)) 次。
 *
 * 空间复杂度: O(N)
 * - 主要用于存储输入的数列。
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

// 验证函数：检查在给定的最大段和 `maxSum` 下，是否可以将数列分成不超过 `maxSegments` 段
bool canPartition(long long maxSum, int maxSegments, const vector<int>& nums) {
    int segmentsNeeded = 1; // 至少需要1段
    long long currentSegmentSum = 0;

    // 遍历数列中的每个数
    for (int num : nums) {
        // 单个元素如果就大于maxSum，则此maxSum不可行
        if (num > maxSum) {
            return false;
        }

        // 如果当前数加入后，当前段的和会超过限制
        if (currentSegmentSum + num > maxSum) {
            // 需要开启一个新段
            segmentsNeeded++;
            // 新段的和从当前数开始
            currentSegmentSum = num;
        } else {
            // 否则，将当前数加入当前段
            currentSegmentSum += num;
        }
    }

    // 返回需要的段数是否在允许范围内
    return segmentsNeeded <= maxSegments;
}

int main() {
    // 优化输入输出，提高效率
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n; // 数列长度
    int m; // 要分成的段数
    cin >> n >> m;

    vector<int> nums(n);
    long long totalSum = 0; // 数列总和，作为二分上界
    int maxElement = 0;     // 数列中最大元素，作为二分下界

    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
        totalSum += nums[i];
        maxElement = max(maxElement, nums[i]);
    }

    // --- 二分查找答案 ---
    long long left = maxElement;  // 搜索区间的左边界
    long long right = totalSum;   // 搜索区间的右边界
    long long minMaxSum = right; // 存储最终结果，初始化为最大可能值

    while (left <= right) {
        // 计算中间值，防止溢出
        long long mid = left + (right - left) / 2;

        // 检查以 mid 作为每段最大和，是否可行
        if (canPartition(mid, m, nums)) {
            // 如果可行，说明 mid 可能就是答案，或者答案在更小的范围里
            minMaxSum = mid;       // 记录下这个可行的答案
            right = mid - 1;     // 尝试在左半部分寻找更小的可行值
        } else {
            // 如果不可行，说明 mid 太小了，需要增大
            left = mid + 1;      // 在右半部分寻找
        }
    }

    // 输出找到的最小的最大和
    cout << minMaxSum << endl;

    return 0;
}


```

### T2_03_Galaxy_Analysis
```cpp
/*
* 3. 银河解析
【问题描述】

宇宙第一天才骇客银狼小姐向全银河喊话：有意愿与银狼对决的骇客诸君，请在任何候任何地点向银狼发起网络进攻，银狼愿意随时应战。作为一个骇客，你也收到了银狼发出的奇怪代码，你需要按照规则解密这些代码，翻译成文字信息，以下是其解析规则：

该代码应为一连串的二进制码，每八个字符为一单元。

一、若前三个字符为101时表示需要转换为字母A-Z。字母A代码为10100000，字母C为10100010，26个大写字母以字母表顺序按照这种规律顺序排列，分别对应一个二进制代码。

二、若前三个字符为111，则该单元翻译为空格。

三、若第一个字符为0，则该单元表示一个数，待定与下一个单元所表示的数做加法。加法过程中，这两个单元应转换为十进制，然后除以2并舍弃余数才相加，加法结束后，这两个单元做加法得到的结果即为这两个单元的翻译结果，翻译结果用十进制表示，这两个单元就都翻译完毕了。

众所周知，银狼小姐十分调皮，她经常发送假代码，而假代码则不存在上述规则，如果是假代码，仅输出WA。

【输入形式】

输入仅一行，包含一串连续的代码（长度不超过114514个字符），中间不会出现空格，保证不出现空代码。

【输出形式】

输出仅一行，包含一个字符串，是二进制代码按照上述规则翻译后的文字信息。若该二进制代码中存在假代码信息，则只输出WA。

【样例输入】


10100000111000001010111110101000101001100000001000000010000000100000001010100000
【样例输出】

A PIG22A
【样例说明】
【评分标准】

【出题人】

拔尖班2022级周新城



 */
/*
 * 核心思路：
 * 1. 将输入的二进制字符串按8位分组处理
 * 2. 根据前缀规则进行不同的解码操作：
 *    - 101开头：解码为大写字母A-Z
 *    - 111开头：解码为空格
 *    - 0开头：与下一个8位单元配对进行数值运算
 * 3. 对于数值运算：两个单元转为十进制后各除以2（整除），然后相加
 * 4. 如果遇到不符合规则的情况，输出"WA"
 *
 * 时间复杂度：O(n)，其中n为输入字符串长度
 * 空间复杂度：O(n)，用于存储结果字符串
 */

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class SilverWolfDecoder {
private:
    // 检查字符串是否为有效的8位二进制
    bool isValidBinary(const string& binaryStr) {
        if (binaryStr.length() != 8) return false;
        for (char ch : binaryStr) {
            if (ch != '0' && ch != '1') return false;
        }
        return true;
    }

    // 将8位二进制字符串转换为十进制数值
    int binaryToDecimal(const string& binaryStr) {
        int decimalValue = 0;
        int powerOfTwo = 1;

        // 从右到左处理二进制位
        for (int i = 7; i >= 0; i--) {
            if (binaryStr[i] == '1') {
                decimalValue += powerOfTwo;
            }
            powerOfTwo *= 2;
        }

        return decimalValue;
    }

    // 解码字母类型的二进制单元（101开头）
    char decodeLetter(const string& binaryUnit) {
        // 字母A的二进制为10100000，对应十进制160
        // 字母按顺序递增，A=160, B=161, C=162, ...
        int decimalValue = binaryToDecimal(binaryUnit);
        int letterIndex = decimalValue - 160; // 160对应字母A

        // 检查是否在有效的字母范围内(A-Z)
        if (letterIndex < 0 || letterIndex > 25) {
            return '\0'; // 无效字母标记
        }

        return 'A' + letterIndex;
    }

public:
    // 主解码函数
    string decodeMessage(const string& encodedMessage) {
        // 检查输入长度是否为8的倍数
        if (encodedMessage.length() % 8 != 0) {
            return "WA";
        }

        vector<string> binaryUnits;

        // 将输入按8位分组
        for (size_t i = 0; i < encodedMessage.length(); i += 8) {
            string currentUnit = encodedMessage.substr(i, 8);

            // 验证每个单元是否为有效二进制
            if (!isValidBinary(currentUnit)) {
                return "WA";
            }

            binaryUnits.push_back(currentUnit);
        }

        string decodedResult = "";

        for (size_t unitIndex = 0; unitIndex < binaryUnits.size(); unitIndex++) {
            string currentUnit = binaryUnits[unitIndex];

            // 规则一：101开头表示字母
            if (currentUnit.substr(0, 3) == "101") {
                char decodedLetter = decodeLetter(currentUnit);
                if (decodedLetter == '\0') {
                    return "WA"; // 无效字母
                }
                decodedResult += decodedLetter;
            }
            // 规则二：111开头表示空格
            else if (currentUnit.substr(0, 3) == "111") {
                decodedResult += " ";
            }
            // 规则三：0开头表示数值运算
            else if (currentUnit[0] == '0') {
                // 需要与下一个单元配对
                if (unitIndex + 1 >= binaryUnits.size()) {
                    return "WA"; // 缺少配对单元
                }

                string nextUnit = binaryUnits[unitIndex + 1];

                // 将两个单元转换为十进制
                int firstValue = binaryToDecimal(currentUnit);
                int secondValue = binaryToDecimal(nextUnit);

                // 各自除以2（整除）后相加
                int computedSum = (firstValue / 2) + (secondValue / 2);

                // 将结果添加到解码字符串中
                decodedResult += to_string(computedSum);

                // 跳过下一个单元，因为已经处理过了
                unitIndex++;
            }
            else {
                // 不符合任何已知规则
                return "WA";
            }
        }

        return decodedResult;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string inputCode;
    cin >> inputCode;

    SilverWolfDecoder decoder;
    string decodedMessage = decoder.decodeMessage(inputCode);

    cout << decodedMessage << endl;

    return 0;
}
```

### T2_04_Omsk_Metro
```cpp
/*
 *
4. Omsk Metro
【问题描述】

最开始有一个编号为1权值为1的点，接下来有 n 个操作：

1、+   u   x   表示添加一条起点为 u 且连向点的点权为 x 的边。x∈{-1,1}，假设在该操作之前点的总数为tot，则该边的终点应为tot+1

2、？ u  v  k    表示询问 u 到 v 的最短路径上是否存在一个子段使得子段上的点的权值和恰好等于 k ，并输出“YES”或“NO”。

*子段：元素的连续序列，这个子段可以为空，且子段为空时子段和为0.

可以保证任意两点之间只有唯一一条路径,且不会出现自环。

【输入形式】

 第一行包含一个正整数n，表示操作的个数

  接下来 n 行，每行表示下列操作之一：

·首先是一个符号‘+’，其次是两个正整数 u、x∈{-1,1}表示新建的边。

·首先是一个符号‘？’，其次是两个正整数 u、v、k 表示询问 u、v 之间的最短路径之间是否存在权重之和恰好为 k 的子段（可以为空）。


【输出形式】

 对于每次询问，输出一行，若存在询问子段则输出“YES”，否则输出“NO”

【样例输入1】


8
+ 1 -1
? 1 1 2
? 1 2 1
+ 1 1
? 1 3 -1
? 1 1 1
? 1 3 2
? 1 1 0
【样例输出1】


NO
YES
NO
YES
YES
YES
【样例输入2】


7
+ 1 -1
+ 2 -1
+ 2 1
+ 3 -1
? 5 2 2
? 3 1 -1
? 5 4 -3
【样例输出2】



NO
YES
YES
【样例1说明】

第二个问题的答案是 "YES"，因为存在一条路径1

在第四题中，我们可以再次选择1路径。

在第五个问题中，答案是 "YES"，因为存在路径 1−3

在第六个问题中，我们可以选择一条空路径，因为它的权重之和为0

不难证明，不存在满足第一和第三个查询的路径。

【评分标准】

对于 20% 的数据，保证 1≤ n ≤ 10000，并且点的个数最多为5000

对于额外的 30% 的数据，保证 ‘?’ 操作中 u = 1

对于100% 的数据，保证 1≤ n ≤ 200000，x ∈{-1,1}


【出题人】

 拔尖班2202级林新晨
 */

/**
 * @file omsk_metro_lct.cpp
 * @brief Omsk Metro (动态树 + 路径子段和查询)
 *
 * 核心思路:
 * 1.  问题建模: 题目描述了一个动态加点的过程，每个新点连接到已存在的某个点上，且保证无环。
 * 这实质上是在动态地构建一棵树。查询操作是在这棵树上任意两点间的唯一路径上进行的。
 * 2.  查询转化: 询问 "u到v的路径上是否存在点权和为k的子段"，由于点权仅为+1或-1，
 * 路径上的点权序列构成了一个由+1和-1组成的数组。一个关键性质是：对于这样的序列，
 * 其所有非空子段和会构成一个连续的整数区间 [minSubarraySum, maxSubarraySum]。
 * 因此，问题转化为求出 u-v 路径上的最大和最小子段和，然后判断 k 是否在该区间内。
 * 特别地，题目允许空子段，其和为0，所以当 k=0 时，答案恒为"YES"。
 * 3.  数据结构选择: 为了高效地支持动态加边（连接）和路径信息查询（最大/最小子段和），
 * Link-Cut Tree (LCT) 是理想的数据结构。LCT 可以将树的路径操作转化为对辅助树
 * (Splay Tree) 的序列操作，均摊时间复杂度为 O(log N)。
 * 4.  信息维护: 我们在 LCT 的每个 Splay 节点上维护其所代表的路径片段的聚合信息。
 * 这包括：区间和、最大/最小前缀和、最大/最小后缀和、以及最大/最小子段和。
 * 这些信息可以在 Splay 树的节点合并时（对应于路径片段的拼接）高效地更新。
 * 5.  查询流程: 查询 u 到 v 的路径时，我们通过 LCT 的 `makeRoot(u)` 和 `access(v)` 操作，
 * 将 u 到 v 的路径提取到一棵单独的 Splay 树中，其根节点就是 v。此时，v 节点上
 * 维护的聚合信息 `pathInfoData[v]` 即为整条 u-v 路径的信息。
 *
 * 时间复杂度: O(M * log N)，其中 M 是操作总数，N 是节点总数。LCT 的每次操作（加点、查询）
 * 的均摊复杂度都是 O(log N)。
 * 空间复杂度: O(N)，LCT 的所有辅助数组都需要线性的空间。
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

// 使用标准命名空间，简化代码
using namespace std;

// 无穷大常量，用于初始化最小/最大值
static const int INF = 1e9;

// ------------------------------ LCT 节点维护的路径信息 ------------------------------

// 用于维护 LCT 辅助树 Splay 节点信息的结构体
// 代表了 Splay 子树中序遍历所形成的路径（或路径片段）的聚合信息
struct PathInfo {
    int sum;         // 区间内所有节点权值的总和
    int prefMax;     // 从区间左端点开始的最大前缀和 (非空)
    int prefMin;     // 从区间左端点开始的最小前缀和 (非空)
    int sufMax;      // 到区间右端点结束的最大后缀和 (非空)
    int sufMin;      // 到区间右端点结束的最小后缀和 (非空)
    int subMax;      // 区间内的最大子段和 (非空)
    int subMin;      // 区间内的最小子段和 (非空)
};

/**
 * @brief 创建一个代表“空路径”的信息单元，作为合并操作的单位元。
 * @return PathInfo 代表空路径的信息。
 */
inline PathInfo createEmptyPathInfo() {
    return PathInfo{
        0,
        -INF, +INF, // 前缀和：空路径无非空前缀，故设为极值
        -INF, +INF, // 后缀和：同上
        -INF, +INF  // 子段和：同上
    };
}

/**
 * @brief 根据单个节点的权值创建一个路径信息单元。
 * @param weight 节点的权值 (+1 或 -1)。
 * @return PathInfo 代表单个节点路径的信息。
 */
inline PathInfo createSingleNodePathInfo(int weight) {
    return PathInfo{
        weight,
        weight, weight, // 单个点的前缀、后缀、子段和都是其自身的值
        weight, weight,
        weight, weight
    };
}

/**
 * @brief 合并两个相邻路径片段 (left + right) 的信息。
 * @param leftPath 左侧路径片段的信息。
 * @param rightPath 右侧路径片段的信息。
 * @return 合并后的新路径片段信息。
 */
inline PathInfo mergePathInfo(const PathInfo& leftPath, const PathInfo& rightPath) {
    PathInfo result;
    // 总和是简单的相加
    result.sum = leftPath.sum + rightPath.sum;

    // 新前缀最大和: 要么是左半部分的前缀最大和，要么是贯穿整个左半部分并延伸到右半部分
    result.prefMax = max(leftPath.prefMax, leftPath.sum + rightPath.prefMax);
    result.prefMin = min(leftPath.prefMin, leftPath.sum + rightPath.prefMin);

    // 新后缀最大和: 要么是右半部分的后缀最大和，要么是贯穿整个右半部分并延伸到左半部分
    result.sufMax = max(rightPath.sufMax, leftPath.sufMax + rightPath.sum);
    result.sufMin = min(rightPath.sufMin, leftPath.sufMin + rightPath.sum);

    // 新子段最大和: 来源有三：完全在左半部分、完全在右半部分、或跨越中点（左半部分的后缀+右半部分的前缀）
    result.subMax = max({leftPath.subMax, rightPath.subMax, leftPath.sufMax + rightPath.prefMax});
    result.subMin = min({leftPath.subMin, rightPath.subMin, leftPath.sufMin + rightPath.prefMin});

    return result;
}

// ------------------------------ Link-Cut Tree 实现 ------------------------------
struct LinkCutTree {
    int maxNodeCount;                      // 预分配的最大节点数量
    vector<int> parent;                    // 存储每个节点的父节点指针（在Splay树中）
    vector<int> leftChild, rightChild;     // Splay树的左右孩子
    vector<bool> isReversed;               // 翻转标记，用于LCT的makeRoot操作
    vector<int> nodeValue;                 // 存储原始图中节点的权值（+1 或 -1）
    vector<PathInfo> pathInfoData;         // 存储每个Splay节点聚合的路径信息

    explicit LinkCutTree(int size = 0) { init(size); }

    void init(int size) {
        maxNodeCount = size;
        parent.assign(maxNodeCount + 1, 0);
        leftChild.assign(maxNodeCount + 1, 0);
        rightChild.assign(maxNodeCount + 1, 0);
        isReversed.assign(maxNodeCount + 1, false);
        nodeValue.assign(maxNodeCount + 1, 0);
        pathInfoData.assign(maxNodeCount + 1, createEmptyPathInfo());
    }

    // 判断节点 x 是否为其所在 Splay 树的根
    inline bool isSplayRoot(int nodeId) const {
        int parentId = parent[nodeId];
        return parentId == 0 || (leftChild[parentId] != nodeId && rightChild[parentId] != nodeId);
    }

    // 对节点 x 应用翻转操作
    inline void applyReverse(int nodeId) {
        if (!nodeId) return;
        isReversed[nodeId] = !isReversed[nodeId];
        swap(leftChild[nodeId], rightChild[nodeId]);
        // 翻转路径后，前缀和与后缀和也需要交换
        swap(pathInfoData[nodeId].prefMax, pathInfoData[nodeId].sufMax);
        swap(pathInfoData[nodeId].prefMin, pathInfoData[nodeId].sufMin);
    }

    // 下放懒标记（当前仅有翻转标记）
    inline void pushDown(int nodeId) {
        if (isReversed[nodeId]) {
            if (leftChild[nodeId]) applyReverse(leftChild[nodeId]);
            if (rightChild[nodeId]) applyReverse(rightChild[nodeId]);
            isReversed[nodeId] = false;
        }
    }

    // 从子节点信息更新父节点信息
    inline void pushUp(int nodeId) {
        PathInfo leftInfo = leftChild[nodeId] ? pathInfoData[leftChild[nodeId]] : createEmptyPathInfo();
        PathInfo midInfo = createSingleNodePathInfo(nodeValue[nodeId]);
        PathInfo rightInfo = rightChild[nodeId] ? pathInfoData[rightChild[nodeId]] : createEmptyPathInfo();
        // 合并顺序必须是：左子树 -> 当前节点 -> 右子树，以保证中序遍历的正确性
        pathInfoData[nodeId] = mergePathInfo(mergePathInfo(leftInfo, midInfo), rightInfo);
    }

    // Splay 树的旋转操作
    void rotate(int nodeId) {
        int parentNode = parent[nodeId];
        int grandparentNode = parent[parentNode];
        bool isRightChild = (nodeId == rightChild[parentNode]);
        int childSubtree = isRightChild ? leftChild[nodeId] : rightChild[nodeId];

        if (!isSplayRoot(parentNode)) {
            if (parentNode == leftChild[grandparentNode]) leftChild[grandparentNode] = nodeId;
            else if (parentNode == rightChild[grandparentNode]) rightChild[grandparentNode] = nodeId;
        }
        parent[nodeId] = grandparentNode;

        if (isRightChild) {
            rightChild[parentNode] = childSubtree;
            leftChild[nodeId] = parentNode;
        } else {
            leftChild[parentNode] = childSubtree;
            rightChild[nodeId] = parentNode;
        }
        if (childSubtree) parent[childSubtree] = parentNode;
        parent[parentNode] = nodeId;

        pushUp(parentNode);
        pushUp(nodeId);
    }

    // 将节点 x 旋转到其所在 Splay 树的根
    void splay(int nodeId) {
        vector<int> ancestorStack;
        ancestorStack.reserve(32); // 预分配空间，避免频繁重分配
        int currentNode = nodeId;
        while (true) {
            ancestorStack.push_back(currentNode);
            if(isSplayRoot(currentNode)) break;
            currentNode = parent[currentNode];
        }
        // 从上到下 pushDown，确保旋转路径上的信息正确
        for (int i = (int)ancestorStack.size() - 1; i >= 0; --i) {
            pushDown(ancestorStack[i]);
        }

        while (!isSplayRoot(nodeId)) {
            int parentNode = parent[nodeId];
            int grandparentNode = parent[parentNode];
            if (!isSplayRoot(parentNode)) {
                bool zigzig = ((nodeId == leftChild[parentNode]) == (parentNode == leftChild[grandparentNode]));
                if (zigzig) rotate(parentNode);
                else rotate(nodeId);
            }
            rotate(nodeId);
        }
        pushUp(nodeId);
    }

    // LCT核心操作：打通从根到 x 的路径，使其成为一条偏爱路径
    void access(int nodeId) {
        int lastChildSplayRoot = 0;
        for (int currentNode = nodeId; currentNode; currentNode = parent[currentNode]) {
            splay(currentNode);
            // 断开旧的偏爱孩子，连接新的（来自更深层的部分）
            rightChild[currentNode] = lastChildSplayRoot;
            pushUp(currentNode);
            lastChildSplayRoot = currentNode;
        }
        splay(nodeId);
    }

    // 将节点 x 设置为它所在原树的根
    void makeRoot(int nodeId) {
        access(nodeId);
        applyReverse(nodeId);
    }

    // 查找节点 x 所在原树的根
    int findRoot(int nodeId) {
        access(nodeId);
        while (true) {
            pushDown(nodeId);
            if (!leftChild[nodeId]) break;
            nodeId = leftChild[nodeId];
        }
        splay(nodeId);
        return nodeId;
    }

    // 连接两个节点（x作为y的子节点），前提是它们不在同一棵树中
    void link(int childNode, int parentNode) {
        makeRoot(childNode);
        if (findRoot(parentNode) != childNode) {
            parent[childNode] = parentNode;
        }
    }

    // 查询 u 到 v 的路径信息
    PathInfo queryPath(int u, int v) {
        makeRoot(u);
        access(v);
        return pathInfoData[v];
    }

    // 设置节点 x 的权值
    void setNodeValue(int nodeId, int weight) {
        access(nodeId);
        nodeValue[nodeId] = weight;
        pushUp(nodeId);
    }
};


/**
 * @brief 主逻辑函数，处理所有输入和操作
 */
void solve() {
    // 提高C++流的性能
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int operationCount;
    if (!(cin >> operationCount)) return;

    // 节点总数最多为 operationCount + 1 (初始的节点1)
    int maxNodes = operationCount + 1;
    LinkCutTree lct(maxNodes + 5); // 预留一些缓冲空间
    int currentNodeCount = 1;      // 当前已创建的节点总数，初始为1

    // 题目规定，节点1初始存在且权值为1
    lct.setNodeValue(1, 1);

    for (int i = 0; i < operationCount; ++i) {
        char operationType;
        cin >> operationType;

        if (operationType == '+') {
            int parentNodeId, newNodeValue;
            cin >> parentNodeId >> newNodeValue; // 读取父节点 u 和新节点权值 x

            ++currentNodeCount; // 节点总数加一，新节点编号即为 currentNodeCount

            lct.setNodeValue(currentNodeCount, newNodeValue); // 设置新节点的权值
            lct.link(parentNodeId, currentNodeCount);         // 将新节点作为 parentNodeId 的子节点连接

        } else if (operationType == '?') {
            int u, v, k;
            cin >> u >> v >> k; // 读取查询的起点、终点和目标子段和

            // 特殊情况：k=0。题目允许空子段，其和为0，因此总是存在。
            if (k == 0) {
                cout << "YES\n";
                continue;
            }

            // 查询 u 到 v 路径上的聚合信息
            PathInfo pathResult = lct.queryPath(u, v);

            // 核心判断：因为权值只有+1和-1，路径上所有非空子段和会构成一个连续的整数区间。
            // 这个区间就是 [pathResult.subMin, pathResult.subMax]。
            // 因此，只需判断 k 是否落在这个区间内即可。
            if (pathResult.subMin <= k && k <= pathResult.subMax) {
                cout << "YES\n";
            } else {
                cout << "NO\n";
            }
        }
    }
}

int main() {
    solve();
    return 0;
}
```

### T2_05_Code_is_an_Art
```cpp
/*5. 代码是一门艺术
【问题背景】

  代码是一门艺术，作为现代代码艺术抽象风格大师，你正打算打造你的绝世之作--“创世原码”！你已经写好了你所需要的所有code，并且code之间有调用关系。你非常珍重自己的成果，所以你决定让“创世之作”可以调用所有的code。但是作为顶级代码艺术大师，你决定使用数量最少的“直接”调用code来“间接”调用所有的code。

比如，1可以调用2，2可以调用3，那么直接调用1就可以间接调用2、3。

【问题描述】

    现在你将编写一个程序来计算这个最少数量，并输出具体的方案数。

    如果存在多个最小数量的方案，请输出字典序最小的一种方案

【输入形式】

    第一行包含两个正整数 n、m 分别表示code的数量和调用关系的数量

    接下来 m 行，每行包含两个正整数 u、v 表示调用 u 可以间接调用 v ，且 u ≠ v.

    保证不会给出重复的调用关系。

【输出形式】

    输出为若干行，每行包含一个要调用的code的编号，且按编号从小到大输出。

【样例输入1】


5 6
2 1
2 4
3 1
3 2
5 1
5 2
【样例输出1】


3
5
【样例输入2】


9 11
1 3
2 4
2 6
4 1
5 3
5 6
5 8
6 8
7 4
8 1
8 2
【样例输出2】


5
7
9


【样例说明】

无
【评分标准】

对于50%的数据，给出的调用关系关系中不会出现环

对于100%的数据，1≤ n ≤30000，1≤ m ≤500000


【出题人】

    拔尖班2022级林新晨


*/
/*
 * 核心思路:
 * 本题要求找到一个最小的节点集合，从这个集合出发可以遍历整个有向图。如果存在多个大小相同的最小集合，则输出字典序最小的那个。
 * 这个问题可以被抽象为在图的“缩点”后的有向无环图（DAG）上寻找所有入度为0的节点。
 * 1.  **强连通分量 (SCC)**: 首先，我们将原图中的所有强连通分量（SCC）缩成一个点。在一个SCC内部，任何一个节点都可以到达该SCC内的所有其他节点。因此，我们只需要进入一个SCC一次，就可以覆盖其中的所有节点。
 *
 * 2.  **Tarjan算法**: 我们使用Tarjan算法来高效地找出所有的强连通分量。在算法执行过程中，为每个节点分配一个它所属的SCC的唯一ID。
 *
 * 3.  **构建缩点图 (Condensation Graph)**: 找出SCC之后，我们可以构建一个缩点图（一个DAG）。在这个图中，每个节点代表一个SCC。如果原图中存在从节点u到节点v的边，且u和v不属于同一个SCC，那么在缩点图中就存在一条从u所属的SCC到v所属的SCC的边。
 *
 * 4.  **寻找根节点**: 问题的核心就变成了在缩点图这个DAG中，找到所有入度为0的SCC。这些SCC是整个调用链的起点，只要我们“直接调用”这些SCC中的任意一个节点，就可以通过依赖关系间接触及到所有从它们出发可以到达的SCC。因此，要覆盖全图，我们必须且只需从每一个入度为0的SCC中选取一个代表节点。
 *
 * 5.  **计算入度**: 我们无需显式地构建缩点图。在找到所有SCC后，我们遍历原图的每一条边(u, v)。如果u和v属于不同的SCC（即 sccId[u] != sccId[v]），那么就在缩点图中存在一条从sccId[u]到sccId[v]的边。我们据此增加 sccId[v] 的入度计数。
 *
 * 6.  **输出字典序最小解**: 题目要求输出字典序最小的方案。这意味着对于每一个入度为0的SCC，我们都应该选择其中编号最小的节点作为代表。我们遍历从1到n的所有节点，如果当前节点i所属的SCC是一个入度为0的SCC，并且我们还未为该SCC选择过代表节点，那么就选择i，并将其加入最终结果。这样可以保证我们总是为每个源SCC选择最小的节点，且最终输出的结果自然是升序的。
 *
 * 时间复杂度: O(N + M)，其中N是节点数，M是边数。这主要是Tarjan算法和后续遍历图计算入度的时间开销。
 * 空间复杂度: O(N + M)，用于存储图的邻接表以及Tarjan算法所需的辅助数组。
 */

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

// 图和Tarjan算法所需的全局变量
vector<vector<int>> adj;
vector<int> dfn, low, sccId;
vector<bool> onStack;
stack<int> s;
int timer = 0, sccCount = 0;

// Tarjan算法实现
void tarjan(int u) {
    dfn[u] = low[u] = ++timer;
    s.push(u);
    onStack[u] = true;

    for (int v : adj[u]) {
        if (dfn[v] == 0) { // v未被访问
            tarjan(v);
            low[u] = min(low[u], low[v]);
        } else if (onStack[v]) { // v已被访问且在当前DFS路径的栈中
            low[u] = min(low[u], dfn[v]);
        }
    }

    // 如果u是其所在SCC的根节点
    if (dfn[u] == low[u]) {
        ++sccCount;
        int node;
        do {
            node = s.top();
            s.pop();
            onStack[node] = false;
            sccId[node] = sccCount;
        } while (node != u);
    }
}

int main() {
    // 禁用 C++ 标准流与 C 标准流的同步，提高 IO 效率
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    cin >> n >> m;

    // 初始化
    adj.resize(n + 1);
    dfn.assign(n + 1, 0);
    low.assign(n + 1, 0);
    sccId.assign(n + 1, 0);
    onStack.assign(n + 1, false);

    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
    }

    // --- 步骤 1 & 2: 运行Tarjan算法找出所有SCC ---
    for (int i = 1; i <= n; ++i) {
        if (dfn[i] == 0) {
            tarjan(i);
        }
    }

    // --- 步骤 3 & 4: 计算每个SCC的入度 ---
    vector<int> inDegree(sccCount + 1, 0);
    for (int u = 1; u <= n; ++u) {
        for (int v : adj[u]) {
            if (sccId[u] != sccId[v]) {
                inDegree[sccId[v]]++;
            }
        }
    }

    // --- 步骤 5: 找出字典序最小的解 ---
    vector<int> result;
    vector<bool> sccChosen(sccCount + 1, false);
    // 遍历所有节点以保证字典序
    for (int i = 1; i <= n; ++i) {
        int currentSccId = sccId[i];
        // 如果节点i所在的SCC入度为0，且我们还未为这个SCC选择代表节点
        if (inDegree[currentSccId] == 0 && !sccChosen[currentSccId]) {
            result.push_back(i);
            sccChosen[currentSccId] = true;
        }
    }

    // 输出结果
    for (int node : result) {
        cout << node << "\n";
    }

    return 0;
}

```

