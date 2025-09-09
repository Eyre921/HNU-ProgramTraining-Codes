// Original Filename: 03.可删除的点.cpp
#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    
    int positive_count = 0; // x坐标为正的点的个数
    int negative_count = 0; // x坐标为负的点的个数
    
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        
        if (x > 0) {
            positive_count++;
        } else {
            negative_count++;
        }
    }
    
    // 如果正数或负数的点最多只有1个，就可以删除使得剩余点在同一边
    if (positive_count <= 1 || negative_count <= 1) {
        cout << "Yes" << endl;
    } else {
        cout << "No" << endl;
    }
    
    return 0;
}
/*【问题描述】

  平面上有n个不同的点，没有在Y轴的点，检查是否存在这样一个点，将其删除后其余所有的点均位于Y轴的同一边。
【输入形式】

  输入第一行包含一个正整数n(2<=n<=105)。

  接下来的n行，包含所有点的坐标，第i行包含两个整数xi和yi(|xi|、|yi|<=109，xi<>0)。
【输出形式】

  如果存在这样的点，则输入"Yes"，否则输出"No"。
【样例输入】

3
1 1
-1 -1
2 -1
*/