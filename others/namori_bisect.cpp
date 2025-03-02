//i->par[i] と遷移すると raw[i] かかるとする
//i->...->j までの raw を全部 composite したものを考える
//これが ok な最大の j というものを取ってくる
//そこまでの composite も取ってくる
//サイクルにも対応!
//高々 upto 回までの遷移を行う
//stress-tested! (others に入れた)
template<class N>
struct namori_bisect{
	vi par;
	const int n;
	vc<N> acc;
	vi sub,head,pos,sop;
	segtree<N> seg;
	template<class t>
	vc<N> init(const vc<t>&raw){
		vvc<int> g(n);
		rep(i,n)if(par[i]!=-1)g[par[i]].pb(i);
		vi deg(n);
		rep(i,n)deg[i]=si(g[i]);
		vi q;
		rep(i,n)if(deg[i]==0)q.pb(i);
		while(si(q)){
			int v=gpp(q);
			if(par[v]!=-1){
				sub[par[v]]+=sub[v];
				if(--deg[par[v]]==0)q.pb(par[v]);
			}
		}
		int ord=0;
		vc<N> res(n);
		auto dfs=[&](auto self,int v,int h)->void{
			head[v]=h;
			if(v==h) acc[v]=raw[v];
			else acc[v]=N::merge(raw[v],acc[par[v]]);
			if(si(g[v])){
				rng(i,1,si(g[v]))
					if(sub[g[v][0]]<sub[g[v][i]])
						swap(g[v][0],g[v][i]);
				rng(i,1,si(g[v]))
					self(self,g[v][i],g[v][i]);
				self(self,g[v][0],h);
			}
			pos[v]=ord++;
			res[pos[v]]=raw[v];
			sop[pos[v]]=v;
		};
		auto work=[&](int root){
			dfs(dfs,root,root);
		};
		rep(i,n)if(par[i]==-1){
			for(auto r:g[i]){
				work(r);
			}
		}
		rep(i,n)if(deg[i]>0){
			int x=i;
			while(1){
				deg[x]=0;
				int y=par[x];
				if(y!=i){
					sub[y]+=sub[x];
					x=y;
				}else break;
			}
			remval(g[i],x);
			work(x);
			par[x]=n+i;
		}
		res.resize(ord);
		return res;
	}
	template<class t>
	namori_bisect(const vi&pp,const vc<t>&raw):par(pp),n(si(par)),
	acc(n),sub(n,1),head(n,-1),pos(n,-1),sop(n,-1),
	seg(init(raw)){}
	//v から start してどの長さまで遷移できるか
	//steps,last_vertex,data
	template <class F,class... Args> 
	tuple<int,int,N> max_len_withinit(int v,int upto,N sm,F f,Args&&... args){
		assert(inc(0,v,n-1));
		assert((sm.*f)(forward<Args>(args)...));
		int step=0;
		auto adv=[&](int u){
			assert(inc(0,u,upto));
			assert(inc(0,v,n-1));
			assert(inc(0,u,pos[head[v]]-pos[v]));
			v=sop[pos[v]+u];
			step+=u;
			upto-=u;
		};
		while(v<n&&par[v]!=-1&&upto>0){
			int h=head[v];
			int dist=pos[h]-pos[v]+1;
			if(upto<dist){//definitely ends here
				auto [u,z]=seg.max_right_withinit(pos[v],sm,f,forward<Args>(args)...);
				if(pos[v]+upto<u){
					sm=N::merge(sm,seg.composite(pos[v],pos[v]+upto));
					adv(upto);
				}else{
					sm=z;
					adv(u-pos[v]);
				}
				break;
			}else{
				N tmp=N::merge(sm,acc[v]);
				if((tmp.*f)(forward<Args>(args)...)){
					adv(dist-1);
					v=par[v];
					step++;
					upto--;
					sm=tmp;
				}else{
					auto [u,z]=seg.max_right_withinit(pos[v],sm,f,forward<Args>(args)...);
					sm=z;
					adv(u-pos[v]);
					break;
				}
			}
		}
		assert(v!=-1);
		if(upto==0||v<n){return mt(step,v%n,sm);}
		v-=n;
		int dist=0;
		N sc;
		{
			for(int x=v;x!=v+n;){
				int h=head[x];
				dist+=pos[h]-pos[x]+1;
				sc=N::merge(sc,acc[x]);
				x=par[h];
			}
		}
		static vc<N> cyc; cyc.clear();
		cyc.pb(sc);
		rep(i,100){
			if((N::merge(sm,cyc[i]).*f)(forward<Args>(args)...)&&(dist<<i)<=upto){
				cyc.pb(N::merge(cyc.back(),cyc.back()));
			}else break;
		}
		per(i,si(cyc)){
			N tmp=N::merge(sm,cyc[i]);
			if((tmp.*f)(forward<Args>(args)...)&&(dist<<i)<=upto){
				sm=tmp;
				step+=dist<<i;
				upto-=dist<<i;
			}
		}
		auto [rstep,rv,rsm]=max_len_withinit(v,upto,sm,f,forward<Args>(args)...);
		rstep+=step;
		return mt(rstep,rv,rsm);
	}
	template <class F,class... Args> 
	tuple<int,int,N> max_len(int v,F f,Args&&... args){
		return max_len_withinit(v,inf,N(),f,forward<Args>(args)...);
	}
};
