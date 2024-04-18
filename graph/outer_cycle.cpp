//全体がサイクルになっている形の outerplanar グラフかどうかを判定
//ダメなら {} を返す
//大丈夫なら p[v] = 頂点 v がサイクルで何番目に位置しているか を返す
//p[0]=0
//CF CodeTON Round I
vi outer_cycle(int n,vc<pi> es){
	vc<set<int>> g(n);
	for(auto [a,b]:es){
		g[a].insert(b);
		g[b].insert(a);
	}
	vc<bool> added(n);
	vi ls;
	auto add=[&](int i){
		if(!added[i]&&si(g[i])==2){
			added[i]=true;
			ls.pb(i);
		}
	};
	rep(i,n)add(i);
	vc<pi> nb(n,pi(-1,-1));
	vi idx;
	rep(_,n-2){
		if(ls.empty())return {};
		int v=ls.back();ls.pop_back();
		if(si(g[v])!=2)return {};
		auto itr=g[v].bg;
		int a=*itr;
		++itr;
		int b=*itr;
		idx.pb(v);
		nb[v]=pi(a,b);
		g[a].erase(v);
		g[b].erase(v);
		g[a].insert(b);
		g[b].insert(a);
		add(a);
		add(b);
	}
	vi pr(n,-1),nx(n,-1);
	auto link=[&](int a,int b){
		nx[a]=b;
		pr[b]=a;
	};
	{
		int a=-1,b=-1;
		rep(i,n)if(nb[i]==pi(-1,-1)){
			if(a==-1)a=i;
			else{
				assert(b==-1);
				b=i;
			}
		}
		assert(b!=-1);
		link(a,b);
		link(b,a);
	}
	reverse(all(idx));
	for(auto v:idx){
		auto [a,b]=nb[v];
		if(nx[a]!=b)swap(a,b);
		if(nx[a]!=b)return {};
		link(a,v);
		link(v,b);
	}
	vi p(n);
	for(int cur=0,ord=0;ord<n;cur=nx[cur],ord++){
		p[cur]=ord;
	}
	return p;
}
