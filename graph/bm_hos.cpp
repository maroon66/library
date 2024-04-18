namespace bm {
constexpr int LIM_N0 = @;
constexpr int LIM_N1 = @;
constexpr int LIM_M = @;
int n0, n1, m, as[LIM_M], bs[LIM_M];
int to[LIM_N0], fr[LIM_N1], tof;
int pt[LIM_N0 + 2], zu[LIM_M], used[LIM_N0], lev[LIM_N0], que[LIM_N0], *qb, *qe;
void init(int n0_, int n1_) {
  n0 = n0_; n1 = n1_; m = 0;
}
int ae(int u, int v) {
  as[m] = u; bs[m] = v; return m++;
}
int augment(int u) {
  used[u] = tof;
  for (int j = pt[u]; j < pt[u + 1]; ++j) {
    const int v = zu[j];
    const int w = fr[v];
    if (!~w || (used[w] != tof && lev[u] < lev[w] && augment(w))) {
      to[u] = v; fr[v] = u; return 1;
    }
  }
  return 0;
}
int run() {
  memset(pt, 0, (n0 + 2) * sizeof(int));
  for (int i = 0; i < m; ++i) ++pt[as[i] + 2];
  for (int u = 2; u <= n0; ++u) pt[u + 1] += pt[u];
  for (int i = 0; i < m; ++i) zu[pt[as[i] + 1]++] = bs[i];
  memset(to, ~0, n0 * sizeof(int));
  memset(fr, ~0, n1 * sizeof(int));
  memset(used, ~0, n0 * sizeof(int));
  for (tof = 0; ; ) {
    qb = qe = que; memset(lev, ~0, n0 * sizeof(int));
    for (int u = 0; u < n0; ++u) if (!~to[u]) lev[*qe++ = u] = 0;
    for (; qb != qe; ) {
      const int u = *qb++;
      for (int j = pt[u]; j < pt[u + 1]; ++j) {
        const int w = fr[zu[j]];
        if (~w && !~lev[w]) lev[*qe++ = w] = lev[u] + 1;
      }
    }
    int f = 0;
    for (int u = 0; u < n0; ++u) if (!~to[u]) f += augment(u);
    if (!f) return tof;
    tof += f;
  }
}

// s: true, t: false (s: reachable from unmatched left)
// vertex cover: (0: false, 0: true)
// independent set: (0: true, 1: false)
bool side0[LIM_N0], side1[LIM_N1];
void dfs(int u) {
  if (!side0[u]) {
    side0[u] = true;
    for (int j = pt[u]; j < pt[u + 1]; ++j) {
      const int v = zu[j];
      if (!side1[v]) {
        side1[v] = true;
        const int w = fr[v];
        if (~w) dfs(w);
      }
    }
  }
}
void minCut() {
  memset(side0, 0, n0 * sizeof(bool));
  memset(side1, 0, n1 * sizeof(bool));
  for (int u = 0; u < n0; ++u) if (!~to[u]) dfs(u);
}
}  // namespace bm
