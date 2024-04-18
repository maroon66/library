//work without define int ll
//maxflow with small constant
//CF Global11 G
//bfs を途中で打ち切る，dfs が逆順
//ae の 4th argument は逆辺の capacity
//無向辺を貼りたい場合などに，有効・・・
template<class d>
struct maxflow{
	struct E{
		int to,nx;
		d cap;
	};
	vc<E> es;
	vi head,q;
	vc<pi> itr,lv;
	int curt;
	maxflow(int n):head(n,-1),q(n),itr(n),lv(n),curt(0){}
	int ae(int s,int t,d c,d c2=0){
		int pres=head[s];
		int pret=head[t];
		head[s]=si(es);
		es.pb({t,pres,c});
		head[t]=si(es);
		es.pb({s,pret,c2});
		return head[s];
	}
	void bfs(int s,int t=-1){
		lv[s]=pi(0,curt);
		int l=0,r=0;
		q[r++]=s;
		while(l<r){
			int v=q[l++];
			for(int id=head[v];id!=-1;id=es[id].nx){
				auto e=es[id];
				if(e.cap>0&&lv[e.to].b<curt){
					lv[e.to]=pi(lv[v].a+1,curt);
					q[r++]=e.to;
					if(e.to==t)return;
				}
			}
		}
	}
	d dfs(int v,int t,d f){
		if(v==t)return f;
		if(itr[v].b<curt)itr[v]=pi(head[v],curt);
		d res=0;
		while(itr[v].a!=-1){
			E& e=es[itr[v].a];
			E& rev=es[itr[v].a^1];
			if(rev.cap>0&&lv[e.to].a==lv[v].a-1&&lv[e.to].b==curt){
				d a=dfs(e.to,t,min(f,rev.cap));
				if(a>0){
					rev.cap-=a;
					e.cap+=a;
					res+=a;
					f-=a;
					if(f<=0)break;
				}
			}
			itr[v].a=e.nx;
		}
		return res;
	}
	d calc(int s,int t,d mx=inf){
		d f=0;
		while(f<mx){
			curt++;
			bfs(s,t);
			if(lv[t].b<curt)
				return f;
			f+=dfs(t,s,mx-f);
		}
		return f;
	}
};

