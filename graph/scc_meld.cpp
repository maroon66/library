//辺を追加して全部の連結成分が強連結になるようにする
//辺の個数の最小を構成

//ICPC 2021 Domestic F (AOJ1653)
//各連結成分が強連結ならばよい
//複数個の成分に分かれていてもかまわない
vc<pi> make_scc_dag(const vvc<int>&g){
	int n=si(g);
	vc<bool> vis(n);
	vc<bool> st(n,true);
	rep(i,n)for(auto j:g[i])st[j]=false;
	auto dfs=[&](auto self,int v)->int{
		if(vis[v])return -1;
		vis[v]=true;
		if(g[v].empty())return v;
		for(auto to:g[v]){
			int z=self(self,to);
			if(z!=-1){
				return z;
			}
		}
		return -1;
	};
	vc<pi> ls;
	rep(i,n)if(st[i]){
		int j=dfs(dfs,i);
		if(j!=-1){
			ls.eb(i,j);
		}
	}
	vc<bool> used(n);
	for(auto [i,j]:ls){
		used[i]=true;
		used[j]=true;
	}
	{
		vc<pi> tmp;
		for(auto [i,j]:ls)if(i!=j)
			tmp.eb(i,j);
		ls.swap(tmp);
	}
	vc<pi> ans;
	rep(i,si(ls)){
		ans.eb(ls[i].b,ls[(i+1)%si(ls)].a);
	}
	vi x,y;
	rep(i,n)if(!used[i]){
		if(st[i])x.pb(i);
		if(g[i].empty())y.pb(i);
	}
	if(si(x)<si(y))x.resize(si(y),ls[0].a);
	if(si(y)<si(x))y.resize(si(x),ls[0].b);
	rep(i,si(x))ans.eb(y[i],x[i]);
	return ans;
}

//ICPC 2021 Domestic F (AOJ1653)
//各連結成分が強連結ならばよい
//複数個の成分に分かれていてもかまわない
vc<pi> make_scc(const vvc<int>&g){
	scc<int> s(g);
	auto z=make_scc_dag(s.da);
	vc<pi> res;
	for(auto [a,b]:z)
		res.eb(s.idx[a][0],s.idx[b][0]);
	return res;
}
