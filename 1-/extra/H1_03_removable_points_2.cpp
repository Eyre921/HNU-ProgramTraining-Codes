// 03.可删除的点.cpp
#include <iostream>
using namespace std;

int main()
{
    long long n, pos = 0, neg = 0;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        long long a, b;
        cin >> a >> b;
        if (a > 0) pos++;
        else neg++;
    }
    if (pos <= 1 || neg <= 1) cout << "Yes" << endl;
    else cout << "No" << endl;
}
