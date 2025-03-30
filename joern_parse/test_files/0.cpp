#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

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
    
    int remaining = W;
    for (auto& box : boxes) {
        if (remaining <= 0) break;
        int v = box.first;
        int w = box.second;
        if (w <= remaining) {
            total_value += v;
            remaining -= w;
        } else {
            total_value += static_cast<double>(v) * remaining / w;
            remaining = 0;
        }
    }
    
    cout << fixed << setprecision(3) << total_value << endl;
    
    return 0;
}