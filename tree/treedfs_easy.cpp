//CF1024 E (TLE)
template<class E>
vi tree_sub(const vvc<E>&t,int root){
	int n=si(t);
	vi sub(n,1);
	auto dfs=[&](auto self,int v,int p)->int{
		for(auto ch:t[v])if(ch!=p)sub[v]+=self(self,ch,v);
		return sub[v];
	};
	dfs(dfs,root,-1);
	return sub;
}
template<class E>
vi tree_dep(const vvc<E>&t,int root){
	int n=si(t);
	vi dep(n);
	auto dfs=[&](auto self,int v,int p,int d)->void{
		dep[v]=d;
		for(auto ch:t[v])if(ch!=p)self(self,ch,v,d+1);
	};
	dfs(dfs,root,-1,0);
	return dep;
}
