//UCUP 2-26-E
//CCPC Finals 2021 H (二部のケースだけ)
template<class E>
pair<bool,vi> bipartite_partition(const vvc<E>&g){
	int n=si(g);
	bool ok=true;
	vi res(n,-1);
	auto dfs=[&](auto self,int v,int d)->void{
		if(res[v]!=-1){
			if(res[v]!=d)ok=false;
			return;
		}
		res[v]=d;
		for(auto to:g[v])self(self,to,d^1);
	};
	rep(i,n)if(res[i]==-1)dfs(dfs,i,0);
	if(ok)return mp(true,res);
	else return mp(false,vi{});
}

//連結成分の個数,01
//無理なら -1
//ARC165 C
pair<int,vi> bipartite(const vvc<int>&g){
	int n=si(g),c=0;
	vi res(n,-1);
	auto dfs=[&](auto self,int v,int d)->void{
		if(res[v]!=-1){
			if(res[v]!=d)c=-1;
			return;
		}
		res[v]=d;
		for(auto to:g[v])self(self,to,d^1);
	};
	rep(i,n)if(c>=0&&res[i]==-1){
		c++;
		dfs(dfs,i,0);
	}
	if(c>=0)return mp(c,move(res));
	else return mp(c,vi{});
}

//連結成分の個数,01
//無理なら -1
pair<int,vi> bipartite(const vvc<pi>&g){
	int n=si(g),c=0;
	vi res(n,-1);
	auto dfs=[&](auto self,int v,int d)->void{
		if(res[v]!=-1){
			if(res[v]!=d)c=-1;
			return;
		}
		res[v]=d;
		for(auto [to,f]:g[v])self(self,to,d^f);
	};
	rep(i,n)if(c>=0&&res[i]==-1){
		c++;
		dfs(dfs,i,0);
	}
	if(c>=0)return mp(c,res);
	else return mp(c,vi{});
}
