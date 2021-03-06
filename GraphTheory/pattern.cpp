// 图论基础知识(存图，最短路算法)与模板
// Reference: 
//   https://zhuanlan.zhihu.com/p/94890350
//   https://zhuanlan.zhihu.com/p/96621396
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <vector>
#include <queue>
#include <conio.h>
#define fp(i, l, r) for(register int i=(l); i<=(r); i++)
#define fd(i, l, r) for(register int i=(l); i>=(r); i--)

using namespace std;
const int maxn = 1005;      //图中结点数
const int maxm = 1e5+5;     //图中边的数目

//一.存图方法
//------------------------------------邻接矩阵__开始—-------------------------------------------
int mat[maxn][maxn];    //邻接矩阵
//邻接矩阵，示例：双向有边权图
inline void add1(int u, int v, int w) {
    mat[u][v] = w;
    mat[v][u] = w;
}
//------------------------------------邻接矩阵__结束—-------------------------------------------

//------------------------------------邻接表__开始--------------------------------------------
// 邻接表：链表数组实现，将邻接矩阵的行从数组替换为链表
// 我们需要用一个边的结构体来同时储存边的终点（相当于邻接矩阵的第二个下标）和权值

//如果没有边权可以不使用结构体，只存储终点即可
struct edge {
    int to, w;  //to代表边的终点，w代表该边的权值
};
// 使用STL中vector实现类链表数组
vector<edge> nodes[maxn];   //nodes[i]存储了i结点所有的邻边与邻接结点
inline void add2(int from, int to, int w) {
    edge e = {to, w};
    nodes[from].push_back(e);   //将相邻边与相邻结点信息存入动态数组中
}
// 存储图，对于无向图，调用两次add()
inline void add3(int u, int v, int w) {
    add2(u, v, w);
    add2(v, u, w);
}

// 遍历所有结点方法
inline void traverse(void) {
    for (int i=0; i<int(nodes[2].size()); i++) {
        cout << nodes[2][i].to << " ";  //打印2号结点所有的邻接结点
    }
    //range-based for形式
    for (auto &no: nodes[2]) {
        cout << no.to << " ";
    }
}

//------------------------------------邻接表__结束--------------------------------------------

//---------------------------链式前向星__开始---------------------------------------------
struct edge2 {
    int to, w, next;    //to,w意义参考邻接表实现方法，next存储下一个结点（实际为前向结点）索引位置
    int from;   //新增from成员，便于后面使用bellman-ford算法求最短路径
} edges[maxm];
int head[maxn] = {0};   //head[i]为第i个结点的相邻结点形成的链表的头节点
int cnt=0;    //edges[maxn]结构体数组计数器

// 更新插入函数
inline void add4(int from, int to, int w) {
    edges[++cnt].to = to;
    edges[cnt].w = w;
    edges[cnt].next = head[from];   //存储上一次存入的前向结点，结束终点为0
    head[from] = cnt;   //记录当前邻边和相邻结点在edges[maxm]数组中的位置
}

// 遍历函数，例子：打印2号结点所有的邻接结点
inline void traverse2(void) {
    for (int e=head[2]; e!=0; e=edges[e].next) {
        cout << edges[e].to << " " << endl;
    }
}

// 链式前向星+DFS
inline void dfs(int root) {
    cout << root << " ";
    for (int e=head[root]; e; e=edges[e].next) {
        dfs(edges[e].to);
    }
}

// 链式前向星+BFS
inline void Bfs(void) {
    queue<int> q;
    int vis[maxn]= {0}; 
    q.push(1);
    while(!q.empty()) {
        int temp = q.front(); q.pop();
        vis[temp] = 1;  //置1表示已经访问
        cout << temp;
        for (int e=head[temp]; e; e=edges[e].next) {
            if (!vis[edges[e].to]) q.push(edges[e].to);
        }
    }
}


//---------------------------链式前向星__结束---------------------------------------------


//二.最短路径算法
//(1)多源最短路问题
//  1)--------------------------Floyd算法__开始-------------------------------------------
int dist[maxn][maxn];   //邻接矩阵
void init(void) {
    memset(dist, 63, sizeof(dist));     //邻接矩阵初始化，63=0x3f,即将dist[maxn][maxn]中每个元素初始化为0x3f3f3f3f，
                                        //这个数的两倍小于32位和64位机器上的INT_MAX
    for (int i=1; i<=maxn; i++) {
        dist[i][i]=0;   //将所有点到自己的距离初始化为0
    }
}
inline void add_floyd(int u, int v, int w) {
    dist[u][v] = w;
}

