//Lyft Level 5 Challenge 2018 - Final F
//CF635F
template<class E>
struct doubling{
	const vvc<E>&g;
	const int n,h;
	int cnt;
	vvc<int> par;
	vi dep,in,out;
	void dfs(int v,int p,int d){
		par[0][v]=p;
		dep[v]=d;
		in[v]=cnt++;
		for(auto e:g[v])if(e!=p)
			dfs(e,v,d+1);
		out[v]=cnt;
	}
	doubling(const vvc<E>&gg,int r):g(gg),n(g.size()),h(__lg(n)+1),
		cnt(0),par(h,vi(n,-1)),dep(n),in(n),out(n){
		dfs(r,-1,0);
		rng(i,1,h){
			rep(j,n)
				if(par[i-1][j]!=-1)
					par[i][j]=par[i-1][par[i-1][j]];
		}
	}
	int lca(int a,int b){
		assert(0<=a&&a<(int)g.size());
		assert(0<=b&&b<(int)g.size());
		if(dep[a]>dep[b])swap(a,b);
		int w=dep[b]-dep[a];
		rep(i,h)if(w&1<<i)
			b=par[i][b];
		if(a==b)return a;
		per(i,h){
			int x=par[i][a],y=par[i][b];
			if(x!=y)tie(a,b)=pi(x,y);
		}
		return par[0][a];
	}
	int len(int a,int b){
		assert(0<=a&&a<(int)g.size());
		assert(0<=b&&b<(int)g.size());
		return dep[a]+dep[b]-dep[lca(a,b)]*2;
	}
	int jump(int a,int b,int d){
		assert(0<=a&&a<(int)g.size());
		assert(0<=b&&b<(int)g.size());
		int c=lca(a,b);
		int w=dep[a]+dep[b]-dep[c]*2;
		assert(0<=d&&d<=w);
		if(d<=dep[a]-dep[c]){
			rep(i,h)if(d&1<<i)
				a=par[i][a];
			return a;
		}else{
			d=w-d;
			rep(i,h)if(d&1<<i)
				b=par[i][b];
			return b;
		}
	}
	int getpar(int v,int len)const{
		rep(i,h)if(len&1<<i)v=par[i][v];
		return v;
	}
};
