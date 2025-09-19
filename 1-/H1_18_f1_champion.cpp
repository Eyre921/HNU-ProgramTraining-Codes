// Original Filename: 18. F1方程式冠军.cpp
#include<iostream>
#include<vector>
#include<map>
#include<algorithm>
using namespace std;
struct drive {
	int score;
	int rank[100];//排位车手人数 1-100
};
struct driver {
	string name;
	drive res;
};
/*******************原始规则排序************************/
bool cmp1(driver a, driver b) {
	if (a.res.score != b.res.score) {
		return a.res.score > b.res.score;
	} else {
		int cnt = 0;
		while (a.res.rank[cnt] == b.res.rank[cnt]) cnt++;
		return a.res.rank[cnt] > b.res.rank[cnt];
	}
}
/*******************可选规则排序************************/
bool cmp2(driver a, driver b) {
	if (a.res.rank[0] != b.res.rank[0]) {
		return a.res.rank[0] > b.res.rank[0];
	} else if (a.res.score != b.res.score) {
		return a.res.score > b.res.score;
	} else {
		int cnt = 1;
		while (a.res.rank[cnt] == b.res.rank[cnt]) cnt++;
		return a.res.rank[cnt] > b.res.rank[cnt];
	}
}
int main() {
	int mark[10] = {25, 18, 15, 12, 10, 8, 6, 4, 2, 1};
	map<string, drive> m;
	vector<driver> res;
	int t;
	cin >> t;
	while (t--) {
		int k;
		cin >> k;
		for (int i = 0; i < k; i++) {
			string temp;
			cin >> temp;
			m[temp].rank[i]++;
			if (i < 10)
				m[temp].score += mark[i];
		}
	}
	for (map<string, drive>::iterator it = m.begin(); it != m.end(); it++) {
		driver temp;
		temp.name = it->first;
		temp.res = it->second;
		res.push_back(temp);
	}
	sort(res.begin(), res.end(), cmp1);
	cout << res[0].name << endl;
	sort(res.begin(), res.end(), cmp2);
	cout << res[0].name << endl;
	return 0;
}
/*
* 18. F1方程式冠军
【问题描述】

  一级方程式F1锦标赛由一系列称为大奖赛的分站赛组成。每一场比赛的车手都根据他们的最后位置获得积分。只有前10名车手按以下顺序获得分数：25、18、15、12、10、8、6、4、2、1。在锦标赛结束时，得分最多的车手是冠军。如果有平分，则冠军是赢的最多的人（即排位第一）。如果还是平分，则选择得到排位第二最多的人，依此类推，直到没有更多的排位进行比较。

  后来又提出了另一个得分制度，其中冠军是赢的最多的。如果有平手，冠军是得分最多的。如果仍然存在平手，则按原来的得分制度进行，即比较第二、第三、第四、...排位的次数。

  在本赛季，你会得到所有比赛的结果，你将根据两个得分系统来分别确定冠军。数据保证两套系统都能得到唯一的冠军。

【输入形式】

  第一行一个整数t（1<=t<=20），t是分站赛的场次数。之后是每个分站赛的最终排位情况，每个的第一行一个整数n(1<=n<=100)表示排位车手人数，之后n行按排位列出车手的名字，排位从第一到最后，车手的名字为长度不超过50的英文字符，大小写区分。
【输出形式】、

  输出为两行，第一行为按照原始规则确定的冠军，第二行是按照可选规则确定的冠军。

【样例输入】

3
3
apple
banana
pear
2
pear
banana
2
apple
banana

【样例输出】

banana
apple

【样例说明】
【评分标准】

 */