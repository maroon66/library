//n=m だけ verify
//CF557F
//lf[i][j] -> 頂点 i から見て j 番目に大切な頂点
pair<vi,vi> stable_matching(vvc<int> lf,vvc<int>rt){
	int n=si(lf),m=si(rt);
	rep(i,n){assert(si(lf[i])==m);}
	rep(i,m){
		assert(si(rt[i])==n);
		vi z(n);
		rep(j,n)z[rt[i][j]]=j;
		rt[i]=z;
	}
	vi head(n,0),mt(n,-1),tm(m,-1);
	vi idx(n);iota(all(idx),0);
	while(si(idx)){
		int i=idx.back();idx.pop_back();
		assert(mt[i]==-1);
		while(head[i]<m){
			int j=lf[i][head[i]++];
			if(tm[j]==-1){
				mt[i]=j;
				tm[j]=i;
				break;
			}else if(rt[j][i]<rt[j][tm[j]]){
				mt[tm[j]]=-1;
				idx.pb(tm[j]);
				mt[i]=j;
				tm[j]=i;
				break;
			}
		}
	}
	return mp(mt,tm);
}
