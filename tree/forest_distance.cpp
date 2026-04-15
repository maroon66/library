//CF1092G
struct Forest_Distance{
	int n;
	vvc<int> t;
	HLD<int> hld;
	Forest_Distance(int nn,vc<pi> es):n(nn),t(n+1){
		unionfind uf(n);
		for(auto [i,j]:es){
			t[i].pb(j);
			t[j].pb(i);
			uf.unite(i,j);
		}
		rep(i,n)if(uf.find(i)==i){
			t[n].pb(i);
			t[i].pb(n);
		}
		hld=HLD<int>(t,n);
	}
	int dist(int i,int j){
		assert(inc(0,i,n-1));
		assert(inc(0,j,n-1));
		int k=hld.lca(i,j);
		if(k==n)return -1;
		return hld.dep[i]+hld.dep[j]-hld.dep[k]*2;
	}
};
