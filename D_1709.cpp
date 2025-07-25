#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

// Solves a single test case
void solve() {
    int n;
    cin >> n;
    vector<int> a(n), b(n);
    // pos[k] stores {array_id, index} for number k
    // array_id: 1 for 'a', 2 for 'b'
    // index: 0-based
    vector<pair<int, int>> pos(2 * n + 1);

    for (int i = 0; i < n; ++i) {
        cin >> a[i];
        pos[a[i]] = {1, i};
    }
    for (int i = 0; i < n; ++i) {
        cin >> b[i];
        pos[b[i]] = {2, i};
    }

    vector<pair<int, int>> ops;

    // Phase 1: Place numbers 1 to n into array a
    for (int k = 1; k <= n; ++k) {
        pair<int, int> current_pos = pos[k];
        int target_idx = k - 1;

        // If k is in array b, move it to array a
        if (current_pos.first == 2) {
            int j = current_pos.second;
            ops.push_back({3, j + 1}); // 1-based index for output

            int val_in_a = a[j];
            swap(a[j], b[j]);

            // Update positions
            pos[k] = {1, j};
            pos[val_in_a] = {2, j};
            current_pos = pos[k];
        }

        // k is now in array a. Bubble it to the correct position.
        int j = current_pos.second;
        while (j > target_idx) {
            // Op to swap a[j-1] and a[j] is (1, j) in 1-based index
            ops.push_back({1, j});

            int val_to_swap = a[j - 1];
            swap(a[j - 1], a[j]);

            // Update positions
            pos[k] = {1, j - 1};
            pos[val_to_swap] = {1, j};
            j--;
        }
    }

    // Phase 2: Place numbers n+1 to 2n into array b
    for (int k = 1; k <= n; ++k) {
        int val_to_place = n + k;
        int target_idx = k - 1;
        pair<int, int> current_pos = pos[val_to_place];

        // The value must be in b, since a is sorted
        int j = current_pos.second;
        while (j > target_idx) {
            // Op to swap b[j-1] and b[j] is (2, j) in 1-based index
            ops.push_back({2, j});

            int val_to_swap = b[j - 1];
            swap(b[j - 1], b[j]);
            
            // Update positions
            pos[val_to_place] = {2, j - 1};
            pos[val_to_swap] = {2, j};
            j--;
        }
    }

    cout << ops.size() << endl;
    for (const auto& op : ops) {
        cout << op.first << " " << op.second << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}