// 完全背包问题
// 有 N 件物品和一个容量是 V 的背包。每种物品都有无限件可用。
// 第 i 件物品的体积是 vi，价值是 wi 。
// 求解将哪些物品装入背包，可使这些物品的总体积不超过背包容量，且总价值最大。 输出最大价值。
// 输入格式
// 第一行两个整数，N，V，用空格隔开，分别表示物品数量和背包容积。
// 接下来有 N 行，每行两个整数 vi,wi，用空格隔开，分别表示第 i 件物品的体积和价值。
// 输出格式
// 输出一个整数，表示最大价值。
// 数据范围
// 0<N,V≤10000 0<vi,wi≤10000
// 输入样例
// 4 5
// 1 2
// 2 4
// 3 4
// 4 5
// 输出样例：
// 8


#include <iostream>
#include <algorithm>
#include <cstdio>
#include <conio.h>
#define fp(i, l, r) for(register int i=(l); i<=(r); i++) 
#define fd(i, l, r) for(register int i=(l); i>=(r); i--)

using namespace std;
const int maxn = 1e4+5;
const int maxv = 1e4+5;
int dp[maxv] = {0};
int n, v;
int V[maxn];    //V[i]表示第i件物品的体积
int W[maxn];    //W[i]表示第i件物品的价值
int res = 0;

int main(void) {
    cin >> n >> v;
    fp(i, 1, n) cin >> V[i] >> W[i];
    fp(i, 1, n) {
        // 注意不同于01背包，此处为升序
        // 完全背包的特点恰是每种物品可选无限件，所以在考虑“加选一件第i种物品”这种策略时，
        // 却正需要一个可能已选入第i种物品的子结果f[i][v-c[i]]，所以就可以并且必须采用v= 0..V的顺序循环。
        fp(j, 1, v) {
            if (j<V[i]) dp[j] = dp[j];
            else {
                //状态转移方程dp[i][j] = max{dp[i-1][j], dp[i][j-V[i]]+W[i]}，此处进行了数组降维
                dp[j] = max(dp[j], dp[j-V[i]]+W[i]);
            }
        }
    }

    fp(j, 1, v) res = max(res, dp[j]);
    cout << res;

    _getch();
    return 0;
}
