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
