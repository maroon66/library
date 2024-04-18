//有向グラフを受け取る．
//単純サイクルと単純パスの和に分解する
//パスの個数は最小化されている．
//パスの順番なんだけど，入次数 0 の頂点から出次数 0 のパスをどんどん取り出す
//という順番で列挙されているイメージ
//構築のときに便利だったりするかも
//辺の id の列が返ってくる．
//多分 O(N+M)
//KUPC2020 K
pair<vvc<int>,vvc<int>> cycle_path_decomp(const vvc<pi>&g){
	int n=si(g);
	vi in(n),out(n);
	vvc<pi> h(n);
	vvc<int> cs,ps;
	{//cycle part
		vi vis(n),head(n);
		vi cyc;
		auto dfs=[&](auto self,int v)->int{
			assert(vis[v]==0);
			vis[v]=1;
			while(head[v]<si(g[v])){
				auto e=g[v][head[v]++];
				if(vis[e.a]==0){
					int f=self(self,e.a);
					if(f!=-1){
						cyc.pb(e.b);
						if(f==v){
							reverse(all(cyc));
							cs.pb(cyc);
							cyc.clear();
						}else{
							vis[v]=0;
							return f;
						}
					}else{
						h[v].pb(e);
					}
				}else if(vis[e.a]==1){
					cyc.pb(e.b);
					vis[v]=0;
					return e.a;
				}else{
					h[v].pb(e);
				}
			}
			vis[v]=2;
			return -1;
		};
		rep(i,n)if(vis[i]==0)dfs(dfs,i);
	}
	rep(i,n)for(auto e:h[i]){
		out[i]++;
		in[e.a]++;
	}
	{//path part
		vi idx;
		rep(i,n)if(in[i]==0)idx.pb(i);
		vi head(n);
		while(si(idx)){
			int i=idx.back();idx.pop_back();
			while(out[i]){
				vi p;
				int cur=i;
				while(out[cur]){
					auto e=h[cur][head[cur]++];
					out[cur]--;
					in[e.a]--;
					if(in[e.a]==0)idx.pb(e.a);
					p.pb(e.b);
					cur=e.a;
				}
				ps.pb(p);
			}
		}
		assert(out==vi(n,0));
		assert(in==vi(n,0));
	}
	return mp(cs,ps);
}
