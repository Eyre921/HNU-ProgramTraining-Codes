// Original Filename: 15.欧洲冠军联赛.cpp
#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
using namespace std;

struct team
{
	int score;
	int goal;
};

struct tteam
{
	string name;
	int score;
	int goal;
};

bool cmp(tteam a, tteam b)
{
	if (a.score != b.score)
		return a.score > b.score;
	else
		return a.goal > b.goal;
}

int main()
{
	int t;
	cin >> t;
	vector<tteam> ranking; //将map数据放到vector中方便sort
	while (t--)
	{
		ranking.clear();
		map<string, team> res;
		for (int i = 0; i < 12; i++)
		{
			int a, b;
			string A, B;
			string vs; //vs.
			cin >> A >> a >> vs >> b >> B;
			if (a > b) res[A].score += 3;
			else if (a == b)
			{
				res[A].score += 1;
				res[B].score += 1;
			} else
			{
				res[B].score += 3;
			}
			res[A].goal += a - b;
			res[B].goal += b - a;
		}
		for (map<string, team>::iterator it = res.begin(); it != res.end(); it++)
		{
			tteam temp;
			temp.name = it->first;
			temp.score = it->second.score;
			temp.goal = it->second.goal;
			ranking.push_back(temp);
		}
		sort(ranking.begin(), ranking.end(), cmp);
		cout << ranking[0].name << " " << ranking[1].name << endl;
	}
	return 0;
}
/*
 *
15. 欧洲冠军联赛
【问题描述】

	   欧洲冠军联赛常被誉为全世界最具影响力的俱乐部级赛事。在比赛的小组赛阶段，欧洲的各个足球俱乐部被分为八个小组，每个小组中四支球队。每个小组中的球队按照如下规则排序：

球队会根据比赛结果获得积分。一场比赛的双方被称为主队和客队。如果其中一方进球数多于另一方，那么进球较多的一方获得3 分，另一方获得0 分。如果双方打成平手，则各得1分。

球队的净胜球数是其进球数减去失球数（不考虑该球队在比赛中作为主队还是客队）。

积分较高的球队排名更加靠前。

如果两支球队积分相同，那么净胜球数较多的球队排名靠前。

	  小组的各队伍进行循环赛，即每两支球队之间进行两场比赛，双方交替作为主队。给定一个小组内12 场比赛的结果，请求出小组的出线队伍：即排名第一和第二的两支球队。

保证答案唯一。

【输入形式】

	  输入的第一行包含一个整数T，代表测试数据的组数。接下来是 T 组数据。

	  每组数据共有12 行，每行描述一场比赛，格式为：“主队队名主队进球数vs. 客队进球数客队队名”，其中“主队队名”和“客队队名”为字符串，“主队进球数”和“客队进球数”为两球队在本场比赛中各自的进球数量。

1 ≤ T ≤ 50

球队队名仅包含小写英文字母

球队队名长度不超过10 个字符

0 ≤ 进球数 ≤ 100

【输出形式】

	   对于每组数据，输出一行，包含两个字符串，代表排名第一和第二的球队的队名。

【样例输入】

2
manutd 8 vs. 2 arsenal
lyon 1 vs. 2 manutd
fcbarca 0 vs. 0 lyon
fcbarca 5 vs. 1 arsenal
manutd 3 vs. 1 fcbarca
arsenal 6 vs. 0 lyon
arsenal 0 vs. 0 manutd
manutd 4 vs. 2 lyon
arsenal 2 vs. 2 fcbarca
lyon 0 vs. 3 fcbarca
lyon 1 vs. 0 arsenal
fcbarca 0 vs. 1 manutd
a 3 vs. 0 b
a 0 vs. 0 c
a 0 vs. 0 d
b 0 vs. 0 a
b 4 vs. 0 c
b 0 vs. 0 d
c 0 vs. 0 a
c 0 vs. 0 b
c 1 vs. 0 d
d 3 vs. 0 a
d 0 vs. 0 b
d 0 vs. 0 c
【样例输出】

manutd fcbarca
d b
【样例说明】

第一组数据：每支球队的积分与净胜球数分别为：

manutd：16 分，净胜球数12。

fcbarca：8 分，净胜球数 4。

arsenal：5 分，净胜球数 −5。

lyon：4 分，净胜球数 −11。

第二组数据：每支球队的积分与净胜球数分别为：

d：7 分，净胜球数 2。

b：7 分，净胜球数 1。

a：7 分，净胜球数 0。

c：7 分，净胜球数 −3。

所有球队的积分相同，但是净胜球数较多的队伍排名更加靠前。
 */