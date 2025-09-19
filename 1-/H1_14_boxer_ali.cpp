// Original Filename: 14.拳王阿里.cpp
#include<iostream>
#include<cstring>
#include<cmath>
using namespace std;
string days[7] = {"monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday"};

int get_day(string str)
{
	for (int i = 0; i < 7; i++)
	{
		if (days[i] == str)
			return i + 1;
	}
	return -1;
}

int main()
{
	int t;
	cin >> t;
	while (t--)
	{
		string a, b;
		int d_begin, d_end;
		int l, r;
		int sum = 0, ans = 0;
		cin >> a >> b >> l >> r;
		d_begin = get_day(a);
		d_end = get_day(b);
		int temp = d_end - d_begin + 1;
		if (d_begin > d_end) temp += 7;
		for (int j = l; j <= r; j++)
		{
			if (j % 7 == temp % 7)
			{
				sum += 1;
				ans = j;
			}
		}
		if (sum == 0) cout << "impossible" << endl;
		if (sum == 1) cout << ans << endl;
		if (sum > 1) cout << "many" << endl;
	}
	return 0;
}
/*
14. 拳王阿里
【问题描述】

	   阿里是上个世纪美国最著名的拳击手，阿里在20年的时间里多次获得重量级拳王称号。不过不幸的是，他在之后患上了帕金森氏病。他参加了许多比赛，多到连自己都数不清了。
	有这么一段时间，他总是参加各种不同的比赛，以至于他自己也不知道从第一场比赛开始到最后一场比赛结束到底用了多长时间。他只记得比赛的第一天是星期几（S)，最后一场比赛的最后一天是星期几(E)。他还记得比赛的总天数（包括第一和最后一天）不少于L天，也不多于R天。给定S和E，能否唯一确定参加比赛总天数(包含该段时间内比赛间的间隔天数)？

【输入形式】

	   输入的第一行包含一个整数T，代表测试数据的组数。接下来是 T 组数据。每组数据仅有一行，首先包含两个字符串S和E，然后包含两个整数L和R。

	• 1 ≤ T ≤ 10,000        1 ≤ L ≤ R ≤ 100

	• S, E ∈ {“monday”,“tuesday”,“wednesday”,“thursday”,“friday”,“saturday”, “sunday”}

【输出形式】

	对于每组数据：

如果不存在满足条件的天数，输出一行“impossible”；

如果存在多个满足条件的天数，输出一行“many”；

否则，输出一行，包含一个整数，代表唯一满足条件的天数。

【样例输入】

3
saturday sunday 2 4
monday wednesday 1 20
saturday sunday 3 5
【样例输出】

2
many
impossible
 */