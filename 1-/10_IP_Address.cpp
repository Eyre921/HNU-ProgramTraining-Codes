// Original Filename: 10.IP地址.cpp
#include<iostream>
#include<string>
#include<stdlib.h>
#include<vector>
using namespace std;

int main()
{
	int n, num = 0;
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		string str[4], tstr = "00000000";
		getline(cin, str[0], '.');
		getline(cin, str[1], '.');
		getline(cin, str[2], '.');
		getline(cin, str[3]); //输入四段ip；
		for (int i = 0; i < 4; i++)
		{
			//atoi函数的作用是 把字符串转换成整型数的一个函数
			int t = atoi(str[i].c_str()); //c_str是string类中的一个函数，功能是返回当前字符串的首字符地址   ps:c_str()返回的是const char *类型
			int j = 7;
			while (t > 0)
			{
				tstr[j] = t % 2 + 48;
				t = t / 2;
				j--;
			}
			for (int i = 0; i < 8; i++)
			{
				if (tstr[i] == '1') num += 1;
			}
			//num = count(tstr.begin(),tstr.end(),'1');
			for (int k = 0; k < 8; k++)
				tstr[k] = '0';
			//cout << num <<endl;
			//tstr.replace(0,8,"00000000"); //下标从0开始的连续8个字符都被"00000000"替代
		}
		cout << num << endl;
		num = 0;
	}
	return 0;
}
