// Original Filename: 21. 计算校验码.cpp
#include <iostream>
#include <string>
using namespace std;

int main()
{
	int t;
	cin >> t;

	while (t--)
	{
		int B;
		string N;
		cin >> B >> N;

		// 计算N各位数字的和
		int digit_sum = 0;
		for (char c: N)
		{
			if (c >= '0' && c <= '9')
			{
				digit_sum += c - '0';
			} else
			{
				// a=10, b=11, c=12, d=13, e=14, f=15
				digit_sum += c - 'a' + 10;
			}
		}

		// 计算需要的校验码
		int remainder = digit_sum % (B - 1);
		int checksum;
		if (remainder == 0)
		{
			checksum = 0;
		} else
		{
			checksum = (B - 1) - remainder;
		}

		// 输出校验码
		if (checksum < 10)
		{
			cout << checksum << endl;
		} else
		{
			// 10->a, 11->b, 12->c, 13->d, 14->e, 15->f
			cout << (char) ('a' + checksum - 10) << endl;
		}
	}

	return 0;
}
