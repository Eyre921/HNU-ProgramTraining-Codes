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
