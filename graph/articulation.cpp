//連結成分と頂点を一緒くたにした場合の木が tr で得られる
//連結成分たくさんとか多重辺ありとかでも動くはずだが，未確認
//連結成分 x の頂点リスト -> tr[n+x]
//孤立点は連結成分として処理されない
//s に biconnected components の個数が入る
//es にその biconnected componenents の辺のリストが入る
//辺の端点はもともとのグラフの頂点番号を使う
//VERIFY
//Petrozavodsk Camp 2020S Day2 C
//TCO2020 Wildcard Hard
//CF CodeTON Round I
template<class E>
struct articulation{
	const int n;
	const vvc<E>&g;
	vi vis,ord,low;
	int head,s;
	vvc<int> tr;
	vvc<pi> es;
	void ae(int a,int b){
		if(a==-1||b==-1)return;
		tr[a].pb(b);
		tr[b].pb(a);
	}
	void dfs(int v){
		assert(vis[v]==0);
		vis[v]=1;
		ord[v]=low[v]=head++;
		for(auto to:g[v]){
			if(vis[to]==0){
				dfs(to);
				chmin(low[v],low[to]);
			}else{
				chmin(low[v],ord[to]);
			}
		}
	}
	void dfs2(int v,int z){
		assert(vis[v]==1);
		vis[v]=2;
		ae(v,z);
		for(auto to:g[v])if(vis[to]==1){
			if(low[to]<ord[v]){
				dfs2(to,z);
			}else{
				int w=si(tr);
				tr.eb();
				es.eb();
				s++;
				ae(v,w);
				dfs2(to,w);
			}
		}else if(vis[to]==2){
			es[z-n].eb(v,to);
		}
		vis[v]=3;
	}
	articulation(const vvc<E>&gg):n(si(gg)),g(gg),
		vis(n,0),ord(n,-1),low(n,-1),head(0),s(0),tr(n){
		rep(i,n)if(vis[i]==0){
			dfs(i);
			dfs2(i,-1);
		}
	}
	//連結成分 i の辺情報を頂点番号を座標圧縮した上で返す
	pair<vi,vc<pi>> getg(int i){
		vi vs=tr[n+i];
		sort(all(vs));
		vc<pi> res=es[i];
		for(auto&[a,b]:res){
			a=lwb(vs,a);
			b=lwb(vs,b);
		}
		return mp(vs,res);
	}
};
