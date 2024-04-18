//GCJ2020 Qual E

//左右 N 頂点ずつ
//各頂点の次数 C
//右側頂点の番号が N 足されているから，g を参照するときは注意が必要
struct bipartite_edgecoloring{
	int n,c;
	//g[i][j] = 頂点 i に接続する色 j の辺の行き先
	vvc<int> g;
	bipartite_edgecoloring(int nn,int cc):n(nn),c(cc),g(n*2,vi(c,-1)){}
	//left(i) - right(j)
	void add(int i,int j){
		j+=n;
		rep(k,c){
			if(g[i][k]==-1&&g[j][k]==-1){
				g[i][k]=j;
				g[j][k]=i;
				return;
			}
		}
		int x=find(all(g[i]),-1)-g[i].bg;
		int y=find(all(g[j]),-1)-g[j].bg;
		int cur=j,nx=x,pre=y;
		g[j][y]=i;
		while(cur!=-1){
			int to=g[cur][nx];
			swap(g[cur][nx],g[cur][pre]);
			swap(nx,pre);
			cur=to;
		}
		g[i][x]=j;
	};
};

//CF591F (N=M の場合だけ verify)
//N+M 頂点のグラフがあって，C 色に塗る
//どの頂点も，接続する色がだいたい同じ（個数の差が 1 以内）
struct bipartite_edgecoloring_general{
	int n,m,c;
	vvc<multiset<int>> g;
	vi deg;
	bipartite_edgecoloring_general(int nn,int mm,int cc):
		n(nn),m(mm),c(cc),g(n+m,vc<multiset<int>>(c)),deg(n+m){}
	bool can(int i,int j){
		return si(g[i][j])<(deg[i]+c-1)/c;
	}
	void add(int i,int j){
		j+=n;
		deg[i]++;
		deg[j]++;
		int x=-1,y=-1;
		rep(k,c){
			if(can(i,k))x=k;
			if(can(j,k))y=k;
			if(x==k&&y==k){
				g[i][k].insert(j);
				g[j][k].insert(i);
				return;
			}
		}
		vc<multiset<int>::iterator> cur;
		rep(k,n)cur.pb(g[k][y].bg);
		rep(k,m)cur.pb(g[k+n][x].bg);
		auto rec=[&](auto self,int v)->void{
			int nx=y,pre=x;
			if(v>=n)swap(nx,pre);
			if(si(g[v][nx])<si(g[v][pre]))return;
			int to=*cur[v]++;
			self(self,to);
			g[v][nx].erase(g[v][nx].find(to));
			g[v][pre].insert(to);
			g[to][nx].erase(g[to][nx].find(v));
			g[to][pre].insert(v);
		};
		rec(rec,j);
		g[i][x].insert(j);
		g[j][x].insert(i);
	}
};
