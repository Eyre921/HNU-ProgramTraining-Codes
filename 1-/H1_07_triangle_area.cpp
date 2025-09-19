// Original Filename: 07.三角形的面积.cpp
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

double calculateTriangleArea(double x1, double y1, double x2, double y2, double x3, double y3)
{
    // 构造向量 AB 和 AC
    double ab_x = x2 - x1;
    double ab_y = y2 - y1;
    double ac_x = x3 - x1;
    double ac_y = y3 - y1;

    // 计算向量叉乘 AB × AC
    double cross_product = ab_x * ac_y - ab_y * ac_x;

    // 三角形面积 = |叉乘结果| / 2
    double area = abs(cross_product) / 2.0;

    return area;
}

int main()
{
    double x1, y1, x2, y2, x3, y3;

    // 设置输出格式：保留6位小数
    cout << fixed << setprecision(6);

    while (cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3)
    {
        // 检查是否为结束条件 (0 0 0 0 0 0)
        if (x1 == 0 && y1 == 0 && x2 == 0 && y2 == 0 && x3 == 0 && y3 == 0)
        {
            break;
        }

        double area = calculateTriangleArea(x1, y1, x2, y2, x3, y3);
        cout << area << endl;
    }

    return 0;
}
/*
7. 三角形的面积
【问题描述】

已知三角形的三个顶点的坐标，求该三角形的面积。

【输入形式】

有多组测试数据。

每组测试数据占一行，6个用空格分隔开的浮点数：x1,y1,x2,y2,x3,y3。表示三角形三个顶点的坐标。

一行6个0（形如0 0 0 0 0 0），表示输入结束，并且不需要处理。

40%的顶点坐标 -10 ≤ xi,yi≤ 10；i=1,2,3

30%的顶点坐标 -100 ≤ xi,yi≤ 100；i=1,2,3

20%的顶点坐标 -1000 ≤ xi,yi≤ 1000；i=1,2,3

10%的顶点坐标 -10000 ≤ xi,yi≤ 10000；i=1,2,3

【输出形式】

对于每组测试数据，输出对应三角形面积，保留小数点后6位。

【样例输入】


1 2 3 4 -2 8
0 0 0 1 1 0
0 0 0 0 0 0
【样例输出】


9.000000
0.500000
 */