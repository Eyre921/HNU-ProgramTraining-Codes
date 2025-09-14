#include <bits/stdc++.h>
using namespace std;
#define MAXSIZE 100001
#define STARTCITY 1
 
typedef struct 
{
    int citynum;
    pair<int, int> routine[MAXSIZE];
} Arabmap;
Arabmap m;
 
bool visited[MAXSIZE];
int leaves[MAXSIZE];
double rates[MAXSIZE];
int p=0;
 
void FindNearCities(vector<int> &v, int city) {
    for (int i=0; i<m.citynum-1; i++) {
        if (m.routine[i].first==city) v.push_back(m.routine[i].second);
        if (m.routine[i].second==city) v.push_back(m.routine[i].first);
    }
}
 
void DFS(int nowcity, int length, double rate) {//length show how long you have walk
    visited[nowcity]=1; length++;
    vector<int> neighbors;
    FindNearCities(neighbors, nowcity);
    int size=(int)neighbors.size();
    if (size==1&&nowcity!=STARTCITY) {//if nowcity is a leaf
        leaves[p]=length; rates[p]=rate;
        p++;
        return;
    }
    //nowcity is not a leaf
    int childnum;
    for (int i=0; i<size; i++) {
        if (nowcity==STARTCITY) childnum=size;
        else childnum=size-1;
        if (!visited[neighbors[i]]) DFS(neighbors[i], length, rate/childnum);
    }
}
 
int main() {
    cin>>m.citynum;
    for (int i=0; i<m.citynum-1; i++) cin>>m.routine[i].first>>m.routine[i].second;
    //you have input infomation to create a map
    DFS(STARTCITY, -1, 1);
    double E=0; p=0;
    while (leaves[p]) {
        E+=leaves[p]*rates[p];
        p++;
    }
    printf("%.7lf", E);//accuicy is less than 10^-6, so you should print the result for 7 bits
    return 0;
}
 