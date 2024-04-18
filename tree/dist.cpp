struct E{
	int to,cost;
	operator int()const{return to;}
};

vi tree_dist(const vvc<E>&t,int root){
	int n=si(t);
	vi res(n);
	auto dfs=[&](auto self,int v,int p,int d)->void{
		res[v]=d;
		for(auto [to,cost]:t[v])if(to!=p)
			self(self,to,v,d+cost);
	};
	dfs(dfs,root,-1,0);
	return res;
}
