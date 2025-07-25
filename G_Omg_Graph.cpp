#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

void update_freq_maps(int val, int delta,
                      vector<int>& overall_freq_map,
                      map<int, int>& freq_to_count_map) {
    int old_freq = overall_freq_map[val];
    if (old_freq > 0) {
        freq_to_count_map[old_freq]--;
        if (freq_to_count_map[old_freq] == 0) {
            freq_to_count_map.erase(old_freq);
        }
    }
    overall_freq_map[val] += delta;
    int new_freq = overall_freq_map[val];
    if (new_freq > 0) {
        freq_to_count_map[new_freq]++;
    }
}

void solve(const vector<int>& a, const vector<int>& b) {
    int n = a.size();
    vector<int> current_overall_freq_val_to_count(n + 1, 0);
    map<int, int> current_freq_to_num_values;

    for (int i = 0; i < n; ++i) {
        if (a[i] == b[i]) {
            update_freq_maps(a[i], 1, current_overall_freq_val_to_count, current_freq_to_num_values);
        } else {
            update_freq_maps(a[i], 1, current_overall_freq_val_to_count, current_freq_to_num_values);
            update_freq_maps(b[i], 1, current_overall_freq_val_to_count, current_freq_to_num_values);
        }
    }

    int max_overall_matches = 0;
    int max_freq_val_no_del = current_freq_to_num_values.empty() ? 0 : current_freq_to_num_values.rbegin()->first;
    max_overall_matches = max(max_overall_matches, n - max(0, max_freq_val_no_del - n));

    for (int k = 0; k < n; ++k) {
        if (a[k] == b[k]) {
            update_freq_maps(a[k], -1, current_overall_freq_val_to_count, current_freq_to_num_values);
        } else {
            update_freq_maps(a[k], -1, current_overall_freq_val_to_count, current_freq_to_num_values);
            update_freq_maps(b[k], -1, current_overall_freq_val_to_count, current_freq_to_num_values);
        }

        int new_max_freq = current_freq_to_num_values.empty() ? 0 : current_freq_to_num_values.rbegin()->first;
        int current_length = n - 1;
        max_overall_matches = max(max_overall_matches, current_length - max(0, new_max_freq - current_length));

        if (a[k] == b[k]) {
            update_freq_maps(a[k], 1, current_overall_freq_val_to_count, current_freq_to_num_values);
        } else {
            update_freq_maps(a[k], 1, current_overall_freq_val_to_count, current_freq_to_num_values);
            update_freq_maps(b[k], 1, current_overall_freq_val_to_count, current_freq_to_num_values);
        }
    }

    cout << max_overall_matches << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    vector<vector<int>> test_a = {
        {1, 2, 1},
        {1, 1, 1, 1}
    };
    vector<vector<int>> test_b = {
        {2, 1, 2},
        {1, 1, 1, 1}
    };

    for (int i = 0; i < test_a.size(); ++i) {
        solve(test_a[i], test_b[i]);
    }

    return 0;
}
