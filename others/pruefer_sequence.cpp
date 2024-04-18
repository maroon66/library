
auto restore(vi a){
	int n=a.size()+2;
	vi d(n);
	rep(i,n-2)d[a[i]]++;
	priority_queue<int,vi,greater<int>> pq;
	rep(i,n)if(d[i]==0)pq.push(i);
	vvc<int> g(n);
	vc<pi> es;
	auto ae=[&](int i,int j){
		es.eb(i,j);
		g[i].pb(j);
		g[j].pb(i);
	};
	vi e(n,1);
	rep(i,n-2){
		int x=pq.top();pq.pop();
		e[x]=0;
		ae(x,a[i]);
		if(--d[a[i]]==0)pq.push(a[i]);
	}
	int x=-1,y=-1;
	rep(i,n)if(e[i]){
		if(x==-1)x=i;
		else y=i;
	}
	ae(x,y);
	//return g;
	return es;
}
