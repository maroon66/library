persistent_heap<pi,greater<pi>,500*ten(6)> ph;

struct E{
	int to,cost;
	operator int()const{return to;}
};
vi k_shortest_walk(const vvc<E>&g,int s,int t,int k){
	int n=si(g);
	using np=decltype(ph)::np;
	auto [dij,par]=dijkstra(g,s);
	vc<np> ls(n);
	vc<bool> done(n);done[s]=true;
	rep(i,n)if(dij[i]<infLL)for(auto e:g[i]){
		int d=dij[i]+e.cost-dij[e.to];
		if(d==0&&par[e.to]==i&&!done[e.to]){
			done[e.to]=true;
			continue;
		}
		ls[e.to]=ph.merge(ls[e.to],ph.nn(pi(d,i),0,0));
	}
	{
		vvc<int> h(n);
		rep(i,n)if(par[i]!=-1)h[par[i]].pb(i);
		auto dfs=[&](auto self,int v)->void{
			for(auto c:h[v]){
				ls[c]=ph.merge(ls[c],ls[v]);
				self(self,c);
			}
		};
		dfs(dfs,s);
	}
	int base=dij[t];
	if(base==infLL)return vi(k,-1);
	vi ans;
	using P=pair<np,int>;
	auto val=[&](np a,int b){return a->v.a+b;};
	auto cmp=[&](P x,P y){return val(x.a,x.b)>val(y.a,y.b);};
	priority_queue<P,vc<P>,decltype(cmp)> pq(cmp);
	auto reach=[&](int v,int d){
		ans.pb(d+base);
		if(ls[v])pq.emplace(ls[v],d);
	};
	reach(t,0);
	while(si(ans)<k&&si(pq)){
		auto [a,b]=pq.top();pq.pop();
		reach(a->v.b,val(a,b));
		if((a=ph.pop(a)))
			pq.emplace(a,b);
	}
	ans.resize(k,-1);
	return ans;
}
