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
