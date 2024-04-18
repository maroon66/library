//verify: yosupo
unionfind threedge(const vvc<int>&g){
	int n=si(g);
	vi in(n,-1),out(n,-1),low(n),up(n),nx(n,-1);
	int s=0;
	unionfind uf(n);
	auto mg=[&](int a,int b){
		uf.unite(a,b);
		up[a]+=up[b];
	};
	auto dfs=[&](auto self,int v,int p)->void{
		assert(in[v]==-1);
		in[v]=low[v]=s++;
		for(auto to:g[v]){
			if(to==v)continue;
			if(to==p){
				p=-1;
				continue;
			}
			if(in[to]==-1){
				self(self,to,v);
				if(nx[to]==-1&&up[to]<=1){
					up[v]+=up[to];
					chmin(low[v],low[to]);
					continue;
				}
				if(up[to]==0){
					to=nx[to];
				}
				if(chmin(low[v],low[to])){
					swap(nx[v],to);
				}
				while(to!=-1){
					mg(v,to);
					to=nx[to];
				}
			}else if(in[to]<in[v]){
				chmin(low[v],in[to]);
				up[v]++;
			}else{
				for(int&x=nx[v];x!=-1&&in[x]<=in[to]&&in[to]<out[x];x=nx[x])
					mg(v,x);
				up[v]--;
			}
		}
		out[v]=s;
	};
	rep(i,n)if(in[i]==-1)dfs(dfs,i,-1);
	return uf;
}
