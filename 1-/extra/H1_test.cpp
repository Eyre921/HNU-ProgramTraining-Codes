#include <bits/stdc++.h>
using namespace std;
struct people
{
    int id;
    int score;
    int time;
    bool infected;
    people(int id, int score, int time,bool infected) : id(id), score(score), time(time), infected(infected) {};
};
int n;
int main()
{
    cin >> n;
    int score, time;
    vector<people> peo;
    for (int i = 1; i <= n; i++)
    {
        cin >> time >> scroe;
        peo.emplace_back(i,score,time,false);
    }

    for (auto &p : peo)
        for (auto &p1 :peo)
            if (p.score >=p1.score && p.time <= p1.time && !(p.time == p1.time && p.score == p1.score))
                p1.infected = true;

    for (auto p : peo) if (!p.infected) cout << p.id << endl;
}