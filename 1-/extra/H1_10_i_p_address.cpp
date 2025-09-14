#include <iostream>
#include <string>
#include <sstream>
using namespace std;

// 方法1：位运算法 - 最高效
int countOnes1(int n) {
    int count = 0;
    while (n) {
        n &= (n - 1);  // 消除最右边的1
        count++;
    }
    return count;
}

// 方法2：逐位检查法
int countOnes2(int n) {
    int count = 0;
    while (n)
    {
        if (n & 1) count++; //检查最后一位是否为1
        n >>=1;//右移一位
    }
    return count;
}



int main() {
    int T;
    cin >> T;

    while (T--) {
        string ip;
        cin >> ip;

        // 解析IP地址
        stringstream ss(ip);
        string segment;
        int totalOnes = 0;

        while (getline(ss, segment, '.')) {
            int num = stoi(segment);
            totalOnes += countOnes2(num);
        }

        cout << totalOnes << endl;
    }

    return 0;
}