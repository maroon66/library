struct E{
	int a,b,c;
};
pair<int,vi> mst(int n,vc<E> es){
	unionfind uf(n);
	vi idx(es.size());
	iota(all(idx),0);
	sort(all(idx),[&](int i,int j){return es[i].c<es[j].c;});
	int ans=0;
	vi used;
	for(auto i:idx)
		if(uf.unite(es[i].a,es[i].b)){
			ans+=es[i].c;
			used.pb(i);
		}
	assert(uf.c==1);
	return {ans,used};
}

//almost same as
//https://github.com/bqi343/USACO/blob/master/Implementations/kactl.pdf
//VERIFY: yosupo
vc<E> manhattanmst(vc<pi> ps){
	vc<E> res;
	rep(_,2){
		rep($,2){
			map<int,int> z;
			vi idx(ps.size());
			iota(all(idx),0);
			sort(all(idx),[&](int i,int j){return ps[i].a+ps[i].b<ps[j].a+ps[j].b;});
			for(auto i:idx){
				for(auto itr=z.lower_bound(-ps[i].b);itr!=z.ed;itr=z.erase(itr)){
					int j=itr->b;
					int x=ps[i].a-ps[j].a;
					int y=ps[i].b-ps[j].b;
					if(y>x)break;
					res.pb({i,j,x+y});
				}
				z[-ps[i].b]=i;
			}
			for(auto&p:ps)
				swap(p.a,p.b);
		}
		for(auto&p:ps)
			p.a*=-1;
	}
	return res;
}
