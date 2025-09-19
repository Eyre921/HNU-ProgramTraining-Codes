/*
 * 核心思路:
 * 这是一个复杂的表达式求值问题，其核心在于如何解析和计算包含变量、函数调用以及自定义运算符优先级的字符串表达式。
 * 本解决方案采用经典的双栈法（一个操作数栈，一个操作符栈）来处理中缀表达式的求值，这是Dijkstra双栈算术表达式求值算法的实现。
 * 为了处理函数调用，我们引入了递归。当解析器遇到一个函数调用（如 F(a, b)）时，它会：
 * 1. 暂停当前表达式的解析。
 * 2. 解析并计算所有传入参数的值（在当前的变量上下文中）。
 * 3. 根据函数定义，结合传入参数、默认参数和0，为函数体创建一个全新的局部变量上下文。
 * 4. 递归调用求值函数 `evaluate`，传入函数体表达式和这个新的局部上下文。
 * 5. 将函数的返回值作为操作数压入操作数栈，然后继续解析原表达式。
 * 通过 `std::string_view` 来传递表达式字符串，可以有效避免在递归调用和字符串处理中产生大量的拷贝开销，提升性能。
 * 整个计算过程使用64位无符号整型 `unsigned long long`，其算术运算会自动对 2^64 取模，符合题目要求。
 *
 * 时间复杂度: O(L_total)
 * L_total 是所有求值操作中“展开后”的总表达式长度。由于函数调用和变量赋值的存在，
 * 每次求值的时间复杂度与当前表达式的结构和长度，以及其中包含的函数体的复杂度有关。
 * 因为函数无相互递归，可以认为时间复杂度与所有相关表达式的总长度成线性关系。
 *
 * 空间复杂度: O(D * L_max)
 * D 是函数调用的最大嵌套深度（在本题中，由于函数不相互调用，D很小），L_max 是单个表达式的最大长度。
 * 空间主要消耗在递归调用 `evaluate` 函数的栈帧上，每个栈帧内部又包含操作数和操作符两个栈。
 */

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include <array>
#include <cctype>
#include <string_view>
#include <stdexcept>
#include <algorithm>

// 使用标准命名空间，简化代码
using namespace std;

// 类型别名，提高代码可读性
using u64 = unsigned long long;
// 变量上下文，存储 a-z 的值
using VarContext = array<u64, 26>;

// 函数定义结构体
struct Function {
    vector<string> defaultParams; // 默认参数表达式
    string body;                  // 函数体表达式
};

// 全局状态：主表达式、全局变量、函数定义
string mainExpression;
VarContext globalVars;
unordered_map<char, Function> functions;

// 前向声明核心求值函数，因为它会被递归调用
u64 evaluate(string_view expr, const VarContext& context);

/**
 * @brief 快速幂函数，用于计算 base^exp。
 * 所有计算均在 unsigned long long 类型下进行，自动实现对 2^64 的取模。
 * @param base 底数
 * @param exp 指数
 * @return 计算结果
 */
u64 qpow(u64 base, u64 exp) {
    u64 res = 1;
    while (exp > 0) {
        if (exp % 2 == 1) res *= base;
        base *= base;
        exp /= 2;
    }
    return res;
}

// 定义运算符的优先级
const unordered_map<char, int> precedence = {
    {'+', 1}, {'-', 1}, {'*', 2}, {'^', 3}
};

/**
 * @brief 从操作符栈弹出一个操作符，并从操作数栈弹出两个操作数，进行计算后将结果压回操作数栈。
 * @param values 操作数栈
 * @param ops 操作符栈
 */
void applyOp(stack<u64>& values, stack<char>& ops) {
    char op = ops.top();
    ops.pop();

    // 根据题目保证表达式合法，这里不需要检查操作数数量
    u64 val2 = values.top();
    values.pop();
    u64 val1 = values.top();
    values.pop();

    switch (op) {
        case '+': values.push(val1 + val2); break;
        case '-': values.push(val1 - val2); break;
        case '*': values.push(val1 * val2); break;
        case '^': values.push(qpow(val1, val2)); break;
    }
}


/**
 * @brief 核心求值函数，递归地计算表达式的值。
 * @param expr 要求值的表达式 (使用 string_view 避免不必要的拷贝)
 * @param context 当前的变量上下文 (a-z 的值)
 * @return 表达式的计算结果
 */
