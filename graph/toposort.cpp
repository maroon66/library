//AOJGRL4B
template<class t>
vi toposort(vvc<t> g){
	int n=g.size();
	vi a(n);
	rep(i,n)for(auto e:g[i])
		a[e]++;
	queue<int> q;
	rep(i,n)if(a[i]==0)
		q.push(i);
	vi res;
	rep(i,n){
		if(q.empty())return {};
		int v=q.front();q.pop();
		res.pb(v);
		for(auto e:g[v])
			if(--a[e]==0)
				q.push(e);
	}
	return res;
}


template<class E>
bool isdag(const vvc<E>&g){
	int n=si(g);
	vi vis(n);
	bool res=true;
	auto dfs=[&](auto self,int v)->void{
		assert(vis[v]==0);
		vis[v]=1;
		for(auto to:g[v]){
			if(vis[to]==0){
				self(self,to);
			}else if(vis[to]==1){
				res=false;
			}else{
				//do nothing
			}
		}
		vis[v]=2;
	};
	rep(i,n)if(!vis[i])dfs(dfs,i);
	return res;
}
