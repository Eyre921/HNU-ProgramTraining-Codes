// Original Filename: 13.错误的里程表.cpp
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

int main()
{
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        //主要思想是给出的数字其实就是一个八进制数字,每位范围12 4567 9, 如果是4-7则减去1, 9减去2, 即得原来的数字
        string s;
        cin >> s;
        int len = s.length();
        for (int i = 0; i < len; i++)
        {
            //转化为真正的八进制数据
            if (s[i] >= '4')
            {
                if (s[i] == '9')
                    s[i] -= 2;
                else
                    s[i] -= 1;
            }
        }
        //下面是两种八进制转化到十进制的方法

        // stringstream ss;
        // ss << oct << s;
        // ss >> a;
        // cout << a << endl;

        int result = strtol(s.c_str(), NULL, 8); //strtol()和c_str()可上网查
        cout << result << endl;
    }
    return 0;
}
