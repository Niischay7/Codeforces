#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Function to get minimum transition time between src and dest
int getCost(int src, int dest, const vector<int>& transition) {
    int m = transition.size() - 1; // transition is 1-based indexed

    if (src == dest) return 0;

    // Clockwise cost
    int clockwise = 0;
    int pos = src;
    while (pos != dest) {
        clockwise += transition[pos];
        pos = (pos % m) + 1;
    }

    // Counter-clockwise cost
    int counter = 0;
    pos = src;
    while (pos != dest) {
        pos = (pos == 1) ? m : pos - 1;
        counter += transition[pos];
    }

    return min(clockwise, counter);
}

int solveRequests(int m, const vector<int>& transition, const vector<int>& requested) {
    int totalTime = 0;
    int current = 1;
    for (int next : requested) {
        totalTime += getCost(current, next, transition);
        current = next;
    }
    return totalTime;
}

int main() {
    int t;
    cin >> t;
    while (t--) {
        int m, n;
        cin >> m >> n;
        vector<int> transition(m + 1); 
        for (int i = 1; i <= m; ++i) {
            cin >> transition[i];
        }

        vector<int> requested(n);
        for (int i = 0; i < n; ++i) {
            cin >> requested[i];
        }

        int result = solveRequests(m, transition, requested);
        cout << result << endl;
    }
    return 0;
}
