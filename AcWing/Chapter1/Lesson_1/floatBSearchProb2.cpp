#include <iostream>
#include <algorithm>
#include <vector>
#include <conio.h>
typedef long long ll;

using namespace std;
const int maxn = 1e5+10;

int main(void){
    double n; scanf("%lf", &n);
    int flag = n>0 ? 1 : -1;
    n = abs(n);

    const double eps = 1e-8;
    double l = 0, r = maxn;
    while (r-l>eps) {
        double mid = (l+r)/2;
        if (mid*mid*mid>=n) r = mid;
        else l = mid;
    }

    printf("%lf", flag*l);

    _getch();
    return 0;
}