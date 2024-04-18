//パスが与えられたときにパス上の辺のうち適当なものを返す
//辺が与えられたときにその辺を含むパスのうち適当なものを返す
//といった操作でSCCなどができるようになる
//CF906E
struct edge_path{
	vvc<pi> t;
	vvc<int> ls;
	vc<pi> x,y;
	int s=0;
	bool mode;//true->x:edge,y:path
	void dfs(int v,int p,int e){
		if(p!=-1)
			x[e].a=s++;
		for(int f:ls[v])
			(y[f].a==-1?y[f].a:y[f].b)=s++;
		for(auto [ch,id]:t[v])if(ch!=p)
			dfs(ch,v,id);
		if(p!=-1)
			x[e].b=s++;
	}
	edge_path(int n,vc<pi> es,vc<pi> fs):t(n),ls(n),
		x(si(es),pi(-1,-1)),y(si(fs),pi(-1,-1)),mode(true){
		assert(si(es)==n-1);
		rep(i,n-1){
			auto [a,b]=es[i];
			t[a].eb(b,i);
			t[b].eb(a,i);
		}
		rep(i,si(fs)){
			auto [a,b]=fs[i];
			ls[a].pb(i);
			ls[b].pb(i);
		}
		dfs(0,-1,-1);
	}
	segtree<MaxNode> maxs;
	segtree<MinNode> mins;
	void init(bool path2edge){
		if(mode!=path2edge){
			swap(x,y);
			mode=path2edge;
		}
		vc<MaxNode> l2r(s);
		vc<MinNode> r2l(s);
		rep(i,si(x)){
			auto [a,b]=x[i];
			l2r[a]=pi(b,i);
			r2l[b]=pi(a,i);
		}
		maxs=segtree<MaxNode>(l2r);
		mins=segtree<MinNode>(r2l);
	}
	int getto(int i){
		assert(inc(0,i,si(y)-1));
		auto [l,r]=y[i];
		{
			auto [v,j]=maxs.composite(l,r).v;
			if(r<v)return j;
		}
		{
			auto [v,j]=mins.composite(l,r).v;
			if(v<l)return j;
		}
		return -1;
	}
	void del(int i){
		assert(inc(0,i,si(x)-1));
		auto [a,b]=x[i];
		maxs.point_set(a,MaxNode());
		mins.point_set(b,MinNode());
	}
};
