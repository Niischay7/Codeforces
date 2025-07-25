#include <bits/stdc++.h>
using namespace std;

long long solve_test_case() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    long long ans = 0;

    // Count odd-length subarrays
    for (int i = 0; i < n; i++) {
        ans += (i + 1) * (n - i + 1) / 2; // Number of odd subarrays including a[i]
    }

    // Handle even-length subarrays using a sliding window
    for (int len = 2; len <= n; len += 2) {
        unordered_map<int, int> freq;
        bool has_majority = false;

        // Initialize the first window of size len
        for (int i = 0; i < len; i++) {
            freq[a[i]]++;
            if (freq[a[i]] > len / 2) {
                has_majority = true;
            }
        }
        if (has_majority) {
            ans++;
        }

        // Slide the window across the array
        for (int i = len; i < n; i++) {
            // Remove the outgoing element
            int out_elem = a[i - len];
            freq[out_elem]--;
            if (freq[out_elem] == len / 2) { // Losing majority
                has_majority = false;
                for (auto &[key, count] : freq) {
                    if (count > len / 2) {
                        has_majority = true;
                        break;
                    }
                }
            }

            // Add the incoming element
            int in_elem = a[i];
            freq[in_elem]++;
            if (freq[in_elem] > len / 2) {
                has_majority = true;
            }

            // Update answer if a majority element exists
            if (has_majority) {
                ans++;
            }
        }
    }

    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        cout << solve_test_case() << '\n';
    }

    return 0;
}
