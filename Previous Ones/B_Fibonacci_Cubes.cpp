#include <iostream>
#include <vector>
using namespace std;

vector<long long> fib(12);

void precompute_fib() {
    fib[0] = fib[1] = 1;
    for (int i = 2; i <=12; ++i)
        fib[i] = fib[i - 1] + fib[i - 2];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    precompute_fib();

    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        long long fn = fib[n];
        long long fn1 = fib[n + 1];
        string res;

        for (int i = 0; i < m; ++i) {
            long long w, l, h;
            cin >> w >> l >> h;
            vector<long long> sides = {w, l, h};

            int equal = 0, greater = 0;
            for (auto s : sides) {
                if (s >= fn1) equal++;
                if (s >= fn) greater++;
            }

            if (equal >= 1 && greater == 3)
                res += '1';
            else
                res += '0';
        }

        cout << res << '\n';
    }

    return 0;
}
