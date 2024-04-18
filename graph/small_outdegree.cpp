//UCUP 2-22-C
	{
		vvc<E> h(n);
		vi deg(n);
		rep(i,n)deg[i]=si(g[i]);
		vc<bool> done(n,false);
		priority_queue<pi,vc<pi>,greater<pi>> pq;
		rep(i,n)pq.emplace(deg[i],i);
		while(si(pq)){
			auto [d,i]=pq.top();pq.pop();
			if(done[i])continue;
			done[i]=true;
			for(auto e:g[i])if(!done[e]){
				h[i].pb(e);
				pq.emplace(--deg[e],e);
			}
		}
		g.swap(h);
	}
