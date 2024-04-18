//2N-1 頂点の根付き木ができる．
//葉が元のグラフの頂点に対応．
//各部分木が Extreme Vertex Set の候補に対応している．
//部分木から親に生えている辺のコストがカットのコスト．
//根の ID は 2N-2
//res[i]=(頂点 i の親, コスト)
vc<pi> extreme_vertex_set(int n,const vc<Edge>&es){
	for(auto [a,b,c]:es){
		assert(inc(0,a,n-1));
		assert(inc(0,b,n-1));
		assert(0<=c);
	}
	vc<pi> buf;
	auto push=[&](int v,int c){
		buf.eb(c,v);
		push_heap(all(buf),greater<pi>());
	};
	auto pop=[&](){
		pop_heap(all(buf),greater<pi>());
		return gpp(buf);
	};
	vc<pi> res(2*n-2);
	vi top=vid(n),cost(2*n-1),cur(2*n-1);
	vvc<pi> g(2*n-1);
	vc<bool> roots(2*n-1);
	rep(i,n)roots[i]=true;
	rep(phase,n-1){
		buf.clear();
		fila(cost,0);
		rep(i,2*n-1)g[i].clear();
		for(auto [a,b,c]:es){
			a=top[a];
			b=top[b];
			if(a!=b){
				cost[a]+=c;
				cost[b]+=c;
				g[a].eb(b,c);
				g[b].eb(a,c);
			}
		}
		rep(i,2*n-1)if(roots[i])push(i,cur[i]=cost[i]);
		int x=-1,y=-1;
		while(si(buf)){
			auto [c,v]=pop();
			if(cur[v]==-1)continue;
			cur[v]=-1;
			y=x;
			x=v;
			for(auto [to,w]:g[v])if(cur[to]!=-1)
				push(to,cur[to]-=w);
		}
		int z=n+phase;
		res[x]=pi(z,cost[x]);
		res[y]=pi(z,cost[y]);
		rep(i,n)if(top[i]==x||top[i]==y)
			top[i]=z;
		roots[x]=false;
		roots[y]=false;
		roots[z]=true;
	}
	return res;
}
