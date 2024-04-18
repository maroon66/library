//ソートにかかる時間がバカにならないという問題がある
//fast MO by yosupo
constexpr ll TEN(int n) { return (n == 0) ? 1 : 10 * TEN(n - 1); }
template <class T> using V = vector<T>;
template <class T> using VV = V<V<T>>;

struct UnionFind {
    V<int> p, r;
    int gn;
    UnionFind(int n = 0) : p(n, -1), r(n, 1), gn(n) {}
    void merge(int a, int b) {
        int x = group(a), y = group(b);
        if (x == y) return;  // same
        gn--;
        if (r[x] < r[y]) {
            p[x] = y;
        } else {
            p[y] = x;
            if (r[x] == r[y]) r[x]++;
        }
    }
    int group(int a) {
        if (p[a] == -1) return a;
        return p[a] = group(p[a]);
    }
    bool same(int a, int b) { return group(a) == group(b); }
};
template <class T>
V<pair<int, int>> manhattan_mst(V<pair<T, T>> ps,
                                T inf = numeric_limits<T>::max()) {
    V<pair<int, int>> edges;
    int n = int(ps.size());
    V<int> ids(n);
    iota(ids.begin(), ids.end(), 0);
    for (int ph = 0; ph < 4; ph++) {
        sort(ids.begin(), ids.end(), [&](auto i, auto j) {
            T ixy = (ps[i].first + ps[i].second),
              jxy = (ps[j].first + ps[j].second);
            return tie(ixy, ps[i].second) > tie(jxy, ps[j].second);
        });
        V<T> xv;
        for (int i = 0; i < n; i++) xv.push_back(ps[i].first);
        sort(xv.begin(), xv.end());
        xv.erase(unique(xv.begin(), xv.end()), xv.end());
        using P = pair<T, int>;
        V<P> fen(n, P(-inf, -1));
        for (int id : ids) {
            auto xi = int(lower_bound(xv.begin(), xv.end(), ps[id].first) -
                          xv.begin());
            P ma = P(-inf, -1);
            {
                int i = xi + 1;
                while (i > 0) {
                    if (ma.first <= fen[i - 1].first) ma = fen[i - 1];
                    i -= i & -i;
                }
            }
            if (ma.second != -1) edges.push_back({id, ma.second});
            {
                T x = ps[id].first - ps[id].second;
                int i = xi + 1;
                while (i <= n) {
                    if (fen[i - 1].first <= x) fen[i - 1] = P(x, id);
                    i += i & -i;
                }
            }
        }
 
        for (auto& p : ps) {
            swap(p.first, p.second);
        }
        if (ph == 1) {
            for (auto& p : ps) {
                p.second *= -1;
            }
        }
    }
    auto dist = [&](int i, int j) {
        return abs(ps[i].first - ps[j].first) +
               abs(ps[i].second - ps[j].second);
    };
    sort(edges.begin(), edges.end(), [&](auto x, auto y) {
        return dist(x.first, x.second) < dist(y.first, y.second);
    });
    auto uf = UnionFind(n);
    V<pair<int, int>> res;
    for (auto p : edges) {
        if (uf.same(p.first, p.second)) continue;
        res.push_back(p);
        uf.merge(p.first, p.second);
    }
    return res;
}
 
template <class Q> void sort_mo(V<Q>& ques) {
    auto dist = [&](Q x, Q y) { return abs(x.l - y.l) + abs(x.r - y.r); };
    int n = int(ques.size());
    if (n <= 1) return;
 
    // ordered by manhattan mst (tsp approx)
    {
        VV<int> tr(n);
        V<pair<int, int>> ps(n);
        for (int i = 0; i < n; i++) {
            ps[i] = {ques[i].l, ques[i].r};
        }
        auto edges = manhattan_mst(ps);
        for (auto e : edges) {
            tr[e.first].push_back(e.second);
            tr[e.second].push_back(e.first);
        }
        V<Q> nques;
        auto dfs = [&](auto self, int p, int b) -> void {
            nques.push_back(ques[p]);
            for (auto d : tr[p]) {
                if (d == b) continue;
                self(self, d, p);
            }
        };
        dfs(dfs, 0, -1);
        ques = nques;
    }
 
    // reverse first
    {
        int mi = TEN(9), idx = -1;
        for (int i = 0; i < n - 1; i++) {
            int nw = abs(ques[i].l - ques[i].r) + dist(ques[0], ques[i + 1]);
            if (nw < mi) {
                mi = nw;
                idx = i;
            }
        }
        reverse(ques.begin(), ques.begin() + idx + 1);
    }
    // reverse last
    {
        int mi = TEN(9), idx = -1;
        for (int i = 1; i < n; i++) {
            int nw = dist(ques[i - 1], ques[n - 1]);
            if (nw < mi) {
                mi = nw;
                idx = i;
            }
        }
        reverse(ques.begin() + idx, ques.end());
    }
    // greedy
    for (int ph = 0; ph < 10; ph++) {
        for (int i = 1; i < n; i++) {
            for (int j = i + 1; j < min(i + 10, n - 1); j++) {
                ll pre =
                    dist(ques[i - 1], ques[i]) + dist(ques[j], ques[j + 1]);
                ll now =
                    dist(ques[i - 1], ques[j]) + dist(ques[i], ques[j + 1]);
                if (now < pre) {
                    reverse(ques.begin() + i, ques.begin() + j + 1);
                }
            }
        }
    }
}
