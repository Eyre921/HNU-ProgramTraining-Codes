// Original Filename: 11.平均方差.cpp
#include<iostream>
using namespace std;

int main()
{
	while (true)
	{
		int n;
		cin >> n;
		if (n == 0)break; //输入n为0时，结束循环
		double a[n], mean = 0, var = 0; //平均值mean,方差var
		//计算平均值mean
		for (int i = 0; i < n; i++)
		{
			cin >> a[i];
			mean += a[i] / n;
		}
		//计算方差var
		for (int j = 0; j < n; j++)
			var += (a[j] - mean) * (a[j] - mean) / n;
		int result; //整型输出
		result = var;
		cout << result << endl;
	}
	return 0;
}
