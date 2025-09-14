#include <iostream>
#include <vector>
#include <cmath> // 包含cmath以使用pow函数
#include <iomanip> // 包含iomanip以控制输出精度
using namespace std;
using FType = long double; 
int main() {
    // 使用 long double 以获得更高的精度，这在数值计算中是好习惯
    int n;
    while (cin >> n && n != 0) {
        vector<FType> a(n);
        FType sum = 0;

        // Pass 1: 计算总和
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
            sum += a[i];
        }

        // 一次除法计算平均值
        FType mean = sum / n;

        FType sum_of_squares = 0.0;
        // Pass 2: 计算差的平方和
        for (int i = 0; i < n; ++i) {
            sum_of_squares += (a[i] - mean) * (a[i] - mean);
            // 或者使用 pow 函数: sum_of_squares += pow(a[i] - mean, 2);
        }

        // 一次除法计算方差
        FType var = sum_of_squares / n;

        long long res = var;
        cout << res << endl;

        // 如果你想看看实际的方差值，可以取消下面的注释
        // cout << fixed << setprecision(20) << "Mean: " << mean << endl;
        // cout << fixed << setprecision(20) << "Var:  " << var << endl;
    }

    return 0;
}