//max weight matching for general graphs O(N^3)
//init with an adjacency matrix g: use none for not-connected pairs.
//ans = max weight matching of size 0,1,...,M. (M = max cardinality).
//sol = solutions
//verify yosupo, stress-tested(without int=ll)
static const ll none=-infLL/3;
struct maxweight{
	static pi rev(pi x){return pi(x.b,x.a);}
	const vvc<ll>&g;
	const int n;
	vi idx,mt,bl,bs,tp;
	vc<ll> u,z;
	vc<pi> par,sf,ss;
	vvc<pi> me,ce;
	vvc<int> cb;
	ll p(pi e){
		if(e.a==e.b)return infLL;
		return u[e.a]+u[e.b]-g[e.a][e.b]*2;
	}
	void chme(pi&x,pi y){
		if(p(x)>p(y))x=y;
	}
	void match(pi e){
		int a=e.a,b=e.b;
		mt[a]=b;
		mt[b]=a;
		bs[bl[a]]=a;
		bs[bl[b]]=b;
	}
	void setb(int v,int b){
		if(v<n) bl[v]=b;
		else for(auto c:cb[v]) setb(c,b);
	}
	void make(const vc<pi>&es){
		int v=idx.back();
		idx.pop_back();
		ce[v]=es;
		cb[v].clear();
		for(auto e:es)
			cb[v].pb(bl[e.a]);
		rep(i,n)
			me[v][i]=pi(-1,-1);
		for(auto c:cb[v]){
			tp[c]=-2;
			setb(c,v);
			rep(i,n)
				chme(me[v][i],me[c][i]);
		}
		bs[v]=bs[cb[v][0]];
		z[v]=0;
		labelS(v,par[cb[v][0]]);
	}
	vc<pi> path(int v){
		vc<pi> es;
		while(par[bl[v]].a!=-1){
			es.pb(par[bl[v]]);
			v=par[bl[v]].a;
		}
		return es;
	}
	bool SS(pi e){
		vc<pi> x=path(e.a),y=path(e.b);
		while(si(x)&&si(y)&&x.back()==y.back()){
			x.pop_back();
			y.pop_back();
		}
		vc<pi> es=x;
		reverse(all(es));
		es.pb(e);
		for(auto f:y)es.pb(rev(f));
		if(bl[es.front().a]==bl[es.back().b]){
			make(es);
			return false;
		}else{
			for(int i=0;i<si(es);i+=2){
				match(es[i]);
				for(auto j:{bl[es[i].a],bl[es[i].b]}){
					expand(j,true);
					setb(j,j);
				}
			}
			return true;
		}
	}
	void expand(int v,bool r){
		if(v<n)return;
		for(auto c:cb[v]) setb(c,c);
		rep(i,si(cb[v]))
			if(bl[bs[v]]==cb[v][i]){
				rotate(cb[v].bg,cb[v].bg+i,cb[v].ed);
				rotate(ce[v].bg,ce[v].bg+i,ce[v].ed);
				break;
			}
		bs[bl[bs[v]]]=bs[v];
		for(int i=1;i<si(ce[v]);i+=2)
			match(ce[v][i]);
		if(r){
			for(auto c:cb[v]) expand(c,true);
		}else{
			int w=bl[par[v].b];
			labelT(w,par[v]);
			rep(i,si(cb[v]))if(cb[v][i]==w){
				if(i%2==0){
					rep(j,i)
						if(j%2==0)
							labelT(bl[ce[v][j].a],rev(ce[v][j]));
						else
							labelS(bl[ce[v][j].a],rev(ce[v][j]));
					rng(j,i+1,si(cb[v]))
						tp[cb[v][j]]=-1;
				}else{
					rng(j,i,si(ce[v]))
						if(j%2==0)
							labelT(bl[ce[v][j].b],ce[v][j]);
						else
							labelS(bl[ce[v][j].b],ce[v][j]);
					rng(j,1,i)
						tp[cb[v][j]]=-1;
				}
			}
			tp[v]=-2;
			idx.pb(v);
		}
	}
	void labelS(int v,pi p){
		tp[v]=0;
		par[v]=p;
		ss[v]=pi(-1,-1);
		rep(i,n)if(bl[i]!=v){
			if(tp[bl[i]]==0){
				chme(ss[v],me[v][i]);
				chme(ss[bl[i]],rev(me[v][i]));
			}else{
				chme(sf[i],me[v][i]);
			}
		}
	}
	void labelT(int v,const pi&p){
		tp[v]=1;
		par[v]=p;
	}
	vc<ll> ans;
	vvc<int> sol;
	maxweight(const vvc<ll>&gg):g(gg),n(si(g)),
		idx(n),mt(n,-1),bl(n),bs(n*2),tp(n*2,-1),u(n),z(n*2,0),
		par(n*2),sf(n),ss(n*2),me(n*2,vc<pi>(n)),
		ce(n*2),cb(n*2){
		iota(all(idx),n);
		iota(all(bl),0);
		iota(all(bs),0);
		rng(i,n,n*2)tp[i]=-2;
		rep(i,n)rep(j,n)me[i][j]=pi(i,j);
		{
			ll mx=-infLL;
			rep(i,n)rep(j,i)chmax(mx,g[i][j]);
			fill(all(u),mx);
		}
		ans.pb(0);
		sol.pb(vi(n,-1));
		rep(_,n/2){
			rep(i,n*2)if(tp[i]!=-2) tp[i]=-1;
			fill(all(sf),pi(-1,-1));
			rep(i,n)if(mt[i]==-1)
				labelS(bl[i],pi(-1,-1));
			while(true){
				ll d=infLL;
				pi e;
				rep(i,n){
					int b=bl[i];
					if(tp[b]==-1){
						if(chmin(d,p(sf[i])))e=sf[i];
					}else if(tp[b]==0){
						if(chmin(d,p(ss[b])/2))e=ss[b];
					}else if(n<=b){
						if(chmin(d,z[b]/2))e=pi(b,-1);
					}
				}
				rep(i,n){
					int t=tp[bl[i]];
					if(t==0) u[i]-=d;
					else if(t==1) u[i]+=d;
				}
				rng(i,n,n*2)
					if(tp[i]==0)
						z[i]+=d*2;
					else if(tp[i]==1)
						z[i]-=d*2;
				if(e.b==-1){
					expand(e.a,false);
				}else if(tp[bl[e.b]]==0){
					if(SS(e)) break;
				}else{
					int t=bl[e.b],m=mt[bs[t]];
					labelT(t,e);
					labelS(bl[m],pi(bs[t],m));
				}
			}
			bool bad=false;
			ll sum=0;
			rep(i,n)if(i<mt[i]){
				if(g[i][mt[i]]==none){
					bad=true;
					break;
				}
				sum+=g[i][mt[i]];
			}
			if(bad)break;
			ans.pb(sum);
			sol.pb(mt);
		}
	}
};
