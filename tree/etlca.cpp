//AOJGRL5C
//Universal Cup 2023 Stage 1 G
auto pimin=[](pi a,pi b){
	return min(a,b);
};
template<class E>
struct etlca{
	const vvc<E>&g;
	const int n;
	int io=0,be=0;
	vi in,out,dep,b,e,par,ni;
	vc<pi> di;
	vc<ll> wei;
	void dfs(int v,int p,int d,ll w){
		in[v]=io++;
		dep[v]=d;
		b[v]=be;
		di[be++]=pi(d,v);
		par[v]=p;
		wei[v]=w;
		for(auto to:g[v])if(to!=p){
			dfs(to,v,d+1,w+to.cost);
			di[be++]=pi(d,v);
		}
		out[v]=io;
		e[v]=be;
	}
	sparsetable<pi,decltype(pimin)> st;
	etlca(const vvc<E>&t,int r):g(t),n(si(g)),
		in(n),out(n),dep(n),b(n),e(n),par(n),ni(n),di(2*n-1),
		wei(n),
		st((dfs(r,-1,0,0),di),pimin,pi(inf,-1)){
		rep(i,n)ni[in[i]]=i;
	}
	int lca(int u,int v){
		return st.get(min(b[u],b[v]),max(e[u],e[v])).b;
	}
	int wdist(int u,int v){
		int z=lca(u,v);
		return wei[u]+wei[v]-wei[z]*2;
	}
	//UCUP 2-22-F
	pi diaadd(pi x,int v){
		int a=wdist(x.a,x.b);
		int c=wdist(x.a,v);
		int d=wdist(v,x.b);
		int m=max({a,c,d});
		if(m==c)x.b=v;
		else if(m==d)x.a=v;
		return x;
	}
	//Universal Cup 2023 Stage 1 G
	vi index;
	//vs を含む virtual tree を返す
	//返すのは virtual tree に使われた頂点と，辺の集合
	//辺の端点は，virtual tree における番号
	//元の木における番号を virtual tree の頂点番号に写すのが，index という変数
	//辺は ch->par の順
	//virtual tree は行き掛け順で番号がついている
	//特に，頂点 0 が根になるようにできている
	//pair<vi,vc<pi>> tree_compress(vi vs){
	void tree_compress(vi&vs,vc<pi>&es){
		if(si(index)==0)index.resize(n);
		assert(index.size());
		auto comp = [&](int x,int y){
			return in[x] < in[y];
		};
		//sort(all(vs),comp);
		assert(is_sorted(all(vs),comp));
		vs.erase(unique(all(vs)),vs.ed);
		int k = vs.size();
		rep(i,k-1){
			vs.pb(lca(vs[i],vs[i+1]));
		}
		sort(all(vs),comp);
		vs.erase(unique(all(vs)),vs.ed);
		k = vs.size();
		rep(i,k) index[vs[i]] = i;
		es.clear();
		rng(i,1,k){
			int p = lca(vs[i-1],vs[i]);
			es.eb(i,index[p]);
		}
		//return mp(vs,es);
	}
};
