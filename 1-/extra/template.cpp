#pragma GCC optimize(2, 3, "Ofast", "inline")

#include <bits/stdc++.h>

using namespace std;

#define endl '\n'

//#define int long long

//#define double long double

using i64 = long long;

using ui64 = unsigned long long;

//using i128 = __int128;

#define inf (int)0x3f3f3f3f3f3f3f3f

#define INF 0x3f3f3f3f3f3f3f3f

#define yn(x) cout << (x ? "yes" : "no") << endl

#define Yn(x) cout << (x ? "Yes" : "No") << endl

#define YN(x) cout << (x ? "YES" : "NO") << endl

#define mem(x, i) memset(x, i, sizeof(x))

#define cinarr(a, n) for (int _ = 1; _ <= n; _++) cin >> a[_]

#define cinstl(a) for (auto& _ : a) cin >> _

#define coutarr(a, n) for (int _ = 1; _ <= n; _++) cout << a[_] << " \n"[_ == n]

#define coutstl(a) for (const auto& _ : a) cout << _ << ' '; cout << endl

#define all(x) (x).begin(), (x).end()

#define md(x) (((x) % mod + mod) % mod)

#define ls (s << 1)

#define rs (s << 1 | 1)

#define ft first

#define se second

#define pii pair<int, int>

#ifdef DEBUG

#include "debug.h"

#else

#define dbg(...) void(0)

#endif


const int N = 1000 + 5;

const int M = N * 40;

const int mod = 998244353;
