//s: the number of SCCs.
//bl[i]: SCC to which the vertex i belongs
//idx[i]: list of the vertices in the i-th SCC
//da: DAG of SCCs
//SCCs are numbered in topological order
//VERIFY: yosupo
//AOJGRL3C(except gr)
template<class t>
struct scc{
	const int n;
	const vvc<t>&g;
	vi ord,low,bl,st;
	int s,head;
	vvc<int> idx,da;
	void dfs(int v){
		ord[v]=low[v]=head++;
		st.pb(v);
		for(auto to:g[v]){
			if(ord[to]==-1){
				dfs(to);
				chmin(low[v],low[to]);
			}else if(bl[to]==-1)
				chmin(low[v],ord[to]);
		}
		if(ord[v]==low[v]){
			int c=idx.size();
			idx.eb();
			while(1){
				int a=st.back();
				st.pop_back();
				bl[a]=c;
				idx.back().pb(a);
				if(v==a)break;
			}
		}
	}
	scc(const vvc<t>&gg,bool cgr=true):n(gg.size()),g(gg),
		ord(n,-1),low(n,-1),bl(n,-1){
		head=0;
		rep(i,n)if(ord[i]==-1)
			dfs(i);
		s=idx.size();
		rep(i,n)
			bl[i]=s-1-bl[i];
		reverse(all(idx));
		//construct the graph
		if(cgr){
			vc<bool> u(s);
			da.resize(s);
			rep(i,s){
				for(auto v:idx[i]){
					for(auto to:g[v])if(bl[v]<bl[to]){
						if(!u[bl[to]])
							da[bl[v]].pb(bl[to]);
						u[bl[to]]=1;
					}
				}
				for(auto v:idx[i])
					for(auto to:g[v])
						u[bl[to]]=0;
			}
		}
	}
};
struct E{
	int to,cost;
	operator int()const{return to;}
};

//軽量
template<class t>
struct scc{
	const int n;
	const vvc<t>&g;
	vi ord,low,bl,st;
	int s,head;
	void dfs(int v){
		ord[v]=low[v]=head++;
		st.pb(v);
		for(auto to:g[v]){
			if(ord[to]==-1){
				dfs(to);
				chmin(low[v],low[to]);
			}else if(bl[to]==-1)
				chmin(low[v],ord[to]);
		}
		if(ord[v]==low[v]){
			int c=s++;
			while(1){
				int a=st.back();
				st.pop_back();
				bl[a]=c;
				if(v==a)break;
			}
		}
	}
	scc(const vvc<t>&gg):n(gg.size()),g(gg),
		ord(n,-1),low(n,-1),bl(n,-1),s(0),head(0){
		rep(i,n)if(ord[i]==-1)
			dfs(i);
		rep(i,n)
			bl[i]=s-1-bl[i];
	}
};
