//20 二叉树遍历，从前序、中序到后序.cpp
#include <iostream>
#include <string>
using namespace std;

string postorder_result;

void build_postorder(string preorder, string inorder)
{
	if (preorder.empty() || inorder.empty())
	{
		return;
	}

	// 前序遍历的第一个字符是根节点
	char root = preorder[0];

	// 在中序遍历中找到根节点的位置
	int root_pos = inorder.find(root);

	// 划分左右子树
	string left_inorder = inorder.substr(0, root_pos);
	string right_inorder = inorder.substr(root_pos + 1);

	string left_preorder = preorder.substr(1, left_inorder.length());
	string right_preorder = preorder.substr(1 + left_inorder.length());

	// 递归处理左子树
	build_postorder(left_preorder, left_inorder);

	// 递归处理右子树
	build_postorder(right_preorder, right_inorder);

	// 添加根节点到后序遍历结果
	postorder_result += root;
}

int main()
{
	int n;
	while (cin >> n && n != 0)
	{
		string preorder, inorder;
		cin >> preorder >> inorder;

		postorder_result = "";
		build_postorder(preorder, inorder);

		cout << postorder_result << endl;
	}

	return 0;
}
/*
20. 二叉树遍历，从前序、中序到后序
【问题描述】

二叉树是一种非常重要的数据结构，非常多其他数据结构都是基于二叉树的基础演变而来的。对于二叉树，深度遍历有前序、中序以及后序三种遍历方法。

三种基本的遍历思想为：

前序遍历：根结点 ---> 左子树 ---> 右子树

中序遍历：左子树---> 根结点 ---> 右子树

后序遍历：左子树 ---> 右子树 ---> 根结点

比如，求以下二叉树的各种遍历

hhh.png

前序遍历：1  2  4  5  7  8  3  6

中序遍历：4  2  7  5  8  1  3  6

后序遍历：4  7  8  5  2  6  3  1

需要你编写程序解决的问题是：已知一个二叉树的前序遍历和中序遍历的结果，给出该二叉树的后序遍历的结果。

【输入形式】

有多组测试数据，每组测试数据三行，每组测试数据第一行只有一个正整数n，表示二叉树节点的数目，n=0意味着输入结束并且不需要处理。

每组测试数据第二行是二叉树的前序遍历的结果，是一个长度为n的字符串，每个节点由一个字符表示，字符是大小写英文字母及10个数字,不同的节点用不同的字符表示，也即无论前序遍历和中序遍历的字符串中没有重复的字符。

每组测试数据第二行是二叉树的中序遍历的结果，也是一个长度为n的字符串。

40%的测试数据1 ≤ n≤ 10；

30%的测试数据1 ≤ n≤ 20；


20%的测试数据1 ≤ n≤ 40；

10%的测试数据1 ≤ n≤ 62；

【输出形式】

对于每组测试数据，输出一行，是一个长度为n的字符串，表示二叉树后序遍历的结果。

【样例输入】


8
12457836
42758136
4
abcd
abcd
4
abcd
dcba
0
【样例输出】


47852631
dcba
dcba
 */