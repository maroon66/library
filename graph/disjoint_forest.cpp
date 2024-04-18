//https://topcoder-g-hatena-ne-jp.jag-icpc.org/spaghetti_source/20121124/1354351349.html
//XXII Opencup GP of BSUIR B (k=2)
//使う辺の重みの和を最大化している

struct Graph {
  int n;
  struct Edge {
    int s, t;
    int c;
    Edge(int ss, int tt, int cc) : s(ss), t(tt), c(cc) { };

    int id; // e in F_{e->id}
    Edge *prev; // e is an alternative for e->prev
  } NIL;
  vector<Edge> edge;
  void addEdge(int s, int t, int c) {
    edge.push_back(Edge(s, t, c));
  }
  Graph(int nn) : n(nn), NIL(-1,-1,0) { NIL.prev = &NIL; }
  typedef vector< set<Edge*> > Forest; // complexity increased by log n factor
  vector<Forest> forest;

  // pick connected component which contains x, and orient edges to x
  void orientTree(int u, Forest &F, vector<Edge *> &p) {
    for(auto e:F[u]){
      if (e == p[u]) continue;
      if (e->s == u) swap(e->s, e->t);
      e->prev = 0; 
      p[e->s] = e;
      orientTree(e->s, F, p);
    }
  }
  bool augment(Edge *e0, int k) {
    int x = e0->s;
    vector< vector<Edge *> > parent(k, vector<Edge*>(n));
    rep(i, k) { 
      parent[i][x] = &NIL;
      orientTree(x, forest[i], parent[i]);
    }
    queue<Edge*> Q; Q.push(e0);
    while (!Q.empty()) { // find augment path 
      Edge *e = Q.front(); Q.pop();
      int u = e->s, v = e->t, j = (e->id+1) % k;
      if (!parent[j][u] || !parent[j][v]) { // e can be added to F_j
        forest[j][u].insert(e);
        forest[j][v].insert(e);
        e->id = j;
        for (Edge *f; (f = e->prev); e = f) { 
          f->id = (e->id+k-1)%k;
          forest[f->id][e->s].erase(e);
          forest[f->id][e->t].erase(e);
          forest[f->id][f->s].insert(f);
          forest[f->id][f->t].insert(f);
        }
        return true;
      } else { // e makes cycle -> enqueue circuit edges
        if (parent[j][u]->prev) swap(u, v);
        stack<Edge *> S; // near x edges first
        for (; !parent[j][u]->prev; u = parent[j][u]->t) S.push(parent[j][u]);
        for (; !S.empty(); S.pop()) { S.top()->prev = e; Q.push(S.top()); }
      }
    }
    return false;
  }
  static bool Compare(Edge*i, Edge*j) { return i->c > j->c; }
  int disjointForest(int k) {
    for(auto&e:edge){ e.id = -1; e.prev = 0; }
    vector<Edge*> ls(si(edge));
    rep(i,si(edge))ls[i]=&edge[i];
    sort(all(ls), Compare);
    forest = vector<Forest>(k, Forest(n));
    int cost = 0;
    unionfind clump(n); // clumps form disjoint sets
    for(auto i:ls){
      if (clump.same(i->s, i->t)) continue;
      if (augment(i, k)) cost += i->c;
      else clump.unite(i->s, i->t);
    }
    return cost;
  }
};
