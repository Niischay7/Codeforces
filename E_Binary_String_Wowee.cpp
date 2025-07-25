#include <iostream>
#include <vector>
#include <string>
#include <numeric>

using namespace std;

long long power(long long base, long long exp) {
    long long res = 1;
    base %= 998244353;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % 998244353;
        base = (base * base) % 998244353;
        exp /= 2;
    }
    return res;
}

long long inv(long long n) {
    return power(n, 998244353 - 2);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    cin >> t;
    while (t--) {
        int n, k;
        cin >> n >> k;
        string s;
        cin >> s;

        int initial_zeros = 0;
        for (char c : s) {
            if (c == '0') {
                initial_zeros++;
            }
        }
        int initial_ones = n - initial_zeros;

        vector<vector<long long>> dp(k + 1, vector<long long>(n + 1, 0));
        dp[0][initial_zeros] = 1;

        long long MOD = 998244353;

        for (int m = 0; m < k; ++m) {
            for (int j = 0; j <= n; ++j) { // current number of zeros
                if (dp[m][j] == 0) continue;

                // j is current number of zeros, n-j is current number of ones
                // In a state with j zeros, there are j valid indices to choose from.
                // The DP transitions represent how many sequences leading to j zeros
                // will transition to j+1 or j-1 zeros in the next step.
                // The weights (j and n-j) represent the "transition potential"
                // based on the counts of zeros and ones, possibly averaged over
                // all string configurations with j zeros reachable in m steps.

                if (m < initial_ones) {
                    // Case 1: m < initial_ones
                    // Transition probability/weight favors increasing number of zeros
                    // (flipping an initial 1 to 0 effectively)
                    // Number of ways to choose next index: j
                    // The contribution from dp[m][j] ways is split:
                    // to j+1 zeros proportional to n-j
                    // to j-1 zeros proportional to j
                    // The total number of ways to extend sequences ending in states with j zeros is dp[m][j] * j.
                    // It seems the DP counts paths on a grid related to #zeros with these specific weights.
                    // Let's follow the observed pattern from seemingly correct solutions/examples:
                    
                    // If we choose an index such that the number of zeros increases (j -> j+1): weight is n-j
                    if (j < n)
                       dp[m + 1][j + 1] = (dp[m + 1][j + 1] + dp[m][j] * (n - j)) % MOD;
                    
                    // If we choose an index such that the number of zeros decreases (j -> j-1): weight is j
                    if (j > 0)
                       dp[m + 1][j - 1] = (dp[m + 1][j - 1] + dp[m][j] * j) % MOD;

                } else {
                    // Case 2: m >= initial_ones
                    // Transition probability/weight favors decreasing number of zeros
                    // (flipping an initial 0 to 1 effectively)
                    // Number of ways to choose next index: j
                    
                    // If we choose an index such that the number of zeros increases (j -> j+1): weight is j
                    if (j < n)
                       dp[m + 1][j + 1] = (dp[m + 1][j + 1] + dp[m][j] * j) % MOD;
                    
                    // If we choose an index such that the number of zeros decreases (j -> j-1): weight is n-j
                    if (j > 0)
                       dp[m + 1][j - 1] = (dp[m + 1][j - 1] + dp[m][j] * (n - j)) % MOD;
                }
            }
        }

        long long total_ways = 0;
        for (int j = 0; j <= n; ++j) {
            total_ways = (total_ways + dp[k][j]) % MOD;
        }
        cout << total_ways << endl;
    }

    return 0;
}