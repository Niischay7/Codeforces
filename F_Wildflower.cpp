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

// const int MOD = 998244353;
const INT MOD = 1e9+7;

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



void BFS(int start, const vector<vector<int>>& adj, vector<int>& adjparent, vector<vector<int>>& adjchildren)
{
    int n = adj.size() - 1;
    adjparent.assign(n + 1, -1);
    adjchildren.assign(n + 1, vector<int>{});

    queue<int> q;
    q.push(start);
    adjparent[start] = -1;
    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        for (int v : adj[u])
        {
            if (adjparent[v] == -1 && v != start) 
            {
                adjparent[v] = u;
                adjchildren[u].push_back(v);
                q.push(v);
            }
        }
    }
}

int DFS(const int root, const vector<vector<int>>& adjchildren)
{
    int k = 0;
    stack<int> dfs;
    dfs.push(root);
    while (!dfs.empty())
    {
        int u = dfs.top();
        dfs.pop();
        ++k;
        for (int v : adjchildren[u])
        {
            dfs.push(v);
        }
    }
    return k;
}

void solve()
{
    INT n;
    cin >> n;
    vector<vector<int>> adj(n + 1);
    for (INT i = 0; i < n - 1; ++i)
    {
        INT u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<int> adjparent;
    vector<vector<int>> adjchildren;
    BFS(1, adj, adjparent, adjchildren);

    vector<INT> ans;
    for (int i = 1; i <= n; i++)
    {
        if (adjchildren[i].empty())
            ans.push_back(i);
    }
    
    if (ans.size() == 1)
    {
        cout << power(2, n) << endl; 
        return;
    }
    if (ans.size() > 2)
    {
        cout << 0 << endl;
        return;
    }


    INT node = -1;
    for (int i = 1; i <= n; i++)
    {
        if (adjchildren[i].size() == 2)
        {
            node = i;
            break;
        }
    }

    if (node == -1)
    {
        cout << 0 << endl; 
        return;
    }

    INT child1 = adjchildren[node][0];
    INT child2 = adjchildren[node][1];

    INT x = DFS(child1, adjchildren);
    INT y = DFS(child2, adjchildren);

    INT p;
    if (abs(x-y) == 0)
        p = 2;
    else
        p = (3 * power(2, abs(x-y) - 1)) % MOD;

    INT result = (power(2,n-x-y) * p) % MOD;
    cout << result << endl;
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
