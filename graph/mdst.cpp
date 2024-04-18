//SPOJ MDST
//XX Opencup GP of Korea
template<class E,class D=ll>
pair<D,vc<pi>> mdst(const vvc<E>&g){
	int n=g.size();
	if(n==1)return make_pair(0,vc<pi>());
	
	//O(NMlogM)
	vvc<D> d(n);
	rep(i,n)d[i]=dijkstra<E,D>(g,i);
	
	//O(N^3)
	vvc<D> d(n,vc<D>(n,infLL));
	rep(i,n)d[i][i]=0;
	rep(i,n)for(auto e:g[i])
		d[i][e.to]=e.cost;
	rep(k,n)rep(i,n)rep(j,n)
		chmin(d[i][j],d[i][k]+d[k][j]);
	
	using A=tuple<D,int,int,D>;
	A ans(inf,-1,-1,0);
	rep(i,n){
		vi idx(n);
		iota(all(idx),0);
		sort(all(idx),[&](int a,int b){
			return d[i][a]>d[i][b];
		});
		for(auto e:g[i]){
			int j=e.to;
			if(i<j){
				vc<pair<D,D>> w;
				for(auto k:idx)w.eb(d[i][k],d[j][k]);
				chmin(ans,A(w[0].a*2,i,j,0));
				int a=0;
				rng(k,1,n)if(w[a].b<w[k].b){
					chmin(ans,A(e.cost+w[a].b+w[k].a,i,j,(w[a].b-w[k].a+e.cost)));
					a=k;
				}
				chmin(ans,A(w[a].b*2,i,j,e.cost*2));
			}
		}
	}
	
	int val,ei,ej,t;
	tie(val,ei,ej,t)=ans;
	
	vc<pi> es;
	es.eb(ei,ej);
	
	vc<D> dist(n);
	rep(i,n)dist[i]=min((d[ei][i])*2+t,(d[ej][i]+d[ei][ej])*2-t);
	
	rep(i,n)if(ei!=i&&ej!=i){
		for(auto e:g[i])
			if(dist[e.to]+e.cost*2==dist[i]){
				es.eb(e.to,i);
				break;
			}
	}
	
	return make_pair(val,es);
}
