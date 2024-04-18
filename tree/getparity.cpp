template<class E>
vi getparity(const vvc<E>&t){
	int n=si(t);
	vi res(n,-1);
	auto dfs=[&](auto self,int v,int d)->void{
		if(res[v]!=-1){
			assert(res[v]==d);
			return;
		}
		res[v]=d;
		for(auto to:t[v])self(self,to,d^1);
	};
	dfs(dfs,0,0);
	return res;
}
