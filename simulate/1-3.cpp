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