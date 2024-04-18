template<class E>
vi bfs(const vvc<E>&g,int r){
	int n=si(g);
	vi dist(n,-1);
	vi q(n);
	int b=0,e=0;
	auto reach=[&](int v,int d){
		if(dist[v]!=-1)return;
		dist[v]=d;
		q[e++]=v;
	};
	reach(r,0);
	while(b<e){
		int v=q[b++];
		for(auto to:g[v])
			reach(to,dist[v]+1);
	}
	return dist;
}

//CF869B
//複数始点
template<class E>
tuple<vi,vi,vc<E>> bfs(const vvc<E>&g,vi rs){
	int n=si(g);
	vi dist(n,-1),from(n,-1);
	vc<E> use(n);
	vi q(n);
	int b=0,e=0;
	auto reach=[&](int v,int d,int f,E u){
		if(dist[v]!=-1)return;
		dist[v]=d;
		from[v]=f;
		use[v]=u;
		q[e++]=v;
	};
	for(auto r:rs)reach(r,0,-1,E());
	while(b<e){
		int v=q[b++];
		for(auto to:g[v])
			reach(to,dist[v]+1,v,to);
	}
	return mt(move(dist),move(from),move(use));
}

vi getpath(const vi&from,int x){
	vi res{x};
	while(from[x]!=-1){
		x=from[x];
		res.pb(x);
	}
	reverse(all(res));
	return res;
}

vi getpath(const vvc<int>&g,int s,int t){
	auto [dist,from,use]=bfs(g,vi{s});
	return getpath(from,t);
}
