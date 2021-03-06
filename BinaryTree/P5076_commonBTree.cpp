//Reference：https://www.luogu.com.cn/problem/solution/P5076

// 二叉搜索树(BST)性质：
//      1. 若左子树不空，则左子树上所有结点的值均小于它的根结点的值
//      2. 若右子树不空，则右子树上所有结点的值均大于它的根结点的值
//      3. 左、右子树也分别为二叉排序树
//      4. 没有权值相等的结点。

#include <iostream>
#include <algorithm>
#include <cstdio>
#include <queue>
#include <cstring>
#include <conio.h>
#define fp(i, l, r) for(register int i=(l); i<=(r); i++)
#define fd(i, l, r) for(register int i=(l); i>=(r); i--)
typedef long long ll;

using namespace std;
const int maxn = 1e5+5;
const int INF = 0x7fffffff;
struct node{
    int val, ls, rs, cnt, siz;  //val代表当前结点的权值，ls代表其左孩子下标，
                                //rs代表其右孩子下标，cnt代表当前结点的权值出现了几次，size代表子树大小与自己大小的和  
} tree[maxn];   
int cont = 0;   //结点计数器
int q, opt, x;

//向BST中插入结点
void add(int loc, int v) {  //loc代表当前结点的下标，v代表要插入的值
    tree[loc].siz++;    //添加v的过程中，到达当前结点，由siz所代表含义，此处自增
    if (tree[loc].val == v) {   //当前结点的val==v,只需将当前结点对应的cnt+1
        tree[loc].cnt++;
        return;
    }
    else if (tree[loc].val > v) {
        if (tree[loc].ls != 0) {
            add(tree[loc].ls, v);   //当左孩子存在，在其左子树中继续搜索。
        }
        else {
            cont++; //tree[loc]新增左孩子结点
            tree[cont].val = v;
            tree[cont].ls = tree[cont].rs = 0;
            tree[cont].siz = tree[cont].cnt = 1;
            tree[loc].ls = cont;
        }
    }
    else {  //tree[loc].val < v
        if (tree[loc].rs != 0) {
            add(tree[loc].rs, v);   //在其右子树中搜索
        }
        else {
            cont++;
            tree[cont].val = v;
            tree[cont].ls = tree[cont].rs = 0;
            tree[cont].siz = tree[cont].cnt = 1;
            tree[loc].rs = cont;
        }
    }
}

//查询x的前驱（前驱定义为小于x，且最大的数）
int queryFront(int loc, int x, int ans) { //loc代表当前结点的下标，并需要求x的前驱, ans存储搜索到的答案
    if (tree[loc].val >= x) {   //当前结点的值大于x，在其左子树中寻找
        if (tree[loc].ls == 0) { //不存在左子树，直接返回已有答案
            return ans;
        }     
        else {
            return queryFront(tree[loc].ls, x, ans);   //存在左子树，在左子树中递归搜索
        }
    }
    else {  //tree[loc].val < x
        if (tree[loc].rs == 0) {    //不存在右子树，
            return tree[loc].val;   //返回当前结点的值
        }
        else {  //存在右子树
            if (tree[loc].cnt == 0) {   //如果当前结点的cnt为0，则不需要更新ans的值
                return queryFront(tree[loc].rs, x, ans);
            } 
            else {
                return queryFront(tree[loc].rs, x, tree[loc].val);  //cnt！=0，需要更新
            }
        }
    }
}

//查询x的后继（后继定义为大于x，且最小的数） 
int queryBack(int loc, int x, int ans) {
    if (tree[loc].val <= x) {
        if (tree[loc].rs == 0) {
            return ans;
        }
        else {  //tree[loc].rs != 0
            return queryBack(tree[loc].rs, x, ans);
        }
    }
    else {  //tree[loc].val > x
        if (tree[loc].ls == 0) {
            return tree[loc].val;
        }
        else {  //tree[loc].ls != 0
           if (tree[loc].cnt == 0) {
               return queryBack(tree[loc].ls, x, ans);
           } 
           else { //tree[loc].cnt !=0
               return queryBack(tree[loc].ls, x, tree[loc].val);
           }
        }
    }
}

//查询数x的排名（排名定义为比当前数小的数的个数+1。若有多个相同的数，应输出最小的排名）
int queryRankforVal(int loc, int x) {
    if (loc == 0) return 0;     //此时到达叶节点，说明没有比x小的数
    else {  //loc!=0
        if (tree[loc].val == x) {
            return tree[tree[loc].ls].siz;
        }
        else if (tree[loc].val < x) {  //右子树寻找
            return queryRankforVal(tree[loc].rs, x)+tree[tree[loc].ls].siz + tree[loc].cnt;
        }
        else {  //tree[loc].val > x 左子树寻找
            return queryRankforVal(tree[loc].ls, x);
        }
    }
}

//查询排名为x的数
int queryValofRank(int loc, int x) {
    if (loc == 0) return INF;   //没有改排名的数
    else {  //loc!=0,未到达叶节点
        if (tree[tree[loc].ls].siz >= x) {  //注意是查询tree[loc].ls的siz，当前结点的值的排名大于x，在其左子树中搜索
            return queryValofRank(tree[loc].ls, x);
        }
        else if (tree[tree[loc].ls].siz + tree[loc].cnt >= x) {     //当前结点值的排名等与x，返回值
            return tree[loc].val;
        }
        else {  //当前结点的排名小于x，在其右子树中寻找
            return queryValofRank(tree[loc].rs, x-tree[tree[loc].ls].siz-tree[loc].cnt);
        }
    }
}


int main(void) {
    cin >> q;
    fp(i, 1, q) {
        cin >> opt >> x;
        if (opt == 1) cout << queryRankforVal(1, x)+1 << endl;
        else if (opt == 2) cout << queryValofRank(1, x) << endl;
        else if (opt == 3) cout << queryFront(1, x, -INF) << endl;
        else if (opt == 4) cout << queryBack(1, x, INF) << endl;
        else if (opt == 5) {
            if (cont==0) {  //当二叉树为空时，手动建立根节点
               cont++;
               tree[cont].val = x;
               tree[cont].ls = tree[cont].rs = 0;
               tree[cont].cnt = tree[cont].siz = 1; 
            }
            else {
                add(1, x);
            }
        }
    }

    _getch();
    return 0;
}