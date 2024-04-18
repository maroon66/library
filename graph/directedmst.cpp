//union find の unite で a,b の順番保持するのが必要
//yosupo judge
//others->root
template<class D>
struct dirmst{
	struct E{
		int to,id;
		D cost;
		bool operator>(const E&e)const{
			return cost>e.cost;
		}
		E&operator+=(const D&d){
			cost+=d;
			return *this;
		}
		E&operator-=(const D&d){
			cost-=d;
			return *this;
		}
	};
	int n;
	vc<tuple<int,int,D>> es;
	dirmst(int nn):n(nn){}
	void ae(int s,int t,D d){
		es.eb(s,t,d);
	}
	//cost,edge-list
	pair<D,vi> calc(int rt){
		int m=si(es);
		leftistheaplazy<E,greater<E>,D> h(m);
		using np=decltype(h)::np;
		vc<np> g(n+m);
		rep(i,m){
			auto [s,t,d]=es[i];
			h.meld(g[s],h.nn({t,i,d}));
		}
		vi st(n+m);
		st[rt]=2;
		vc<E> best(n+m);
		vi p(n+m,-1);
		unionfind uf(n+m);
		D ans{};
		int tail=n;
		rep(i,n)if(st[i]==0){
			vi vs{i};
			while(1){
				int a=vs.back();
				assert(st[a]==0);
				st[a]=1;
				if(!g[a]){
					assert(false);
					//答えが存在しなければ，{-1,{}} を返すはず（未検証）
					//return mp(-1,vi{});
				}
				E e=h.pop(g[a]);
				best[a]=e;
				ans+=e.cost;
				int to=uf.find(e.to);
				if(st[to]==0){
					vs.pb(to);
				}else if(st[to]==1){
					int r=si(vs),l=r-1;
					while(vs[l]!=to)
						l--;
					int v=tail++;
					rng(j,l,r){
						h.meld(g[v],h.sub(g[vs[j]],best[vs[j]].cost));
						p[vs[j]]=v;
						uf.unite(v,vs[j]);
					}
					vs.resize(l);
					vs.pb(v);
				}else if(st[to]==2){
					break;
				}else{
					assert(false);
				}
			}
			for(auto v:vs)
				st[v]=2;
		}
		
		//union-find の過程の木の根の方から辺を採用していく
		vi ls;
		vc<bool> done(tail);
		done[rt]=true;
		per(v,tail)if(!done[v]){
			ls.pb(best[v].id);
			auto [s,t,d]=es[best[v].id];
			while(s!=-1&&!done[s]){
				done[s]=true;
				s=p[s];
			}
		}
		return make_pair(ans,ls);
	}
};
