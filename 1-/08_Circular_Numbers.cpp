// Original Filename: 08.循环数.cpp
#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool isCycle(const string &str)
{
    vector<int> num(str.length());
    // 将字符串转换为数字数组
    int length = str.length();
    for (int i = 0; i < length; i++)
    {
        num[i] = str[i] - '0';
    }

    // 创建双倍字符串用于循环检查
    string strCycle = str + str;
    vector<int> num2(num.size());

    // 对每个乘数从1到字符串长度进行测试
    for (int i = 1; i <= length; i++)
    {
        // 复制原数组
        for (int j = 0; j < length; j++)
        {
            num2[j] = num[j];
        }

        int carry = 0;
        string result = "";

        // 高精度乘法（从右到左）
        for (int j = length - 1; j >= 0; j--)
        {
            num2[j] = num2[j] * i + carry;
            carry = num2[j] / 10;
            num2[j] = num2[j] % 10;
            // 构建结果字符串（从左到右添加）
        }

        // 将结果数组转换为字符串
        for (int j = 0; j < length; j++)
        {
            result += (char) ('0' + num2[j]);
        }

        // 检查结果是否在双倍字符串中（即是否为循环）
        if (strCycle.find(result) == string::npos)
        {
            return false;
        }
    }

    return true;
}

int main()
{
    string str;
    cin >> str;

    if (isCycle(str))
    {
        cout << "Yes" << endl;
    } else
    {
        cout << "No" << endl;
    }

    return 0;
}
/*
 *
【问题描述】

      循环数是n位长度的整数，当乘以从1到n的任何整数时，产生原始数字的“循环”。也就是说，如果考虑最后一个数字之后的数字“绕”回到第一个数字，两个数字中的数字序列将是相同的，尽管它们可能从不同的位置开始。例如，数字142857是循环的，如下表所示：

        142857 *1 = 142857
        142857 *2 = 285714
        142857 *3 = 428571
        142857 *4 = 571428
        142857 *5 = 714285
        142857 *6 = 857142

       编写一个程序来确定数字是否是循环数。

【输入形式】

       输入一个数，长度在2到60位之间(请注意，前面的零不应该被删除，它们被认为是确定n的大小和计数的一部分，因此，“01”是一个两位数的数字，与“1”是一个一位数的数字不同。) 。

【输出形式】

       对于每个输入，输出一行(Yes或No)标识它是否是循环数。

【样例输入】

142857
【样例输出】

Yes
 */