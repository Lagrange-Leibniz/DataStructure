#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<stack>
#include<queue>
#define MAX 100 //�����������
#define INF (~(0x1<<31))//�����

using namespace std;

//�ڽӾ���
typedef struct _graph {
	int matrix[MAX][MAX];//��ά�����ʾ�ľ���
	char vexs[MAX];//���㼯
	int vexnum;//�������
	int edgnum;//�ߵĸ���
}Graph, *pGraph;//�ڽӾ���Ĵ洢�ṹ����pGraphָ��ָ��

				//�߽ṹ��
typedef struct _edge {
	char start;//���
	char end;//����
	int weight;//Ȩֵ
}Edge;

//�ɽڵ�����Ѱ�Ҷ�Ӧ�ڽӾ���λ��
int getPosition(Graph* gra, char ch) {
	for (int i = 0; i < gra->vexnum; i++) {
		if (gra->vexs[i] == ch) {
			return i;
		}
	}
	return -1;
}
//���û��Լ�����һ��ͼ
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
	//���Զ���ʼ��Ȩֵ
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
	//���û���ʼ��Ȩֵ
	printf("\nplease initiate the weights of your edge, 1st char is the start, 3rd is the weight\n");
	for (int i = 0; i < e; i++) {
		printf("egde(%d): ", i);
		cin >> c1 >> c2 >> weight;
		p1 = getPosition(pgra, c1);
		p2 = getPosition(pgra, c2);
		if (p1 != -1 && p2 != -1) {
			pgra->matrix[p1][p2] = weight;                                      /*!!!!!!!.....ע��.....������������*/
			pgra->matrix[p2][p1] = weight;                                     //���������ͼ�Ļ�����Ѵ���ע�͵�
		}
		else {
			printf("Input Error: Invalid edge\n");
			delete pgra;
			return NULL;
		}
	}
	return pgra;
}
//������ȱ���ͼ
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
//������ȱ���ͼ
void DFS(Graph*gra, int v, vector<bool>& visited) {
	cout << gra->vexs[v] << endl;
	visited[v] = true;
	for (int i = 0; i < gra->vexnum; i++) {
		if (!visited[i] && gra->matrix[v][i] != INF && i != v) {
			DFS(gra, i, visited);
		}
	}
}
//��С������ ����ķ�㷨
void prim(Graph*gra, char v0) {
	int n = gra->vexnum;
	int k = getPosition(gra, v0);
	vector<vector<int>> close(n, vector<int>(n));//��һ����Ϊ���ĸ��������ڶ���Ϊ�õ㵽������ֱ��·������
	vector<bool> isvisited(n, true);
	//��ʼ����k���������·��
	for (int i = 0; i < n; i++) {
		close[k][i] = gra->matrix[k][i];
	}
	isvisited[k] = false;//��k����u��
	for (int i = 1; i < n; i++) {//��ʣ���n-1��·�����в���
		int min = INF, v;
		for (int j = 0; j < n; j++) {//ѡ���k��v-u������̵�һ��
			if (isvisited[j] && k != j && gra->matrix[k][j] < INF) {//���ȱ�֤��v-u����ѡԪ�أ�Ȼ����ѡ�����������С��·��
				v = j;
				min = gra->matrix[k][j];
			}
		}
		isvisited[v] = false;
		cout << gra->vexs[k] << " " << gra->vexs[v] << endl;
		k = v;
	}
}
//�������� - ����ͼ �ڽӾ���洢
bool topological(Graph*gra, vector<int>& topo) {
	//ͳ�����нڵ�����
	int n = gra->vexnum;
	//int* indegree = new int[gra->vexnum]{0};//���ÿ���ڵ�����
	vector<int> indegree(n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (gra->matrix[j][i] != INF && gra->matrix[j][i] != 0) {
				indegree[i]++;
			}
		}
	}
	stack<int> sta;//������Ϊ0�ĵ�
	for (int i = 0; i < n; i++) {
		if (indegree[i] == 0) {
			sta.push(i);
		}
	}
	int count = 0;//��Ϊ������
	while (!sta.empty()) {
		int k = sta.top();
		sta.pop();
		topo[count] = k;//���浽���˼�����
		count++;
		//�����߼��ϣ�ɾ���ýڵ㣬ͳ���µ����Ϊ0�ĵ�
		for (int i = 0; i < n; i++) {
			if (gra->matrix[k][i] != INF) {//���֮ǰ����·��
				indegree[i]--;
				if (indegree[i] == 0) {//�������µ����Ϊ0�ĵ�
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
//�ؼ�·��
void critical(Graph*gra) {
	int n = gra->vexnum;
	vector<int> topo(n);
	if (!topological(gra, topo)) return;//˵��ͼ���л�·
	vector<int> ve(n);
	vector<int> vl(n, INF);
	//�����˼���������ѡȡ���㣬����ve��ͳ��
	for (int i = 0; i < n; i++) {
		int k = topo[i];//��k��¼��ǰ�Ľڵ�
						//̽����k��j��·��
		for (int j = 0; j < n; j++) {
			if (gra->matrix[k][j] != INF && gra->matrix[k][j] != 0) {//�������·�������ܰ�������
				if (ve[k] + gra->matrix[k][j] > ve[j]) {//ѡ��ϴ��
					ve[j] = ve[k] + gra->matrix[k][j];
				}
			}
		}
	}
	//�����˼����е���ѡȡ���㣬����vl��ͳ��
	vl[n - 1] = ve[n - 1];//���һ�������ve��vl����ȵ�
	for (int i = n - 1; i >= 0; i--) {//ע���Ƿ�������
		int k = topo[i];
		//̽����j��k��·��
		for (int j = 0; j < n; j++) {
			if (gra->matrix[j][k] != INF && gra->matrix[j][k] != 0) {
				if (vl[k] - gra->matrix[j][k] < vl[j]) {//ѡ���С��
					vl[j] = vl[k] - gra->matrix[j][k];
				}
			}
		}
	}
	//ÿ��������������ж�
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (gra->matrix[i][j] != INF && gra->matrix[i][j] != 0) {//��֤��·��
				if (ve[i] == vl[j] - gra->matrix[i][j]) {//���������ͬ
					cout << gra->vexs[i] << " " << gra->vexs[j] << endl;
				}
			}
		}
	}
}
//���������㷨�����·��
void Floyd(Graph* gra) {
	//��ʼ��
	int n = gra->vexnum;
	vector<vector<int>> dist(n, vector<int>(n));//�������֮�����̾��볤��
	vector<vector<int>> path(n, vector<int>(n));//��������·����Ϣ����һ����ǰ��
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
	for (int k = 0; k < n; k++) {//k���м�����Ԫ��
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (dist[i][k] + dist[k][j] < dist[i][j]) {//��i����k��j��һ��·����ԭ�ȵ�·��Ҫ��
					dist[i][j] = dist[i][k] + dist[k][j];//����·������
					path[i][j] = path[k][j];//����ǰ��
				}
			}
		}
	}
}
//�Ͻ�˹�����㷨�����·��
void DJS(Graph* pgra, char ch) {
	int v0 = getPosition(pgra, ch);//�õ��ýڵ����ڽӾ����е�λ��
	int vexnum = pgra->vexnum;//�������
	bool* S = new bool[vexnum] {false};//S������ʼ��Ϊδѡ��
	int* dist = new int[vexnum];//dist���ϣ���ŵ��ǵ���ǰ�ڵ��·��
	int* path = new int[vexnum];//���ýڵ����·���Ľڵ���Ϣ
	S[v0] = true;//�Ѹýڵ��Ƴ�S��
	dist[v0] = 0;
	//��ʼ��
	for (int v = 0; v < vexnum; v++) {
		dist[v] = pgra->matrix[v0][v];
		if (dist[v] != INF) {//��ʱ��·��
			path[v] = v0;
		}
	}

	//��ʣ���n-1���ڵ���һ�����·��
	for (int i = 1; i < vexnum - 1; i++) {//�൱�ڼ�����
		int min = INF;
		int v;
		//��S��������ѡһ��������̽
		for (int w = 0; w < vexnum; w++) {
			if (!S[w] && dist[w] < min) {//�ٴ�dist����ѡȡһ����̵�
				min = dist[w];
				v = w;//�Ѹýڵ��¼����
			}
		}
		S[v] = true;//�Ѹõ��Ƴ�S��
					//���������ڵ�����·��
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