#define FAST_ALLOCATOR_MEMORY 1e8
#include "optimization.h"
#include <climits>
#include <vector>
#include <iostream>
#include <queue>

using namespace std;
queue <int> q;
vector <vector<int>> d;
vector <vector<int>> vec;
long long ans = 0;
int n, m, b, e, u, x, v;

int main() {
  n = readInt(); m = readInt();
  d.resize(n, vector<int> (n, INT_MAX));
  vec.resize(n);
  for(int i = 0; i < m; i++){
    b = readInt() - 1; e = readInt() - 1;
    vec[b].push_back(e);
    vec[e].push_back(b);
  }
  for(int i = 0; i < n; i++){
    d[i][i] = 0; q.push(i);
    while (!q.empty()){
      x = q.front();
      q.pop();
      for(int e: vec[x]){
        if(d[i][e] > d[i][x] + 1){
          d[i][e] = d[i][x] + 1;
          q.push(e);
        }
      }
    }
    for(int j = 0; j < n; j++) ans += d[i][j];
  }
  writeInt<long long>(ans / 2, '\n');
  return 0;
}
