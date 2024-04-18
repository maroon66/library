//assume g is biconnected
pair<vi,vi> disjoint_paths(vvc<int> g,int a,int b){
	int n=si(g);
	maxflow<int> mf(2*n);
	rep(i,n){
		mf.ae(i*2,i*2+1,1);
		for(auto j:g[i]){
			mf.ae(i*2+1,j*2,1);
		}
	}
	int tmp=mf.calc(b*2+1,a*2,2);
	assert(tmp==2);
	vvc<int> pre(n);
	rep(i,n)for(auto e:mf.g[i*2])if(e.cap>0){
		int j=e.to/2;
		if(i!=j){
			pre[i].pb(j);
		}
	}
	assert(si(pre[a])==2);
	vi x,y;
	rep(i,2){
		vi z{a};
		int cur=pre[a][i];
		while(1){
			z.pb(cur);
			if(cur==b)break;
			assert(si(pre[cur])==1);
			cur=pre[cur][0];
		}
		if(i==0)x=z;
		else y=z;
	}
	return mp(x,y);
}