inline void floyd(void) {
    //算法思想
    //Floyd本质上是一个动态规划的思想，每一次循环更新经过前k个节点，i到j的最短路径。
    //渐进时间复杂度O(n^3),空间复杂度O(n^2) 
    for (int k=1; k<=maxn; k++) {
        for (int i=1; i<=maxn; i++) {
            for (int j=1; j<=maxn; j++) {
                dist[i][j] = min(dist[i][j], dist[i][k]+dist[k][j]);    
            }
        }
    }
}

//  1)--------------------------Floyd算法__结束-------------------------------------------

//(2)单源最短路问题
//  1)---------------------------Bellman-Ford算法__开始------------------------------------
int dist_BF[maxn];  //
inline void Bellman_Ford(int n, int m, int start) {
    memset(dist_BF, 63, sizeof(dist_BF));
    dist_BF[start] = 0;
    //最短路上的点的总个数一定不大于n，尽管一般而言最短路上的顶点数比n少得多，但反正多余的松弛操作不会影响结果，我们索性：
    //把所有边松弛n-1遍！但其实Bellman-Ford算法是可以很简单地处理负权环的，只需要再多对每条边松弛一遍，如果这次还有点被更新，就说明存在负权环。
    //因为没有负权环时，最短路上的顶点数一定小于n，而存在负权环时，可以无数次地环绕这个环，最短路上的顶点数是无限的。
    for (int i=1; i<=n-1; i++) {
        for (int j=1; j<=m; j++) {
            dist_BF[edges[j].to] = min(dist_BF[edges[j].to], dist_BF[edges[j].from]+edges[j].w);
        }
    }
}
//  1)---------------------------Bellman-Ford算法__结束------------------------------------

//  2)--------------------------------SPFA_开始-------------------------------------------
        //算法思想
        //  a.只让当前点能到达的点入队
        //  b.如果一个点已经在队列里，便不重复入队
        //  c.如果一条边未被更新，那么它的终点不入队
int dist_spfa[maxn];
int inqueue[maxn];  //inqueue[i]记录结点是否在队列中
int pre[maxn];  //存储最短路上到达该结点的前一个父节点。 

inline void SPFA(int start) {   //start为起始节点
    memset(dist_spfa, 63, sizeof(dist_spfa));
    dist_spfa[start]=0;
    //初始化
    queue<int> Q;
    Q.push(start);
    inqueue[start] = 1; //已访问

    while (!Q.empty()) {
        int p = Q.front();
        Q.pop();
        inqueue[p] = 0; //更新记录数组
        for (int e=head[p]; e!=0; e=edges[e].next) {
            int to = edges[e].to;
            if (dist_spfa[to] > dist_spfa[p] + edges[e].w) {
                pre[to] = p;    //记录其父节点，用于打印路径
                dist_spfa[to] = dist_spfa[p] + edges[e].w;
                if (!inqueue[to]) {
                    Q.push(to); inqueue[to] = 1;
                }
            }
        }
    }

}
//  判负环：SPFA也可以判负权环，我们可以用一个数组记录每个顶点进队的次数，
//  当一个顶点进队超过n次时，就说明存在负权环。（这与Bellman-Ford判负权环的原理类似）
//  2)--------------------------------SPFA_开始-------------------------------------------

//  3)-------------------------------Dijkstra__开始------------------------------------------
int dist_dij[maxn];
struct node {   
    int dist, id;   //id代表节点编号，dist代表代表到达当前结点的最短距离
    node(int dist, int id) :  dist(dist), id(id){}  //构造函数
};
//仿函数
struct cmp {
    bool operator ()(node a, node b) {  //重载()函数，使其成为一个仿函数
        return a.dist > b.dist;
    }
};
priority_queue<node, vector<node>, cmp> Q;  //构造小顶堆
int vis[maxn];  //vis[i]记录第i个结点是否被访问，已访问为1，未访问为0

inline void Dijkstra(int start) {
    //初始化
    memset(dist_dij, 63, sizeof(dist_dij));
    dist_dij[start] = 0;
    Q.push(node(0, start));
    vis[start] = 1; //已访问
    while (!Q.empty()) {
        int p = Q.top().id;
        Q.pop();
        if (vis[p]) continue;
        for (int e=head[p]; e!=0; e=edges[e].next) {
            int to = edges[e].to;
            if (dist_dij[to] > dist_dij[p] + edges[e].w) {
                dist_dij[to] = dist_dij[p] + edges[e].w;
                pre[to] = p;    //更新父结点
            }
            //如果当前结点未被访问过，则加入优先队列中。
            if (!vis[to]) Q.push(node(dist_dij[to], to));   
        }

    }
}

//打印最短路径，例子：打印从结点1-4的最短路径
inline void print_path(int pos) {
    pos = 4;
    while (pos != 1) {
        cout << pos << "<-";
        pos = pre[pos]; //找寻其父结点
    }
}

//  3)-------------------------------Dijkstra__结束------------------------------------------
int main(void) {

    _getch();
    return 0;
}