template<class d>
struct maxflow{
	struct E{
		int to,rev;
		d cap;
	};
	vvc<E> g;
	vi itr,lv,q;
	maxflow(int n):g(n),itr(n),lv(n),q(n){}
	pi ae(int s,int t,d c){
		g[s].pb({t,(int)g[t].size(),c});
		g[t].pb({s,(int)g[s].size()-1,0});
		return pi(s,si(g[s])-1);
	}
	//undirected edge
	void aeU(int s,int t,d c){
		g[s].pb({t,si(g[t]),c});
		g[t].pb({s,si(g[s])-1,c});
	}
	void bfs(int s,int t){
		fill(all(lv),-1);
		lv[s]=0;
		int l=0,r=0;
		q[r++]=s;
		while(l<r){
			int v=q[l++];
			if(v==t)break;//speed up!
			for(auto e:g[v])if(e.cap>0&&lv[e.to]==-1){
				lv[e.to]=lv[v]+1;
				q[r++]=e.to;
			}
		}
	}
	d dfs(int v,int t,d f){
		if(v==t)return f;
		d res=0;
		for(int&i=itr[v];i<si(g[v]);i++){
			E& e=g[v][i];
			if(e.cap>0&&lv[e.to]==lv[v]+1){
				d a=dfs(e.to,t,min(f,e.cap));
				if(a>0){
					e.cap-=a;
					g[e.to][e.rev].cap+=a;
					res+=a;
					f-=a;
					if(f<=0)break;
				}
			}
		}
		return res;
	}
	d calc(int s,int t,d mx=inf){
		d f=0;
		while(f<mx){
			bfs(s,t);
			if(lv[t]==-1)
				return f;
			fill(all(itr),0);
			f+=dfs(s,t,mx-f);
		}
		return f;
	}
	//decompose_flows
	//UCUP2-13-A
	
	//gomory-hu tree ではありません！
	//flow equivalent tree ではある．
	//無向グラフなので aeU を呼ぶ
	//https://github.com/spaghetti-source/Competitive-Programming/blob/master/SPOJ3900.cc
	//https://codeforces.com/gym/103652/problem/D
	using T=tuple<d,int,int>;
	vc<T> gomory_hu(){
		int n=si(g);
		vc<T> res;
		auto org=g;
		vi p(n);
		rng(i,1,n){
			rep(v,n)rep(e,si(g[v]))g[v][e].cap=org[v][e].cap;
			res.eb(calc(i,p[i]),i,p[i]);
			rng(j,i+1,si(g))if(lv[j]!=-1&&p[i]==p[j])
				p[j]=i;
		}
		return res;
	}
	int getflow(pi i){
		auto e=g[i.a][i.b];
		return g[e.to][e.rev].cap;
	}
};

//Bytecamp 2022 Day2 I
template<class d>
struct feasible_cycles{
	const int n;
	d tot;
	maxflow<d> mf;
	vc<d> up;
	vc<pi> eid;
	feasible_cycles(int nn):n(nn),tot(0),mf(n+2){}
	int ae(int i,int j,d l,d r){
		assert(0<=l);
		assert(l<=r);
		mf.ae(n,j,l);
		mf.ae(i,n+1,l);
		tot+=l;
		int res=si(eid);
		up.pb(r);
		eid.eb(i,mf.ae(i,j,r-l));
		return res;
	}
	bool slv(){
		return mf.calc(n,n+1)>=tot;
	}
	int getflow(int i){
		auto [v,e]=eid[i];
		return up[i]-mf.g[v][e].cap;
	}
};

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
	int n;
	vc<E> es;
	vi head,q;
	vc<pi> itr,lv;
	int curt;
	maxflow(int nn):n(nn),head(n,-1),q(n),itr(n),lv(n),curt(0){}
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
	//s:false, t:true
	vc<bool> getcut(){
		vc<bool> res(n);
		rep(i,n)res[i]=lv[i].b<curt;
		return res;
	}
};
