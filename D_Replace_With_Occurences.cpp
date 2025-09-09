#include <bits/stdc++.h>
#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <chrono>
#include <cmath>
#include <complex>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <random>
#include <set>
#include <vector>

using namespace std;

#define INT long long
#define pb push_back
#define timetaken cerr << fixed << setprecision(10); cerr << "time taken : " << (float)clock() / CLOCKS_PER_SEC << " secs" << endl

const int MOD = 998244353;

vector<int> SieveOfEratosthenes(int n)
{
    vector<int> prime(n + 1, 1);
    for (int p = 2; p * p <= n; p++)
    {
        if (prime[p] == 1)
        {
            for (int i = p * p; i <= n; i += p)
                prime[i] = 0;
        }
    }
    return prime;
}

INT gcd(INT a, INT b)
{
    while (b != 0)
    {
        INT temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

INT power(INT x, INT y)
{
    const int MOD = 1e9 + 7;
    INT res = 1;
    x = x % MOD;
    while (y > 0)
    {
        if (y & 1)
            res = (res * x) % MOD;
        y = y >> 1;
        x = (x * x) % MOD;
    }
    return res;
}

vector<int> bfs(int start, const vector<vector<int>>& adj)
{
    vector<int> dist(adj.size(), -1);
    queue<int> q;
    q.push(start);
    dist[start] = 0;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        for (int v : adj[u])
        {
            if (dist[v] == -1)
            {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    return dist;
}

vector<int> dfs(int start, const vector<vector<int>>& adj)
{
    vector<int> visited(adj.size(), 0);
    vector<int> result;
    stack<int> s;
    s.push(start);

    while (!s.empty())
    {
        int u = s.top();
        s.pop();

        if (visited[u] == 0)
        {
            visited[u] = 1;
            result.pb(u);

            for (int v : adj[u])
            {
                if (visited[v] == 0)
                {
                    s.push(v);
                }
            }
        }
    }
    return result;
}

void solve()
{
    int n;
    cin >> n;
    vector<int> b(n);
    for (int i = 0; i < n; ++i) cin >> b[i];

    vector<int> a(n);
    vector<queue<int>> groups(n + 1);

    for (int i = 0; i < n; ++i) groups[b[i]].push(i);

    int label = 1;
    bool ok = true;

    for (int v = 1; v <= n; ++v) {
        int sz = groups[v].size();
        if (sz % v != 0) {
            ok = false;
            break;
        }

        while (!groups[v].empty()) {
            for (int i = 0; i < v; ++i) {
                a[groups[v].front()] = label;
                groups[v].pop();
            }
            label++;
        }
    }

    if (!ok) {
        cout << -1 << "\n";
    } else {
        for (int x : a) cout << x << " ";
        cout << "\n";
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    INT t;
    t = 1;
    cin >> t;
    while (t--)
    {
        solve();
    }
    timetaken;
    return 0;
}


