//UCUP 3-29-J
	INT(n,q);
	vc<R> rs(n);
	rep(i,n)rs[i].readinit();
	vvc<int> t(n+1);
	vi par(n+1,-1);
	auto ae=[&](int v,int p){
		t[p].pb(v);
		par[v]=p;
	};
	{
		vc<pi> ys;
		rep(i,n)ys.eb(rs[i].yl,i);
		soin(ys);
		vi idx=vid(n);
		soin(idx,[&](int i,int j){return rs[i].xr<rs[j].xr;});
		segtree<MaxNode> seg(n);
		for(auto i:idx){
			int l=lwb(ys,pi(rs[i].yl,-1));
			int r=lwb(ys,pi(rs[i].yr,-1));
			while(1){
				auto [x,j]=seg.composite(l,r).v;
				if(j==-1)break;
				if(x<=rs[i].xl)break;
				seg.point_set(j,MaxNode());
				ae(ys[j].b,i);
			}
			int j=lwb(ys,pi(rs[i].yl,i));
			seg.point_set(j,pi(rs[i].xr,j));
		}
		rep(i,n)if(par[i]==-1)ae(i,n);
	}
