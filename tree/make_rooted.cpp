void make_rooted(vvc<int>&t,int r){
	auto dfs=[&](auto self,int v,int p)->void{
		t[v].erase(remove(all(t[v]),p),t[v].ed);
		for(auto ch:t[v])self(self,ch,v);
	};
	dfs(dfs,r,-1);
}
