
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

template<class F>
void tree_rec(int n,F f){
	assert(n>=2);
	array_rec(n-2,n,[&](const vi&a){
		auto g=restore(a);
		f(g);
	});
}

vc<ull> get_trees(int n){
	assert(n>0);
	if(n==1)return vc<ull>{0};
	vc<ull> ls;
	array_rec(n-2,n,[&](vi a){
		auto es=restore(a);
		ull z=0;
		for(auto [i,j]:es){
			z|=1ull<<(i*n+j);
			z|=1ull<<(j*n+i);
		}
		ls.pb(z);
	});
	soin(ls);
	return ls;
}
