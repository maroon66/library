//dep,par は not verified
//yukicoder no.1024
//https://www.codechef.com/LTIME110A/problems/TREEQUER
//IOI2022 Islands
template<class E>
struct treedfs{
	const vvc<E>&g;
	const int n;
	int cnt;
	vi par,dep,in,out,ni,sub;
	void dfs(int v,int p,int d){
		par[v]=p;
		dep[v]=d;
		in[v]=cnt++;
		for(auto e:g[v])if(e!=p)
			dfs(e,v,d+1);
		out[v]=cnt;
	}
	treedfs(const vvc<E>&gg,int r):g(gg),n(si(g)),cnt(0),par(n),dep(n,-1),
		in(n,-1),out(n,-1),sub(n){
		dfs(r,-1,0);
		//not necessarily connected
		ni.resize(cnt);
		rep(i,n)if(in[i]!=-1)ni[in[i]]=i;
		rep(i,n)sub[i]=out[i]-in[i];
	}
	bool asde(int a,int b){
		return in[a]<=in[b]&&out[b]<=out[a];
	}
};
