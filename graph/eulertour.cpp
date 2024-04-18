struct geteulertour{
	const vvc<int>&g;
	vi head,res;
	void dfs(int v){
		while(head[v]<si(g[v])){
			int to=g[v][head[v]++];
			dfs(to);
			res.pb(to);
		}
	}
	geteulertour(const vvc<int>&gg,int r):g(gg),head(si(g)){
		dfs(r);
		res.pb(r);
		reverse(all(res));
	}
};

//辺の番号順に addedge してこれを呼ぶと，使う辺の番号が辞書順で最小化される(Bytecamp 2022 Day5 E)
//CF CodeTON Round G
struct geteulertour{
	const vvc<pi>&g;
	vi head,res;
	void dfs(int v){
		while(head[v]<si(g[v])){
			auto [to,idx]=g[v][head[v]++];
			dfs(to);
			res.pb(idx);
		}
	}
	geteulertour(const vvc<pi>&gg,int r):g(gg),head(si(g)){
		dfs(r);
		reverse(all(res));
	}
};

//CF Pinely2 D
//undirected
struct geteulertour{
	vvc<pi> g;
	vi head,res;
	void dfs(int v){
		while(head[v]<si(g[v])){
			auto [to,idx]=g[v][head[v]++];
			if(res[idx/2]==-1){
				res[idx/2]=idx%2;
				dfs(to);
			}
		}
	}
	void ae(int a,int b){
		int m=si(res);
		g[a].eb(b,m*2);
		g[b].eb(a,m*2+1);
		res.pb(-1);
	}
	geteulertour(int n):g(n),head(n){}
	bool calc(){
		rep(i,si(g))if(si(g[i])%2)return false;
		rep(i,si(g))dfs(i);
		return true;
	}
};
