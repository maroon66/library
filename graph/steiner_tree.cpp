//VERIFYされていないが，俺はいけると思う
//復元なんて必要ねえんだよ
int steiner_tree_cost(int n,vc<tuple<int,int,int>> es,vi ts){
	mkuni(ts);
	assert(si(ts));
	vvc<pi> g(n);
	for(auto e:es){
		int a,b,c;tie(a,b,c)=e;
		g[a].eb(b,c);
		g[b].eb(a,c);
	}
	int k=si(ts)-1;
	vvc<int> dp(1<<k,vi(n,inf));
	rep(bit,1<<k){
		if(bit==0){
			dp[bit]=vi(n,0);
		}else{
			if(popcount(bit)==1){
				dp[bit][ts[topbit(bit)]]=0;
			}else{
				for(int sub=(bit-1)&bit;sub>(bit-sub);sub=(sub-1)&bit){
					rep(i,n)chmin(dp[bit][i],dp[sub][i]+dp[bit-sub][i]);
				}
			}
			priority_queue<pi,vc<pi>,greater<pi>> pq;
			auto reach=[&](int v,int d){
				if(dp[bit][v]>d){
					dp[bit][v]=d;
					pq.emplace(d,v);
				}
			};
			rep(i,n)if(dp[bit][i]<inf)pq.emplace(dp[bit][i],i);
			while(si(pq)){
				int d,v;tie(d,v)=pq.top();pq.pop();
				if(dp[bit][v]!=d)continue;
				for(auto e:g[v]){
					reach(e.a,d+e.b);
				}
			}
		}
	}
	return dp[mask(k)][ts[k]];
}
