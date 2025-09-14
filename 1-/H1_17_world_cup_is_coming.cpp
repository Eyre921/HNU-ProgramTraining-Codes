// Original Filename: 17.世界杯来了.cpp
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <sstream>

using namespace std;

struct TeamStats
{
    int points;
    int goal_diff;
    int goals_for;
    int goals_against;

    TeamStats() : points(0), goal_diff(0), goals_for(0), goals_against(0) {}
};

bool compareTeams(const pair<string, TeamStats> &a, const pair<string, TeamStats> &b)
{
    if (a.second.points != b.second.points)
    {
        return a.second.points > b.second.points;
    }
    if (a.second.goal_diff != b.second.goal_diff)
    {
        return a.second.goal_diff > b.second.goal_diff;
    }
    if (a.second.goals_for != b.second.goals_for)
    {
        return a.second.goals_for > b.second.goals_for;
    }
    return a.first < b.first; // 按字典序排列
}

int main()
{
    int n;
    cin >> n;
    cin.ignore(); // 忽略换行符

    vector<string> teams(n);
    map<string, TeamStats> team_stats;

    // 读取球队名单
    for (int i = 0; i < n; i++)
    {
        getline(cin, teams[i]);
        team_stats[teams[i]] = TeamStats();
    }

    // 读取比赛结果
    int num_matches = n * (n - 1) / 2;
    for (int i = 0; i < num_matches; i++)
    {
        string line;
        getline(cin, line);

        // 解析比赛信息
        size_t space_pos = line.find(' ');
        string teams_part = line.substr(0, space_pos);
        string score_part = line.substr(space_pos + 1);

        // 分离球队名
        size_t dash_pos = teams_part.find('-');
        string team1 = teams_part.substr(0, dash_pos);
        string team2 = teams_part.substr(dash_pos + 1);

        // 分离比分
        size_t colon_pos = score_part.find(':');
        int score1 = stoi(score_part.substr(0, colon_pos));
        int score2 = stoi(score_part.substr(colon_pos + 1));

        // 更新统计数据
        team_stats[team1].goals_for += score1;
        team_stats[team1].goals_against += score2;
        team_stats[team1].goal_diff += (score1 - score2);

        team_stats[team2].goals_for += score2;
        team_stats[team2].goals_against += score1;
        team_stats[team2].goal_diff += (score2 - score1);

        // 更新积分
        if (score1 > score2)
        {
            team_stats[team1].points += 3;
        } else if (score1 < score2)
        {
            team_stats[team2].points += 3;
        } else
        {
            team_stats[team1].points += 1;
            team_stats[team2].points += 1;
        }
    }

    // 创建排序用的vector
    vector<pair<string, TeamStats> > team_pairs;
    for (const auto &team: teams)
    {
        team_pairs.push_back({team, team_stats[team]});
    }

    // 按排名规则排序
    sort(team_pairs.begin(), team_pairs.end(), compareTeams);

    // 取前n/2的球队
    vector<string> qualified_teams;
    for (int i = 0; i < n / 2; i++)
    {
        qualified_teams.push_back(team_pairs[i].first);
    }

    // 按字典序排列输出
    sort(qualified_teams.begin(), qualified_teams.end());

    // 输出结果
    for (const string &team: qualified_teams)
    {
        cout << team << endl;
    }

    return 0;
}
