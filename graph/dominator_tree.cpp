//CCPC Finals 2021 H
//Multiuni2023-3 E
struct dominator_tree{
	int n;
	vvc<int> g,rG,bct;
	vi idom,semi,us,id,rId,par,mn,anc;
	dominator_tree(int nn):n(nn),
		g(n),rG(n),bct(n),
		idom(n,-1),semi(n),us(n),id(n,-1),rId(n),par(n,-1),mn(n),anc(n,-1){
		iota(all(semi),0);
		iota(all(mn),0);
	}
	void ae(int a,int b){
		if(a==b)return;
		g[a].pb(b);
		rG[b].pb(a);
	}
	int find(int v){
		if(anc[v]==-1)
			return v;
		int a=find(anc[v]);
		if(id[semi[mn[anc[v]]]]<id[semi[mn[v]]])
			mn[v]=mn[anc[v]];
		return anc[v]=a;
	}
	void link(int c,int p){
		anc[c]=p;
	}
	void dfs(int v,int p,int& i){
		if(id[v]!=-1)
			return;
		id[v]=i;
		rId[i++]=v;
		par[v]=p;
		for(int c:g[v])
			dfs(c,v,i);
	}
	//return idom or -1 if unreachable from the root
	vi calc(int root){
		int sz=0;
		dfs(root,-1,sz);
		for(int i=sz-1;i>0;i--){
			int w=rId[i];
			for(int v:rG[w])if(id[v]!=-1){
				find(v);
				if(id[semi[mn[v]]]<id[semi[w]])
					semi[w]=semi[mn[v]];
			}
			bct[semi[w]].pb(w);
			for(int v:bct[par[w]]){
				find(v);
				us[v]=mn[v];
			}
			bct[par[w]].clear();
			link(w,par[w]);
		}
		rng(i,1,sz){
			int w=rId[i];
			if(semi[w]==semi[us[w]])
				idom[w]=semi[w];
			else
				idom[w]=idom[us[w]];
		}
		return idom;
	}
};
