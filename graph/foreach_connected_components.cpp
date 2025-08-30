//multiuni 2025-10-L
template<class F>
void foreach_connected_components(const vvc<int>&g,F f){
	int n=si(g);
	vi id(n,-1);
	vi vs;
	auto dfs=[&](auto self,int v)->void{
		if(id[v]!=-1)return;
		id[v]=si(vs);
		vs.pb(v);
		for(auto to:g[v])self(self,to);
	};
	vvc<int> h(n);
	rep(root,n)if(id[root]==-1){
		vs.clear();
		dfs(dfs,root);
		
		int s=si(vs);
		h.resize(s);
		rep(i,s){
			h[i].clear();
			for(auto j:g[vs[i]])
				h[i].pb(id[j]);
		}
		f(vs,h);
	}
}
