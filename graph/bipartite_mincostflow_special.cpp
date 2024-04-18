//https://contest.ucup.ac/submission/164701

// m, n, capa, cost -> capa, to, slopes
//   m sources, supply 1
//   n sinks, capacity capa[v]
//   min cost
//   O(m n^3 + m n^2 log m) time
namespace mcbm {
using Cost = int;
using Total = Int;
constexpr int MAX_M = 100'000;
constexpr int MAX_N = 10;
constexpr Cost COST_INF = 1001001001;
int m, n;
int capa[MAX_N];
Cost cost[MAX_M][MAX_N + 1];
int to[MAX_M];
Cost slopes[MAX_M];
priority_queue<pair<Cost, int>, vector<pair<Cost, int>>, greater<pair<Cost, int>>> cisss[MAX_N + 1][MAX_N];
Cost adj[MAX_N + 1][MAX_N], dist[MAX_N];
int prv[MAX_N];
bool on[MAX_N];
int que[MAX_N * MAX_N], *qb, *qe;
Total run() {
  for (int u = 0; u <= n; ++u) for (int v = 0; v < n; ++v) cisss[u][v] = {};
  for (int i = 0; i < m; ++i) {
    cost[i][to[i] = n] = 0;
    for (int v = 0; v < n; ++v) cisss[n][v].emplace(cost[i][v], i);
  }
  Total total = 0;
  for (int flow = 0; flow < m; ++flow) {
    for (int u = 0; u <= n; ++u) for (int v = 0; v < n; ++v) {
      auto &cis = cisss[u][v];
      for (; !cis.empty() && to[cis.top().second] != u; cis.pop()) {}
      adj[u][v] = cis.empty() ? COST_INF : cis.top().first;
    }
    qb = qe = que;
    for (int v = 0; v < n; ++v) {
      dist[v] = adj[n][v]; prv[v] = n;
      on[*qe++ = v] = true;
    }
    for (; qb != qe; ) {
      const int u = *qb++;
      on[u] = false;
      for (int v = 0; v < n; ++v) {
        const Cost cc = dist[u] + adj[u][v];
        if (dist[v] > cc) {
          dist[v] = cc; prv[v] = u;
          if (!on[v]) on[*qe++ = v] = true;
        }
      }
    }
    int vm = -1;
    for (int v = 0; v < n; ++v) if (capa[v] > 0) {
      if (!~vm || dist[vm] > dist[v]) vm = v;
    }
    // Failure means m > \sum_v capa[v]
    assert(~vm);
    total += slopes[flow] = dist[vm];
    --capa[vm];
    for (int v = vm; v != n; ) {
      const int u = prv[v];
      const int i = cisss[u][v].top().second;
      for (int w = 0; w < n; ++w) if (v != w) {
        cisss[v][w].emplace(cost[i][w] - cost[i][v], i);
      }
      to[i] = v; v = u;
    }
  }
  return total;
}
}  // namespace mcbm
