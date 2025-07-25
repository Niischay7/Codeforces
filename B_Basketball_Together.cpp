#include <bits/stdc++.h>
using namespace std;

#define ll long long

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ll N, D;
    cin >> N >> D;

    vector<ll> P(N);
    for (int i = 0; i < N; i++) {
        cin >> P[i];
    }
    sort(P.rbegin(), P.rend());

    ll wins = 0;
    ll used = 0;

    for (int i = 0; i < N; i++) {
        ll power = P[i];
        ll required = D / power + 1;

        if (used + required <= N) {
            wins++;
            used += required;
        } else {
            break;
        }
    }

    cout << wins << '\n';

    return 0;
}
