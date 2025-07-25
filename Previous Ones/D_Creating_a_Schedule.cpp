#include <bits/stdc++.h>
using namespace std;

// Helper to get the floor number from the room number
int getFloor(int room) {
    return room / 100;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<int> rooms(m);
        for (int i = 0; i < m; i++) cin >> rooms[i];

        // Sort rooms by floor
        sort(rooms.begin(), rooms.end(), [](int a, int b) {
            return getFloor(a) < getFloor(b);
        });

        int left = 0, right = m - 1;
        vector<pair<int, int>> pairs;

        // Create pairs: lowest with highest, next lowest with next highest, etc.
        while (pairs.size() < n) {
            pairs.push_back({rooms[left++], rooms[right--]});
        }

        // For each group, assign 6 classes: low, high, low, high, low, high
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < 6; j++) {
                if (j % 2 == 0)
                    cout << pairs[i].first << " ";
                else
                    cout << pairs[i].second << " ";
            }
            cout << "\n";
        }
    }

    return 0;
}