u64 evaluate(string_view expr, const VarContext& context) {
    stack<u64> values; // 操作数栈
    stack<char> ops;   // 操作符栈

    for (size_t i = 0; i < expr.length(); ++i) {
        // 跳过所有空白字符
        if (isspace(expr[i])) {
            continue;
        }

        // 解析非负整数
        if (isdigit(expr[i])) {
            u64 num = 0;
            size_t j = i;
            while (j < expr.length() && isdigit(expr[j])) {
                num = num * 10 + (expr[j] - '0');
                j++;
            }
            values.push(num);
            i = j - 1; // 外层循环会i++，所以这里要-1
        }
        // 解析变量（小写字母）
        else if (islower(expr[i])) {
            values.push(context[expr[i] - 'a']);
        }
        // 解析函数调用（大写字母）
        else if (isupper(expr[i])) {
            char funcName = expr[i];
            i += 2; // 跳过函数名和'('

            vector<string_view> args;
            // 解析函数参数列表
            if (i < expr.length() && expr[i] != ')') {
                size_t argStart = i;
                int parenLevel = 0;
                while (i < expr.length()) {
                    if (expr[i] == '(') parenLevel++;
                    else if (expr[i] == ')') {
                        if (parenLevel == 0) {
                            args.push_back(expr.substr(argStart, i - argStart));
                            break;
                        }
                        parenLevel--;
                    } else if (expr[i] == ',' && parenLevel == 0) {
                        args.push_back(expr.substr(argStart, i - argStart));
                        argStart = i + 1;
                    }
                    i++;
                }
            }

            // 为函数调用创建新的局部变量上下文
            VarContext newContext;
            const auto& funcDef = functions.at(funcName);

            for (int j = 0; j < 26; ++j) {
                if (j < args.size()) {
                    // 传入的参数在调用者的上下文中求值
                    newContext[j] = evaluate(args[j], context);
                } else if (j < funcDef.defaultParams.size()) {
                    // 默认参数也在调用者的上下文中求值
                    newContext[j] = evaluate(funcDef.defaultParams[j], context);
                } else {
                    // 不足的参数用0补充
                    newContext[j] = 0;
                }
            }
            // 递归调用evaluate来计算函数体的值
            values.push(evaluate(funcDef.body, newContext));
        }
        // 左括号直接入栈
        else if (expr[i] == '(') {
            ops.push('(');
        }
        // 右括号，触发括号内表达式的计算
        else if (expr[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                applyOp(values, ops);
            }
            if (!ops.empty()) ops.pop(); // 弹出'('
        }
        // 操作符
        else {
            char currentOp = expr[i];
            // 当栈顶操作符优先级不低于当前操作符时，先计算栈顶操作符
            while (!ops.empty() && ops.top() != '(' && precedence.count(ops.top()) &&
                   precedence.at(ops.top()) >= precedence.at(currentOp)) {
                applyOp(values, ops);
            }
            ops.push(currentOp);
        }
    }

    // 处理栈中剩余的所有操作符
    while (!ops.empty()) {
        applyOp(values, ops);
    }

    // 若表达式为空（例如"")或只有括号，值为0；否则为计算结果
    return values.empty() ? 0 : values.top();
}


/**
 * @brief 辅助函数: 去除字符串首尾的空白字符
 * @param s 输入字符串
 * @return 去除首尾空白后的字符串
 */
string trim(const string& s) {
    auto start = s.find_first_not_of(" \t\n\r");
    if (string::npos == start) return "";
    auto end = s.find_last_not_of(" \t\n\r");
    return s.substr(start, end - start + 1);
}

// 处理单个测试用例的逻辑
void solve() {
    // 为每个测试用例重置全局状态
    mainExpression = "0";
    globalVars.fill(0);
    functions.clear();
    // 初始时，所有函数定义都等价于返回0
    for (char c = 'A'; c <= 'Z'; ++c) {
        functions[c] = {{}, "0"};
    }

    int q;
    cin >> q;
    // 循环处理所有操作
    for (int i = 0; i < q; ++i) {
        int op;
        cin >> op;
        switch(op) {
            case 1: {
                // op=1: 输出当前表达式的值
                cout << evaluate(mainExpression, globalVars) << endl;
                break;
            }
            case 2: {
                // op=2: 修改变量值
                char varName;
                string expr;
                cin >> varName;
                getline(cin, expr); // 读取该行剩余部分作为表达式
                // 先在当前上下文中计算表达式的值
                u64 val = evaluate(trim(expr), globalVars);
                // 再更新变量的值
                globalVars[varName - 'a'] = val;
                break;
            }
            case 3: {
                // op=3: 定义函数
                string def;
                getline(cin, def);
                def = trim(def);

                char funcName = def[0];
                size_t openParen = def.find('(');
                size_t colon = def.find(':');

                // 稳健地找到与第一个'('匹配的')'
                size_t closeParen = 0;
                int parenLevel = 0;
                for (size_t k = openParen; k < colon; ++k) {
                    if (def[k] == '(') parenLevel++;
                    else if (def[k] == ')') parenLevel--;
                    if (parenLevel == 0) {
                        closeParen = k;
                        break;
                    }
                }

                Function func;
                func.body = trim(def.substr(colon + 1));

                string_view params_sv(def);
                params_sv = params_sv.substr(openParen + 1, closeParen - openParen - 1);

                // 解析默认参数列表，同样需要处理参数内嵌套括号的情况
                if (!params_sv.empty()) {
                    size_t start = 0;
                    parenLevel = 0;
                    for (size_t k = 0; k < params_sv.length(); ++k) {
                        if (params_sv[k] == '(') parenLevel++;
                        else if (params_sv[k] == ')') parenLevel--;
                        else if (params_sv[k] == ',' && parenLevel == 0) {
                            func.defaultParams.push_back(trim(string(params_sv.substr(start, k - start))));
                            start = k + 1;
                        }
                    }
                    func.defaultParams.push_back(trim(string(params_sv.substr(start))));
                }
                functions[funcName] = move(func);
                break;
            }
            case 4: {
                // op=4: 修改主表达式
                string expr;
                getline(cin, expr);
                mainExpression = trim(expr);
                break;
            }
        }
    }
}

int main() {
    // 提高C++ IO性能
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int T;
    cin >> T;
    while (T--) {
        solve();
    }

    return 0;
}