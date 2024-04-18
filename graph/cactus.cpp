//KUPC 2020 C
//XXII Opencup GP of Gomel E
//非連結な場合はverifyされてない

//多重辺なしの cactus を分解する
//cs にサイクルに使われた辺の集合が入る
//順番は，dfs 木を下る->後退辺で上がる，の順
//cs のサイズが 2 のときはシンプルに辺があるだけ
//連結性を仮定せず
template<class E>
struct cactus{
	const vvc<E>&g;
	const int n;
	vvc<E> cs;
	vi vis,par,u;
	vc<E> come;
	vc<bool> done;
	void dfs(int v,int p,E co){
		assert(vis[v]==0);
		vis[v]=1;
		par[v]=p;
		come[v]=co;
		E gopar;
		for(auto e:g[v]){
			if(e==p){
				gopar=e;
			}else if(vis[e]==0){
				dfs(e,v,e);
			}else if(vis[e]==1){
				int x=v;
				vc<E> z{e};
				while(x!=e){
					assert(!done[x]);
					done[x]=true;
					z.pb(come[x]);
					x=par[x];
				}
				reverse(all(z));
				cs.pb(z);
			}
		}
		if(p!=-1&&!done[v]){
			done[v]=true;
			cs.pb({co,gopar});
		}
		vis[v]=2;
	}
	cactus(const vvc<E>&gg):g(gg),n(g.size()),vis(n),par(n),u(n),come(n),done(n){
		rep(i,n)if(!vis[i])
			dfs(i,-1,E());
	}
};
struct E{
	int to,cost;
	operator int()const{return to;}
};

//Multiuni 2021 Day5 A
template<class E>
struct cactus_hld{
	const cactus<E>&cc;
	const int n;
	//Num of Vertices in the tree (N+num of cycles)
	const int m;
	vvc<pi> cycid;
	//v:cactus ID
	//a: vertex ID (in cycle of v)
	//return idx in cycle
	int getid(int v,int a){
		assert(v<m-n);
		auto itr=lower_bound(all(cycid[v]),pi(a,-1));
		assert(itr!=cycid[v].ed);
		assert(itr->a==a);
		return itr->b;
	}
	//v:cactus ID
	//a,b: vertex ID (in cycle of v)
	//return dist in cycle
	int getdistI(int v,int a,int b){
		if(a>b)swap(a,b);
		return min(b-a,a+si(cycid[v])-b);
	}
	//v:cactus ID
	//a,b: vertex ID (in the graph)
	//return dist in cycle
	int getdistV(int v,int a,int b){
		assert(v<m-n);
		a=getid(v,a);
		b=getid(v,b);
		return getdistI(v,a,b);
	}
	//v: cactus ID
	//returns: neighbour ID in the graph
	int getneighbourV(int v,int a){
		v+=n;
		assert(v<m);
		assert(a<n);
		if(!hld.asde(v,a))return par[v];
		int lw=0,up=si(hld.g[v]);
		while(up-lw>1){
			int mid=(lw+up)/2;
			if(hld.in[a]<hld.in[hld.g[v][mid]])
				up=mid;
			else
				lw=mid;
		}
		return hld.g[v][lw];
	}
	//v: cactus ID
	//returns: neighbour in the cycle
	int getneighbourI(int v,int a){
		return getid(v,getneighbourV(v,a));
	}
	vvc<int> t;
	vi par,len;
	void dfs1(int v,int p,int d){
		par[v]=p;
		len[v]=d;
		if(v<n){
			for(auto ch:t[v])if(ch!=p){
				dfs1(ch,v,d);
			}
		}else{
			for(auto ch:t[v])if(ch!=p){
				dfs1(ch,v,d+getdistV(v-n,ch,p));
			}
		}
	}
	HLD<int> hld;
	cactus_hld(const cactus<E>&tmp):cc(tmp),n(cc.n),m(cc.n+si(cc.cs)),
		cycid(si(cc.cs)),
		t(m),par(m,-1),len(m){
		
		rep(i,si(cc.cs)){
			cycid[i].resize(si(cc.cs[i]));
			rep(j,si(cc.cs[i]))
				cycid[i][j]=pi(cc.cs[i][j],j);
			sort(all(cycid[i]));
		}
		
		rep(i,m){
			if(i<n){
			}else{
				for(auto to:cc.cs[i-n]){
					t[i].pb(to);
					t[to].pb(i);
				}
			}
		}
		dfs1(0,-1,0);
		
		hld=HLD<int>(t,0);
	}
	int dist(int a,int b){
		int c=hld.lca(a,b);
		int ans=len[a]+len[b]-len[c]*2;
		if(c>=n){
			c-=n;
			a=getneighbourI(c,a);
			b=getneighbourI(c,b);
			int p=getid(c,par[c+n]);
			ans-=getdistI(c,a,p);
			ans-=getdistI(c,b,p);
			ans+=getdistI(c,a,b);
		}
		return ans;
	}
	//cactus での 3 頂点の合流点みたいなもの
	void query(){
		int a,b,c;cin>>a>>b>>c;
		a--;b--;c--;
		int tar=hld.lca(a,b);
		for(auto z:{hld.lca(a,c),hld.lca(b,c)}){
			if(hld.dep[tar]<hld.dep[z]){
				tar=z;
			}
		}
		int arw=a;
		if(tar>=n){
			tar-=n;
			a=getneighbourI(tar,a);
			b=getneighbourI(tar,b);
			c=getneighbourI(tar,c);
			
			int w=si(cc.cs[tar]);
			
			int blf=(b-a+w)%w,brt=(a-b+w)%w;
			int clf=(c-a+w)%w,crt=(a-c+w)%w;
			int dst=cc.cs[tar][a];
			if(blf<=brt&&clf<=crt){
				dst=cc.cs[tar][(a+min(blf,clf))%w];
			}
			if(brt<=blf&&crt<=clf){
				dst=cc.cs[tar][(a-min(brt,crt)+w)%w];
			}
			tar=dst;
		}
		cout<<tar+1<<" "<<dist(arw,tar)<<"\n";
	}
};

