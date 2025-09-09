#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main()
{
    long long m, n;
    cin >> m >> n;
    for (long long i = 1; i <= m; i++)
        if (i % n == 0 || to_string(i).find(to_string(n)) != string::npos)
            cout << i << " ";
}
