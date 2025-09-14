// Original Filename: 09.电能消耗.cpp
#include<iostream>
#include<algorithm>
#include <vector>
using namespace std;
int p1, p2, p3, t1, t2, sum;

int power(int n)
{
    if (n <= t1) return n * p1;
    else if (n > t1 && n <= t2 + t1) return t1 * p1 + (n - t1) * p2;
    else return t1 * p1 + t2 * p2 + (n - t1 - t2) * p3;
}

int main()
{
    int n;
    cin >> n >> p1 >> p2 >> p3 >> t1 >> t2;
    vector<int> l(n), r(n);
    for (int i = 0; i < n; i++)
    {
        cin >> l[i] >> r[i];
        sum += p1 * (r[i] - l[i]);
    }
    for (int i = 1; i < n; i++)sum += power(l[i] - r[i - 1]);
    cout << sum << endl;
}
