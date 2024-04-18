//init() グラフを初期化
//grev() init の前に呼ばれ，グラフを逆辺にする
//getto(v) v から遷移できる先をなにか一つ返す(なければ -1)
//del(v) v をグラフから削除
//UCUP 2023-3 F
//UCUP 1-20-C
//多分 toposort 順にグループわけされている，はず
template<class F,class F2,class G,class H>
vvc<int> scc_dynamic(int n,F init,F2 grev,G getto,H del){
	init();
	vi idx;
	vc<bool> vis(n);
	auto dfs=[&](auto self,int v)->void{
		assert(!vis[v]);
		vis[v]=true;
		del(v);
		while(1){
			int to=getto(v);
			if(to==-1)break;
			self(self,to);
		}
		idx.pb(v);
	};
	rep(i,n)if(!vis[i]){
		dfs(dfs,i);
	}
	
	grev();
	init();
	fill(all(vis),false);
	vi tmp;tmp.swap(idx);
	reverse(all(tmp));
	vvc<int> res;
	for(auto i:tmp)if(!vis[i]){
		idx.clear();
		dfs(dfs,i);
		res.pb(idx);
	}
	
	grev();
	return res;
}

//UCUP 2023-3 F
template<class F,class G,class H>
vc<bool> reacheble(int n,int root,F init,G getto,H del){
	init();
	vc<bool> vis(n);
	auto dfs=[&](auto self,int v)->void{
		assert(!vis[v]);
		vis[v]=true;
		del(v);
		while(1){
			int to=getto(v);
			if(to==-1)break;
			self(self,to);
		}
	};
	dfs(dfs,root);
	return vis;
}
