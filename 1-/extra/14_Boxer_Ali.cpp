// Original Filename: 14.拳王阿里.cpp
#include<iostream>
#include<cstring>
#include<cmath>
using namespace std;
string days[7] = {"monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday"};

int get_day(string str)
{
    for (int i = 0; i < 7; i++) if (str == days[i]) return i + 1;
    return -1;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    int t;
    cin >> t;

    while (t--)
    {
        string a, b;
        int l, r;
        cin >> a >> b >> l >> r;
        int na = get_day(a), nb = get_day(b);
        int tmp = (nb - na + 1 > 0) ? nb - na + 1 : nb - na + 8;
        int count = 0, ans = 0;
        for (int i = l; i <= r; i++)
        {
            if (i % 7 == tmp % 7)
            {
                count++;
                ans = i;
            }
        }
        if (count == 1) cout << ans << endl;
        else if (count == 0) cout << "impossible" << endl;
        else if (count > 1) cout << "many" << endl;
    }
}