//以下のパラメータを問題に応じていじって最強の定数倍を手に入れよう！
//- eps を割る scale
//- refine の必要性を確かめる嘘 bellman-ford のループ回数
//- dfs を途中で打ち切る深さ
//VERIFY:
//yosupo mincost b flow
//AGC034D
//work without define int ll
//captype=O(cap*max(n,m))
//costtype=O(cost*n*max(n,m))
//あらゆる関数が破壊的
//よくわからないが cap=inf だとTLEする(overflow?)ことがあるので，10^9 とかでお茶を濁す
template<class captype,class costtype,class anstype>
struct costscaling{
	struct E{
		int to,rev;
		captype cap;
		costtype cost;
	};
	int n;
	costtype eps;
	vvc<E> g;
	vc<pi> idx;
	vi itr,q;
	vc<bool> inq,vis;
	vc<captype> fl,off;
	vc<costtype> p;
	maxflow<captype> mf;
	anstype ans;
	costscaling(int nn):n(nn),g(n),itr(n),inq(n),vis(n),fl(n),p(n),mf(n+2),ans(0){}
	void ae(int s,int t,captype l,captype r,costtype c){
		assert(l<=r);
		
		if(s==t){
			idx.eb(-1,-1);
			captype f=c>0?l:r;
			ans+=anstype(c)*f;
			off.pb(f);
		}else{
			c*=(n+1);
			idx.eb(s,si(g[s]));
			off.pb(r);
			fl[s]-=l;
			fl[t]+=l;
			g[s].pb({t,si(g[t]),r-l,c});
			g[t].pb({s,si(g[s])-1,0,-c});
		}
		
		mf.ae(s,t,r-l);
	}
	void add_excess(int v,captype c){
		fl[v]+=c;
	}
	void add_demand(int v,captype c){
		fl[v]-=c;
	}
	costtype getcost(int v,const E&e){
		return e.cost-p[e.to]+p[v];
	}
	void addq(int v){
		if(fl[v]>0&&!inq[v]){
			q.pb(v);
			inq[v]=true;
		}
	}
	void push(int v,E&e,captype a){
		int to=e.to,j=e.rev;
		assert(0<=a);
		assert(a<=e.cap);
		e.cap-=a;
		g[to][j].cap+=a;
		fl[v]-=a;
		fl[to]+=a;
		addq(to);
	}
	void relabel(int v){
		p[v]=numeric_limits<costtype>::min()/2;
		for(const auto&e:g[v]){
			if(e.cap>0){
				chmax(p[v],-eps-e.cost+p[e.to]);
			}
		}
		itr[v]=0;
	}
	bool dfs(int v,int dep,captype come){
		if(vis[v])return false;
		vis[v]=true;
		if(fl[v]<0||dep==4){
			vis[v]=false;
			return true;
		}else{
			while(itr[v]<si(g[v])){
				auto&e=g[v][itr[v]];
				if(e.cap&&getcost(v,e)<0){
					auto u=min(e.cap,fl[v]+come);
					if(dfs(e.to,dep+1,u)){
						push(v,e,u);
						vis[v]=false;
						return true;
					}
				}
				itr[v]++;
			}
			relabel(v);
			vis[v]=false;
			return false;
		}
	}
	bool slv(){
		int m=si(idx);
		{
			if(accumulate(all(fl),captype(0))){
				//demand and excess don't match
				return false;
			}
			captype f=0;
			rep(i,n)if(fl[i]>0){
				f+=fl[i];
				mf.ae(n,i,fl[i]);
			}else if(fl[i]<0){
				mf.ae(i,n+1,-fl[i]);
			}
			captype did=mf.calc(n,n+1,f);
			if(did<f)return false;
			rep(k,m)if(idx[k].a!=-1){
				auto&e=g[idx[k].a][idx[k].b];
				e.cap=mf.es[k*2].cap;
				g[e.to][e.rev].cap=mf.es[k*2+1].cap;
			}
			rep(i,n)fl[i]=0;
		}
		while(1){
			//now eps-optimal
			rep(i,n)assert(fl[i]==0);
			eps=0;
			rep(v,n)for(const auto&e:g[v])if(e.cap>0)chmax(eps,-getcost(v,e));
			if(eps<=1)break;
			
			eps=max(eps/8,costtype(1));
			
			bool need=true;
			rep(_,5){
				bool upd=false;
				rep(v,n)for(const auto&e:g[v])if(e.cap>0){
					if(chmin(p[e.to],p[v]+e.cost+eps))
						upd=true;
				}
				if(!upd){
					need=false;
					break;
				}
			}
			if(!need)continue;
			
			rep(v,n)for(auto&e:g[v])if(getcost(v,e)<0)
				push(v,e,e.cap);
			
			q.clear();
			rep(i,n){
				itr[i]=0;
				inq[i]=false;
			}
			int qb=0;
			rep(i,n)addq(i);
			while(qb<si(q)){
				int v=q[qb++];
				inq[v]=false;
				while(fl[v]>0)dfs(v,0,0);
				/*while(fl[v]>0){
					if(itr[v]==si(g[v]))
						relabel(v);
					while(itr[v]<si(g[v])){
						auto&e=g[v][itr[v]];
						if(e.cap&&getcost(v,e)<0){
							push(v,e,min(e.cap,fl[v]));
							if(fl[v]==0)break;
						}
						itr[v]++;
					}
				}*/
			}
		}
		rep(i,n)assert(fl[i]==0);
		rep(v,n)for(const auto&e:g[v])if(e.cap>0)assert(getcost(v,e)>=-1);
		
		rep(v,n)for(auto&e:g[v])e.cost/=(n+1);
		return true;
	}
	pair<anstype,vc<captype>> get_solution(){
		rep(i,si(off))if(idx[i].a!=-1){
			const auto&e=g[idx[i].a][idx[i].b];
			off[i]-=e.cap;
			ans+=anstype(e.cost)*off[i];
		}
		return mp(ans,off);
	}
	vc<costtype> get_potential(){
		rep(i,n)p[i]/=(n+1);
		rep(_,n+1){
			bool upd=false;
			rep(v,n)for(const auto&e:g[v])if(e.cap>0){
				if(chmin(p[e.to],p[v]+e.cost))
					upd=true;
			}
			if(!upd)break;
			assert(_<n);
		}
		return p;
	}
};
