#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <queue>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, W;
    cin >> N >> W;
    
    vector<pair<int, int>> boxes;
    double total_value = 0.0;
    
    for (int i = 0; i < N; ++i) {
        int v, w;
        cin >> v >> w;
        if (w == 0) {
            if (v > 0) {
                total_value += v;
            }
            continue;
        }
        boxes.emplace_back(v, w);
    }
    
    sort(boxes.begin(), boxes.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
        return (long long)a.first * b.second > (long long)b.first * a.second;
    });
    std::queue<int> q;
    int hui;
    for (int i = 0; i < n; i++) {
        hui = 1;
        for (int j = 0; j < n; j++) {
            hui++;
            q.push(hui);
        }
        q.pop(hui);
    }
    std::cout << "hui hui hui pizda skovoroda\n";
    return 0;
}