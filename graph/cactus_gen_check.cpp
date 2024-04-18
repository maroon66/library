bool is_cactus(int n,vc<pi> es){
	unionfind uf(n);
	for(auto [a,b]:es){
		uf.unite(a,b);
	}
	if(uf.c>1)return false;
	vvc<int> g(n);
	for(auto [a,b]:es){
		g[a].pb(b);
		g[b].pb(a);
	}
	bool cac=true;
	vc<bool> used(n);
	vi par(n,-1),vis(n);
	auto dfs=[&](auto self,int v,int p)->void{
		assert(vis[v]==0);
		vis[v]=1;
		par[v]=p;
		for(auto to:g[v])if(to!=p){
			if(vis[to]==0){
				self(self,to,v);
			}else if(vis[to]==1){
				int x=v;
				while(x!=to){
					if(used[x])cac=false;
					used[x]=true;
					x=par[x];
				}
			}else{
				//do nothing
			}
		}
		vis[v]=2;
	};
	dfs(dfs,0,-1);
	return cac;
}

//gen NERC 2023-2 C
vc<pi> cactus_gen(int n,int m){
	int C=m-(n-1);
	vi ls;
	{
		int u=0;
		if(C){
			u=rand_int(C*3,m);
			ls=rand_vi_givensum(C,u,3);
		}
		rep(_,m-u)ls.pb(2);
		myshuffle(ls);
		C=si(ls);
	}
	vc<pi> es;
	vi cut=presum(ls);
	rep(i,C){
		int len=ls[i];
		if(len==2){
			es.eb(cut[i],cut[i]+1);
		}else{
			rep(j,len){
				es.eb(cut[i]+j,cut[i]+(j+1)%len);
			}
		}
	}
	assert(si(es)==m);
	auto gv=[&](int i){
		int len=ls[i];
		return cut[i]+rand_int(len);
	};
	int V=cut[C];
	unionfind uf(V);
	rng(i,1,C){
		int j=rand_int(i);
		uf.unite(gv(i),gv(j));
	}
	vi tar(V,-1);
	vi perm=vid(n);myshuffle(perm);
	int used=0;
	rep(i,V)if(tar[uf.find(i)]==-1){
		tar[uf.find(i)]=perm[used++];
	}
	assert(used==n);
	vc<pi> res;
	for(auto [a,b]:es){
		a=tar[uf.find(a)];
		b=tar[uf.find(b)];
		res.eb(a,b);
	}
	for(auto&[a,b]:res){
		if(a>b)swap(a,b);
	}
	soin(res);
	return res;
}
