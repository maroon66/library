vi make_rooted(vvc<int>&t,int r){
	vi par(si(t),-1);
	auto dfs=[&](auto self,int v,int p)->void{
		par[v]=p;
		t[v].erase(remove(all(t[v]),p),t[v].ed);
		for(auto ch:t[v])self(self,ch,v);
	};
	dfs(dfs,r,-1);
	return par;
}

//assume rooted
template<class E>
void make_hld(vvc<E>&t,int root){
	auto dfs=[&](auto self,int v)->int{
		int res=1,best=0;
		for(auto&ch:t[v]){
			int w=self(self,ch);
			res+=w;
			if(chmax(best,w))swap(t[v][0],ch);
		}
		return res;
	};
	dfs(dfs,root);
}
