//sz: the size of the max matching
//VERIFY: yosupo
template<class E>
struct bipartitematching{
	int n,m;
	vvc<E> g;
	vi dist,vis;
	vi to,ot,q;
	int sz;
	bipartitematching(int nn,int mm):n(nn),m(mm),g(n),
	dist(n),vis(n),to(n,-1),ot(m,-1),sz(0){}
	void ae(int a,E e){
		g[a].pb(e);
	}
	void bfs(){
		fill(all(dist),-1);
		q.clear();
		int h=0;
		auto rc=[&](int v,int d){
			if(v==-1||dist[v]!=-1)return;
			dist[v]=d;
			q.pb(v);
		};
		rep(i,n)if(to[i]==-1)rc(i,0);
		while(h<(int)q.size()){
			int v=q[h++];
			for(auto e:g[v])
				rc(ot[e],dist[v]+1);
		}
	}
	bool dfs(int v){
		if(vis[v])return 0;
		vis[v]=1;
		for(auto e:g[v]){
			if(ot[e]==-1||(dist[v]+1==dist[ot[e]]&&dfs(ot[e]))){
				to[v]=e;
				ot[e]=v;
				return 1;
			}
		}
		return 0;
	}
	void calc(){
		while(1){
			bfs();
			fill(all(vis),0);
			int f=0;
			rep(i,n)if(to[i]==-1)
				if(dfs(i))f++;
			if(f==0)break;
			sz+=f;
		}
	}
};

//ord の順に左側頂点を優先してマッチングに組み込もうとする
//PTZ Day 4, ICPC/Shanghai Contest 1 (PKU) I
//sz: the size of the max matching
//VERIFY: yosupo
template<class E>
struct bipartitematching{
	int n,m;
	vvc<E> g;
	vi vis;
	vi to,ot;
	int sz;
	bipartitematching(int nn,int mm):n(nn),m(mm),g(n),
	vis(n),to(n,-1),ot(m,-1),sz(0){}
	void ae(int a,E e){
		g[a].pb(e);
	}
	bool dfs(int v){
		if(vis[v])return 0;
		vis[v]=1;
		for(auto e:g[v]){
			if(ot[e]==-1||dfs(ot[e])){
				to[v]=e;
				ot[e]=v;
				return 1;
			}
		}
		return 0;
	}
	void calc(const vi&ord){
		for(auto i:ord){
			if(dfs(i)){
				sz++;
				fill(all(vis),0);
			}
		}
	}
};
