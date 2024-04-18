//二重辺連結成分に分解する
//s: 連結成分数.
//bl[i]: 頂点 i の属する成分の番号
//pos[i]: 頂点 i の属する成分において i が何番目か
//idx[i]: 連結成分 i の頂点番号
//t: 連結成分からなる木(森かも)
//cmp[i]: 連結成分 i だけを圧縮したグラフ
//MHC2023 Practice D
template<class E>
struct bridge{
	const int n;
	const vvc<E>&g;
	vi ord,low,bl,pos,st;
	int s,head;
	vvc<int> idx,t;
	vc<vvc<int>> cmp;
	void dfs(int v,int p){
		ord[v]=low[v]=head++;
		st.pb(v);
		for(auto to:g[v]){
			if(ord[to]==-1){
				dfs(to,v);
				chmin(low[v],low[to]);
			}else if(to==p){
				p=-1;
			}else{
				chmin(low[v],ord[to]);
			}
		}
		if(ord[v]==low[v]){
			int c=s++;
			while(1){
				int a=st.back();
				st.pop_back();
				bl[a]=c;
				if(v==a)break;
			}
		}
	}
	bridge(const vvc<E>&gg):n(gg.size()),g(gg),
		ord(n,-1),low(n,-1),bl(n,-1),pos(n,-1),
		s(0),head(0){
		rep(i,n)if(ord[i]==-1)
			dfs(i,-1);
		vi ss(s);
		rep(i,n)pos[i]=ss[bl[i]]++;
		//construct the graph
		idx.resize(n);
		rep(i,n)idx[bl[i]].pb(i);
		cmp.resize(s);
		rep(i,s)cmp[i].resize(ss[i]);
		t.resize(s);
		rep(i,n)for(auto j:g[i])if(bl[i]!=bl[j])
			t[bl[i]].pb(bl[j]);
		else
			cmp[bl[i]][pos[i]].pb(pos[j]);
	}
};
