// Original Filename: 09.电能消耗.cpp
#include<iostream>
#include<algorithm>
using namespace std;

int power(int n, int p1, int p2, int p3, int t1, int t2)
{
	if (n <= t1)return n * p1;
	else if (n > t1 && n <= t1 + t2)return p1 * t1 + (n - t1) * p2;
	else return p1 * t1 + p2 * t2 + (n - t1 - t2) * p3;
}

int main()
{
	int n, p1, p2, p3, t1, t2;
	cin >> n >> p1 >> p2 >> p3 >> t1 >> t2;
	int l[n], r[n];
	int sum = 0;
	for (int i = 0; i < n; ++i)
	{
		cin >> l[i] >> r[i];
		sum += ((r[i] - l[i]) * p1);
	}
	for (int i = 1; i < n; ++i) sum += power(l[i] - r[i - 1], p1, p2, p3, t1, t2);
	cout << sum;
}
/*
【问题描述】

	  汤姆对他最喜欢的笔记本电脑的耗电量很感兴趣。他的笔记本电脑有三种模式。在正常模式下，笔记本电脑每分钟消耗P1瓦。在汤姆最后一次移动鼠标或触摸键盘后的T1分钟，屏幕保护程序启动，每分钟的功耗变化为P2瓦。最后，从屏幕保护程序启动到T2分钟后，笔记本电脑切换到“睡眠”模式，每分钟消耗P3瓦。 当笔记本电脑处于第二或第三模式时，如果汤姆移动鼠标或触摸键盘，则切换到第一种(正常)模式。 汤姆使用笔记本电脑工作的时间可以分为n个时间间期[l1, r1]、[l2, r2]、...、[ln, rn]。在每个间期，汤姆连续移动鼠标并按下键盘。 在间期之间，汤姆什么都不做。请找出在间期[l1, rn]笔记本电脑的总耗电量。

【输入形式】

	  第一行包含6个整数n、P1、P2、P3、T1、T2(1<=n<=100，0<=P1、P2、P3<=100，1<=T1、T2<=60)。接下来的n行包含了汤姆工作的期间，第i行是两个用空格分隔的整数li和ri(0<=li<=ri<=1440, 当i<n时ri<li+1）, 表示工作期间的开始时间和结束时间。

【输出形式】

	  输出总的耗电量。
【样例输入】

2 8 4 2 5 10
20 30
50 100

【样例输出】

570
 */