vi getcoloring(const vvc<int>&g){
	int n=si(g);
	vi deg(n);
	priority_queue<pi,vc<pi>,greater<pi>> pq;
	rep(i,n){
		deg[i]=si(g[i]);
		pq.emplace(deg[i],i);
	}
	vc<bool> vis(n);
	vi idx;
	while(si(pq)){
		int i=pq.top().b;pq.pop();
		if(vis[i])continue;
		vis[i]=true;
		idx.pb(i);
		for(auto j:g[i])if(!vis[j])pq.emplace(--deg[j],j);
	}
	reverse(all(idx));
	vi ans(n,-1);
	int curtime=0;
	vi used(n);
	for(auto i:idx){
		curtime++;
		for(auto j:g[i])if(ans[j]!=-1)used[ans[j]]=curtime;
		rep(k,n)if(used[k]<curtime){
			ans[i]=k;
			break;
		}
	}
	return ans;
}
