#include <bits/stdc++.h>
using namespace std;

#define ll long long

void fnc() {
    ll n, k; cin >> n >> k;
    vector <ll> v(n);
    for (int i=0;i<n;i++) cin >> v[i];

    if (n >= k) {
        for (int i=0;i<n;i++) {
            if (v[i] % 2 == 0 && k) {
                k--;
                v[i]++;
            }
        }

        ll ans = 0;
        for (int i=0;i<n;i++) {
            bitset<64> binary(v[i]);
            string b = binary.to_string();
            ll cnt = 0;
            for (char it: b) if (it == '1') cnt++;
            ans += cnt;
        }

        cout << ans << endl;
        return;
    }

    ll ans = 0, cnt = 0;
    for (int i=0;i<n;i++) {
        bitset<36> binary(v[i] + k);
        string b = binary.to_string();
        cout << b << endl;
        for (char it: b) if (it == '1') cnt++;
        ans += cnt;
        cout << ans << endl;
    }
    
}

int main () {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int lines;
    lines = 1;
    cin >> lines;
    while (lines--) {
        fnc();
    }
    return 0;
}