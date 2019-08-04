/**
 *@author PKU LIU,Yong
 *@date 8.4.2019
 */
#include<iostream>
#include<queue>
using namespace std;
typedef struct BitNode {
	char key;//数据域
	struct BitNode* lchild;//左孩子
	struct BitNode* rchild;//右孩子
}Node,*BitTree;
//层序遍历二叉树
void sequence(BitTree& tree) {
	queue<Node*> que;
	if (tree == NULL) {
		return;
	}
	que.push(tree);
	while(!que.empty()) {
		Node* node = que.front();
		que.pop();
		cout << node->key << endl;
		if (node->lchild != NULL) {
			que.push(node->lchild);
		}
		if (node->rchild != NULL) {
			que.push(node->rchild);
		}
	}
}
//先序遍历二叉树
void preorder(BitTree& tree) {
	if (tree != NULL) {
		cout << tree->key << endl;
		preorder(tree->lchild);
		preorder(tree->rchild);
	}
}
//由用户输入创建一颗二叉树
void creat_BitTree(BitTree& tree) {
	char ch; cin >> ch;
	if (ch == '#') {
		return;
	}
	tree = new BitNode();//生成根节点
	tree->key = ch;
	printf("the Lchild of the (%c) is: ", ch);
	creat_BitTree(tree->lchild);
	printf("the Rchild of the (%c) is: ", ch);
	creat_BitTree(tree->rchild);
}
int main() {
	BitTree tree = NULL;//创建根节点的指针
	printf("please input the nodes of the tree, If there isn't any node, please input '#'.\n");
	creat_BitTree(tree);
	//preorder(tree);
	
	sequence(tree);
	system("pause");
	return 0;
}
