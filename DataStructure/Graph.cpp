/**
 * @author PKU LIU,Yong
 * @date 8/3/2019
 */
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<stack>
#include<queue>
#define MAX 100 //矩阵最大容量
#define INF (~(0x1<<31))//无穷大

using namespace std;

//邻接矩阵
typedef struct _graph {
	int matrix[MAX][MAX];//二维数组表示的矩阵
	char vexs[MAX];//顶点集
	int vexnum;//顶点个数
	int edgnum;//边的个数
}Graph, *pGraph;//邻接矩阵的存储结构，用pGraph指针指向

				//边结构体
typedef struct _edge {
	char start;//入度
	char end;//出度
	int weight;//权值
}Edge;

//由节点名称寻找对应邻接矩阵位置
int getPosition(Graph* gra, char ch) {
	for (int i = 0; i < gra->vexnum; i++) {
		if (gra->vexs[i] == ch) {
			return i;
		}
	}
	return -1;
}
//由用户自己创建一个图
Graph* creatGraph() {
	char c1, c2;
	int v, e;
	int weight, p1, p2;
	printf("please input the number of vex: ");
	scanf("%d", &v);
	printf("\nplease continue to input the number of edge: ");
	scanf("%d", &e);
	if (v < 0 || e < 0 || (e >(v * (v - 1)))) {
		printf("\nInput Error: invalid parameters! please check them! ");
		return NULL;
	}
	Graph* pgra = new Graph();
	pgra->vexnum = v;
	pgra->edgnum = e;
	printf("\nplease initiate your vexs");
	for (int i = 0; i < v; i++) {
		printf("\nvex(%d): ", i);
		cin >> c1;
		pgra->vexs[i] = c1;
	}
	//先自动初始化权值
	for (int i = 0; i < v; i++) {
		for (int j = 0; j < v; j++) {
			if (i == j) {
				pgra->matrix[i][j] = 0;
			}
			else {
				pgra->matrix[i][j] = INF;
			}
		}
	}
	//让用户初始化权值
	printf("\nplease initiate the weights of your edge, 1st char is the start, 3rd is the weight\n");
	for (int i = 0; i < e; i++) {
		printf("egde(%d): ", i);
		cin >> c1 >> c2 >> weight;
		p1 = getPosition(pgra, c1);
		p2 = getPosition(pgra, c2);
		if (p1 != -1 && p2 != -1) {
			pgra->matrix[p1][p2] = weight;                                      /*!!!!!!!.....注意.....！！！！！！*/
			pgra->matrix[p2][p1] = weight;                                     //如果是有向图的话，请把此行注释掉
		}
		else {
			printf("Input Error: Invalid edge\n");
			delete pgra;
			return NULL;
		}
	}
	return pgra;
}
//广度优先遍历图
void BFS(Graph*gra, int v, vector<bool>& visited) {
	int n = gra->vexnum;
	queue<int> que;
	que.push(v);
	while (!que.empty()) {
		int u = que.back();
		que.pop();
		if (!visited[u]) {
			cout << gra->vexs[u] << " " << endl;
			visited[u] = true;
		}
		for (int i = 0; i < n; i++) {
			if ((gra->matrix[u][i] != INF || gra->matrix[i][u] != INF) && u != i) {
				if (!visited[i]) {
					que.push(i);
				}
			}
		}
	}
}
//深度优先遍历图
void DFS(Graph*gra, int v, vector<bool>& visited) {
	cout << gra->vexs[v] << endl;
	visited[v] = true;
	for (int i = 0; i < gra->vexnum; i++) {
		if (!visited[i] && gra->matrix[v][i] != INF && i != v) {
			DFS(gra, i, visited);
		}
	}
}
//最小生成树 普里姆算法
void prim(Graph*gra, char v0) {
	int n = gra->vexnum;
	int k = getPosition(gra, v0);
	vector<vector<int>> close(n, vector<int>(n));//第一个数为从哪个点来，第二个为该点到其余点的直接路径长度
	vector<bool> isvisited(n, true);
	//初始化从k到其他点的路径
	for (int i = 0; i < n; i++) {
		close[k][i] = gra->matrix[k][i];
	}
	isvisited[k] = false;//将k加入u集
	for (int i = 1; i < n; i++) {//对剩余的n-1条路径进行操作
		int min = INF, v;
		for (int j = 0; j < n; j++) {//选择从k到v-u集中最短的一条
			if (isvisited[j] && k != j && gra->matrix[k][j] < INF) {//首先保证从v-u集里选元素，然后再选除自身外的最小的路径
				v = j;
				min = gra->matrix[k][j];
			}
		}
		isvisited[v] = false;
		cout << gra->vexs[k] << " " << gra->vexs[v] << endl;
		k = v;
	}
}
//拓扑排序 - 有向图 邻接矩阵存储
bool topological(Graph*gra, vector<int>& topo) {
	//统计所有节点的入度
	int n = gra->vexnum;
	//int* indegree = new int[gra->vexnum]{0};//存放每个节点的入度
	vector<int> indegree(n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (gra->matrix[j][i] != INF && gra->matrix[j][i] != 0) {
				indegree[i]++;
			}
		}
	}
	stack<int> sta;//存放入度为0的点
	for (int i = 0; i < n; i++) {
		if (indegree[i] == 0) {
			sta.push(i);
		}
	}
	int count = 0;//作为计数器
	while (!sta.empty()) {
		int k = sta.top();
		sta.pop();
		topo[count] = k;//保存到拓扑集合中
		count++;
		//（在逻辑上）删除该节点，统计新的入度为0的点
		for (int i = 0; i < n; i++) {
			if (gra->matrix[k][i] != INF) {//如果之前存在路径
				indegree[i]--;
				if (indegree[i] == 0) {//出现了新的入度为0的点
					sta.push(i);
				}
			}
		}
	}
	if (count == n) {
		return true;
	}
	return false;
}
//关键路径
void critical(Graph*gra) {
	int n = gra->vexnum;
	vector<int> topo(n);
	if (!topological(gra, topo)) return;//说明图中有环路
	vector<int> ve(n);
	vector<int> vl(n, INF);
	//从拓扑集合中依次选取顶点，进行ve的统计
	for (int i = 0; i < n; i++) {
		int k = topo[i];//用k记录当前的节点
						//探索从k到j的路径
		for (int j = 0; j < n; j++) {
			if (gra->matrix[k][j] != INF && gra->matrix[k][j] != 0) {//必须存在路径，不能包括自身
				if (ve[k] + gra->matrix[k][j] > ve[j]) {//选择较大的
					ve[j] = ve[k] + gra->matrix[k][j];
				}
			}
		}
	}
	//从拓扑集合中倒着选取顶点，进行vl的统计
	vl[n - 1] = ve[n - 1];//最后一个工序的ve和vl是相等的
	for (int i = n - 1; i >= 0; i--) {//注意是反过来的
		int k = topo[i];
		//探索从j到k的路径
		for (int j = 0; j < n; j++) {
			if (gra->matrix[j][k] != INF && gra->matrix[j][k] != 0) {
				if (vl[k] - gra->matrix[j][k] < vl[j]) {//选择较小的
					vl[j] = vl[k] - gra->matrix[j][k];
				}
			}
		}
	}
	//每道工序最早最迟判断
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (gra->matrix[i][j] != INF && gra->matrix[i][j] != 0) {//保证有路径
				if (ve[i] == vl[j] - gra->matrix[i][j]) {//最早最迟相同
					cout << gra->vexs[i] << " " << gra->vexs[j] << endl;
				}
			}
		}
	}
}
//弗洛伊德算法求最短路径
void Floyd(Graph* gra) {
	//初始化
	int n = gra->vexnum;
	vector<vector<int>> dist(n, vector<int>(n));//存放两点之间的最短距离长度
	vector<vector<int>> path(n, vector<int>(n));//存放两点的路径信息，第一个是前驱
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			dist[i][j] = gra->matrix[i][j];
			if (gra->matrix[i][j] != INF) {
				path[i][j] = i;
			}
			else {
				path[i][j] = -1;
			}
		}
	}
	for (int k = 0; k < n; k++) {//k是中间插入的元素
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (dist[i][k] + dist[k][j] < dist[i][j]) {//从i经过k到j的一条路径，原先的路径要短
					dist[i][j] = dist[i][k] + dist[k][j];//更新路径长度
					path[i][j] = path[k][j];//更新前驱
				}
			}
		}
	}
}
//迪杰斯特拉算法求最短路径
void DJS(Graph* pgra, char ch) {
	int v0 = getPosition(pgra, ch);//得到该节点在邻接矩阵中的位置
	int vexnum = pgra->vexnum;//顶点个数
	bool* S = new bool[vexnum] {false};//S集，初始化为未选中
	int* dist = new int[vexnum];//dist集合，存放的是到当前节点的路径
	int* path = new int[vexnum];//到该节点最短路径的节点信息
	S[v0] = true;//把该节点移除S集
	dist[v0] = 0;
	//初始化
	for (int v = 0; v < vexnum; v++) {
		dist[v] = pgra->matrix[v0][v];
		if (dist[v] != INF) {//此时有路径
			path[v] = v0;
		}
	}

	//对剩余的n-1个节点逐一求最短路径
	for (int i = 1; i < vexnum - 1; i++) {//相当于计数器
		int min = INF;
		int v;
		//从S集合中任选一个进行试探
		for (int w = 0; w < vexnum; w++) {
			if (!S[w] && dist[w] < min) {//再从dist里面选取一条最短的
				min = dist[w];
				v = w;//把该节点记录下来
			}
		}
		S[v] = true;//把该点移除S集
					//更新其他节点的最短路径
		for (int w = 0; w < vexnum; w++) {
			if (!S[w] && dist[v] + pgra->matrix[v][w] < dist[w]) {
				dist[w] = dist[v] + pgra->matrix[v][w];
				path[w] = v;
			}
		}
	}
}
int main() {
	Graph* pgra = creatGraph();
	//DJS(pgra, 'a');
	//vector<int> topo(pgra->vexnum);
	//bool flag = topological(pgra,topo);
	//critical(pgra);
	//prim(pgra, 'a');
	vector<bool> visited(pgra->vexnum, false);
	//DFS(pgra, 1, visited);
	BFS(pgra, 1, visited);
	cout << "hello" << endl;
	system("pause");
	return 0;
}
