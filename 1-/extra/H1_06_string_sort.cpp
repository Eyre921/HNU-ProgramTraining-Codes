// Original Filename: 06.字符串排序.cpp
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct StringInfo
{
    string str;
    int disorder;
    int originalIndex;
    StringInfo(string &s, int d, int idx) : str(s), disorder(d), originalIndex(idx) {}
};

int CalculateDisorder(const string &s)
{
    int disorder = 0;
    for (int i = 0; i < int(s.length()); i++)
        for (int j = i + 1; j < int(s.length()); j++)
            if (s[j] < s[i]) disorder++;
    return disorder;
}

bool compare(const StringInfo &a, const StringInfo &b)
{
    if (a.disorder != b.disorder) return a.disorder < b.disorder;
    else return a.originalIndex < b.originalIndex;
}

int main()
{
    int m, n;
    cin >> m >> n;
    vector<StringInfo> strings;
    for (int i = 0; i < n; i++)
    {
        string str;
        cin >> str;
        strings.emplace_back(str, CalculateDisorder(str), i);
    }
    sort(strings.begin(), strings.end(), compare);
    for (auto &s: strings) cout << s.str << endl;
}

/*
*6. 字符串排序
【问题描述】

       定义一个字符串的无序度为所有位置后面的字母比该位置的字母小的总数之和。比如"DAABEC''这个字符串的无序度是5，因为D后面有4个位置比它小（AABC），E后面有1个比它小（C），其它位置后面没有比自己小的。" AACEDGG "的无序度为1（E后面有一个D比它小）。" ZWQM "的无序度为6，每个位置后面所有的字母都比它小。
       现在你的任务是给定一些字符串（只由大写字母组成），把他们按照无序度从小到大排序，如果无序度一样，那么就按照输入的相对顺序排序。

【输入形式】

     单组测试数据。
     第一行有两个整数n(0 < n <= 50)和m (0 < m <= 100)，分别表示输入的字符串的长度和字符串的个数。
     接下来m行，每一行包含一个长度为n的字符串，只由大写字母组成。
【输出形式】

    输出m行，表示排序之后的字符串。
【样例输入】

10 6
AACATGAAGG
TTTTGGCCAA
TTTGGCCAAA
GATCAGATTT
CCCGGGGGGA
ATCGATGCAT
【样例输出】

CCCGGGGGGA
AACATGAAGG
GATCAGATTT
ATCGATGCAT
TTTTGGCCAA
TTTGGCCAAA

 */
