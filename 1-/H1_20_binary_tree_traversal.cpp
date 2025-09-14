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
