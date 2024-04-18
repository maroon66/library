using B=bitset<510>;
//UCUP2-5-E
struct bipartite64{
	const int n,m;
	vc<B> g;
	vi to,ot;
	int sz;
	bipartite64(int nn,int mm):n(nn),m(mm),g(n),to(n,-1),ot(m,-1),sz(0){}
	void clear(){
		rep(i,n)g[i].reset();
		fill(all(to),-1);
		fill(all(ot),-1);
		sz=0;
	}
	void ae(int i,int j){
		assert(inc(0,i,n-1));
		assert(inc(0,j,m-1));
		g[i][j]=true;
	}
	void match(int i,int j){
		to[i]=j;
		ot[j]=i;
	}
	B Runvis;
	bool dfs(int v){
		while(1){
			int x=(Runvis&g[v])._Find_first();
			if(x<m){
				Runvis[x]=false;
				if(ot[x]==-1||dfs(ot[x])){
					match(v,x);
					return true;
				}
			}else break;
		}
		return false;
	}
	void calc(){
		Runvis.reset();
		rep(j,m)if(ot[j]==-1)Runvis[j]=true;
		rep(i,n)if(to[i]==-1){
			int j=(Runvis&g[i])._Find_first();
			if(j<m){
				Runvis[j]=false;
				match(i,j);
				sz++;
			}
		}
		rep(i,n)if(to[i]==-1){
			Runvis.set();
			if(dfs(i))sz++;
		}
	}
};



//bitset?
//thanks maspy

// vc<bitset> で biadj matrix を渡す
// N^3/w. (5000,5000) で 300ms 程度で動く場合がある
// https://qoj.ac/problem/6308
// (10000, 20000) で 3837ms
// https://codeforces.com/contest/786/problem/E
template <typename BS>
struct BipartiteMatching_Dense {
  int N1, N2;
  vc<BS>& adj;
  vc<int> match_1, match_2;
  vc<int> que;
  vc<int> prev;
  BS vis;
 
  BipartiteMatching_Dense(vc<BS>& adj, int N1, int N2)
      : N1(N1), N2(N2), adj(adj), match_1(N1, -1), match_2(N2, -1) {
    FOR(s, N1) bfs(s);
  }
 
  void bfs(int s) {
    if (match_1[s] != -1) return;
    que.resize(N1), prev.resize(N1);
    int l = 0, r = 0;
    vis.set(), prev[s] = -1;
 
    que[r++] = s;
    while (l < r) {
      int u = que[l++];
      BS cand = vis & adj[u];
      for (int v = cand._Find_first(); v < N2; v = cand._Find_next(v)) {
        vis[v] = 0;
        if (match_2[v] != -1) {
          que[r++] = match_2[v];
          prev[match_2[v]] = u;
          continue;
        }
        int a = u, b = v;
        while (a != -1) {
          int t = match_1[a];
          match_1[a] = b, match_2[b] = a, a = prev[a], b = t;
        }
        return;
      }
    }
    return;
  }
 
  vc<pair<int, int>> matching() {
    vc<pair<int, int>> res;
    FOR(v, N1) if (match_1[v] != -1) res.eb(v, match_1[v]);
    return res;
  }
 
  pair<vc<int>, vc<int>> vertex_cover() {
    vc<int> que(N1);
    int l = 0, r = 0;
    vis.set();
    vc<bool> done(N1);
    FOR(i, N1) {
      if (match_1[i] == -1) done[i] = 1, que[r++] = i;
    }
    while (l < r) {
      int a = que[l++];
      BS cand = adj[a] & vis;
      for (int b = cand._Find_first(); b < N2; b = cand._Find_next(b)) {
        vis[b] = 0;
        int to = match_2[b];
        assert(to != -1);
        if (!done[to]) done[to] = 1, que[r++] = to;
      }
    }
    vc<int> left, right;
    FOR(i, N1) if (!done[i]) left.eb(i);
    FOR(i, N2) if (!vis[i]) right.eb(i);
    return {left, right};
  }
};
#line 5 "main.cpp"
 
using BS = bitset<3000>;
