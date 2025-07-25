#include <iostream>
#include <vector>
using namespace std;

int main() {
    int t;
    cin >> t;
    
    while(t--) {
        int n;
        cin >> n;
        for(int i = 0; i < n; i++) {
            if(i > 0) cout << " ";
            cout << (i % 2 == 0 ? 1 : 2);
        }
        cout << endl;
    }
    
    return 0;
}