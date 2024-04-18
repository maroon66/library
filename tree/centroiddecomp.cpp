//Nikkei2019 Final G
//XX Opencup GP of Warsaw I
template<class E>
struct cdecomp{
	const vvc<E>&g;
	int n;
	vi lv,rem;
	int ts(int v,int p){
		int res=1;
		for(auto e:g[v])if(e!=p&&!rem[e])
			res+=ts(e,v);
		return res;
	}
	int fc(int v,int p,int s){
		int ret=1,mx=0;
		for(auto e:g[v])if(e!=p&&!rem[e]){
			int f=fc(e,v,s);
			if(f<=0)
				return f;
			else{
				ret+=f;
				mx=max(mx,f);
			}
		}
		mx=max(mx,s-ret);
		if(mx*2<=s)
			return -v;
		else
			return ret;
	}
	void con(int r,int d){
		r=-fc(r,-1,ts(r,-1));
		lv[r]=d;
		
		initsub(r,d);
		
		rem[r]=1;
		for(auto e:g[r])if(!rem[e])
			con(e,d+1);
	}
	cdecomp(const vvc<E>&gg):g(gg),n(g.size()),
		lv(n),rem(n),nd(n){
		con(0,0);
		initfin();
	}
	//ここから先を編集
	struct N{
		int d,i,j;
	};
	//nd は絶対必要
	//各頂点，レベルごとにデータを保持
	vc<array<N,20>> nd;
	vc<vi> ds;
	void dfs(int v,int p,int h,int d,int i,int j){
		if(i==-1){
			i=ds.size();
			ds.eb();
		}else if(j==-1){
			j=ds.size();
			ds.eb();
		}
		nd[v][h]={d,i,j};
		if(i!=-1)
			ds[i].pb(d);
		if(j!=-1)
			ds[j].pb(d);
		for(auto e:g[v])if(e!=p&&!rem[e])
			dfs(e,v,h,d+1,i,j);
	}
	void initsub(int r,int h){
		dfs(r,-1,h,0,-1,-1);
	}
	void initfin(){
		for(auto&z:ds)
			sort(all(z));
	}
	int cntsub(int i,int r){
		return upper_bound(all(ds[i]),r)-ds[i].bg;
	}
	//頂点 v から距離 r 以下
	int query(int v,int r){
		if(r<0)return 0;
		int res=0;
		rep(i,lv[v]+1){
			N x=nd[v][i];
			if(r>=x.d){
				int w=r-x.d;
				res+=cntsub(x.i,w);
				if(x.j!=-1)
					res-=cntsub(x.j,w);
			}
		}
		return res;
	}
};

void init(){
	rep(i,n)initsub(i,fr[lv[i]],nd[lv[i]]);
}
//ここから先を編集
//Nikkei2019 Final G
struct N{
	int d,l,i;
};
vvc<N> nd;
vc<LineContainer> lcs;
void dfs(int v,int d,int l,int mx,int i,const vvc<E>&t,vc<N>&x){
	x[v]={d,l,i};
	for(auto e:t[v]){
		if(e.cost>mx)
			lcs[i].add(e.cost,l-e.cost*d);
		dfs(e.to,d+1,l+e.cost,max(e.cost,mx),i,t,x);
	}
}
void initsub(int r,const vvc<E>&t,vc<N>&x){
	int i=lcs.size();
	lcs.eb();
	dfs(r,0,0,0,i,t,x);
}
int query(int v,int l){
	int res=-inf;
	rep(i,lv[v]+1){
		N x=nd[i][v];
		if(l>=x.d)
			chmax(res,x.l+lcs[x.i].query(l-x.d));
	}
	return res;
}

//CF467E
//重心への距離のリストを取得
vc<array<pi,20>> nd;
void dfs(int v,int p,int h,int r,int d){
	nd[v][h]={r,d};
	
	for(auto e:g[v])if(e!=p&&!rem[e])
		dfs(e,v,h,r,d+1);
}
void initsub(int r,int h){
	dfs(r,-1,h,r,0);
}
void initfin(){
}

//オフラインで，各頂点からある距離の頂点数を取得する
//マークのついている頂点の個数だけ数える
//Petrozavodsk 2020w 300iq Contest A
//Yahoo 2018 Final C
template<class E>
struct cdecomp{
	const vvc<E>&g;
	int n;
	vi lv,rem;
	
	int ts(int v,int p){
		int res=1;
		for(auto e:g[v])if(e!=p&&!rem[e])
			res+=ts(e,v);
		return res;
	}
	int fc(int v,int p,int s){
		int ret=1,mx=0;
		for(auto e:g[v])if(e!=p&&!rem[e]){
			int f=fc(e,v,s);
			if(f<=0)
				return f;
			else{
				ret+=f;
				mx=max(mx,f);
			}
		}
		mx=max(mx,s-ret);
		if(mx*2<=s)
			return -v;
		else
			return ret;
	}
	
	cdecomp(const vvc<E>&gg):g(gg),n(g.size()),
		lv(n),rem(n){
			
		qs.resize(n);
		look.resize(n);
	}
	
	//ここらへんをいじる
	vi mark;
	vvc<pi> qs;
	vi ans;
	
	vvc<int> dbuf;
	vc<pi> look;
	void dfs1(int v,int p,int i,int j,int d){
		if(i==-1){
			i=dbuf.size();
			dbuf.eb();
		}else if(j==-1){
			j=dbuf.size();
			dbuf.eb();
		}
		look[v]=pi(i,j);
		if(mark[v]){
			if(i!=-1)dbuf[i].pb(d);
			if(j!=-1)dbuf[j].pb(d);
		}
		for(auto e:g[v])if(e!=p&&!rem[e])
			dfs1(e,v,i,j,d+1);
	}
	
	void dfs2(int v,int p,int d){
		int i,j;tie(i,j)=look[v];
		if(i!=-1){
			for(auto z:qs[v]){
				ans[z.b]+=upper_bound(all(dbuf[i]),z.a-d)-dbuf[i].bg;
			}
		}
		if(j!=-1){
			for(auto z:qs[v]){
				ans[z.b]-=upper_bound(all(dbuf[j]),z.a-d)-dbuf[j].bg;
			}
		}
		for(auto e:g[v])if(e!=p&&!rem[e])
			dfs2(e,v,d+1);
	}
	
	void con(int r,int d){
		r=-fc(r,-1,ts(r,-1));
		lv[r]=d;
		
		dbuf.clear();
		dfs1(r,-1,-1,-1,0);
		for(auto&ds:dbuf)sort(all(ds));
		dfs2(r,-1,0);
		
		rem[r]=1;
		for(auto e:g[r])if(!rem[e])
			con(e,d+1);
	}
	
	void aq(int v,int d){
		qs[v].eb(d,si(ans));
		ans.pb(0);
	}
	void slv(const vi&mk){
		mark=mk;
		con(0,0);
	}
};

//頂点間の距離の分布を求めるんだけど，typeとかついてる
//これはtype&1の点とtype&2の点の距離の分布
vc<mint> calc(const vc<pi>&z){
	vc<mint> x[2];
	for(auto v:z){
		int i=v.a;
		rep(k,2)if(v.b&1<<k){
			if(si(x[k])<=i){
				x[k].resize(i+1);
			}
			x[k][i]+=v.b/4;
		}
	}
	return multiply(x[0],x[1]);
}

template<class E>
struct cdecomp{
	const vvc<E>&g;
	int n;
	vi rem;
	
	int ts(int v,int p){
		int res=1;
		for(auto e:g[v])if(e!=p&&!rem[e])
			res+=ts(e,v);
		return res;
	}
	int fc(int v,int p,int s){
		int ret=1,mx=0;
		for(auto e:g[v])if(e!=p&&!rem[e]){
			int f=fc(e,v,s);
			if(f<=0)
				return f;
			else{
				ret+=f;
				mx=max(mx,f);
			}
		}
		mx=max(mx,s-ret);
		if(mx*2<=s)
			return -v;
		else
			return ret;
	}
	
	cdecomp(const vvc<E>&gg):g(gg),n(g.size()),
		rem(n){
		
	}
	
	//ここらへんをいじる
	
	vvc<pi> buf;
	vc<mint> ans;
	vi tp;
	
	void dfs1(int v,int p,int i,int j,int d){
		if(i==-1){
			i=buf.size();
			buf.eb();
		}else if(j==-1){
			j=buf.size();
			buf.eb();
		}
		if(tp[v]!=-1){
			if(i!=-1)buf[i].eb(d,si(g[v])*4+tp[v]);
			if(j!=-1)buf[j].eb(d,si(g[v])*4+tp[v]);
		}
		for(auto e:g[v])if(e!=p&&!rem[e])
			dfs1(e,v,i,j,d+1);
	}
	
	void con(int r){
		r=-fc(r,-1,ts(r,-1));
		
		dfs1(r,-1,-1,-1,0);
		rep(i,si(buf)){
			auto w=calc(buf[i]);
			if(i==0)rep(j,si(w))ans[j]+=w[j];
			else rep(j,si(w))ans[j]-=w[j];
		}
		buf.clear();
		
		rem[r]=1;
		for(auto e:g[r])if(!rem[e])
			con(e);
	}
	
	vc<mint> slv(const vi&t){
		fill(all(rem),0);
		tp=t;
		ans.assign(n,0);
		
		con(0);
		
		return ans;
	}
};


//KUPC 2020 C
//重心分解してクエリに答える
//クエリ (a,b) に対しては，LCA から a までのパスと，LCA から b までのパスの情報を使って答える．
//パスの情報は N に保存される
//N() が単位元的な役割
//N.extend(e) で辺 e を使って下に下って新しいノードを作る
//N 同士のマージをやると計算量が壊れるが extend なら．．．というときに使える
template<class E,class N>
struct cdecomp{
	const vvc<E>&g;
	int n;
	vi rem;
	
	int ts(int v,int p){
		int res=1;
		for(auto e:g[v])if(e!=p&&!rem[e])
			res+=ts(e,v);
		return res;
	}
	int fc(int v,int p,int s){
		int ret=1,mx=0;
		for(auto e:g[v])if(e!=p&&!rem[e]){
			int f=fc(e,v,s);
			if(f<=0)
				return f;
			else{
				ret+=f;
				mx=max(mx,f);
			}
		}
		mx=max(mx,s-ret);
		if(mx*2<=s)
			return -v;
		else
			return ret;
	}
	
	cdecomp(const vvc<E>&gg):g(gg),n(g.size()),rem(n){
	}
	
	vc<N> buf;
	vi tp;
	vvc<tuple<int,int,int>> bucket;
	
	void dfs1(int v,int p,int i,N cur){
		buf[v]=cur;
		tp[v]=i;
		for(auto e:g[v])if(e!=p&&!rem[e]){
			dfs1(e,v,i==-1?e:i,cur.extend(e));
		}
	}
	
	template<class F>
	void con(int r,const vc<tuple<int,int,int>>&qs,F f){
		r=-fc(r,-1,ts(r,-1));
		
		dfs1(r,-1,-1,N());
		for(const auto&w:qs){
			int a,b,i;tie(a,b,i)=w;
			if(tp[a]!=tp[b]){
				f(i,buf[a],buf[b],r);
			}else{
				bucket[tp[a]].pb(w);
			}
		}
		
		rem[r]=1;
		for(auto e:g[r])if(!rem[e]){
			vc<tuple<int,int,int>> tmp;
			tmp.swap(bucket[e]);
			con(e,tmp,f);
		}
	}
	
	//f(idx,N lf,N rt,int lca) がよばれる
	//[qs[i].a,lca) が lf に，[qs[i].b,lca) が rt に入るらしい
	template<class F>
	void slv(const vc<pi>&qs,F f){
		fill(all(rem),0);
		buf.resize(n);
		tp.resize(n);
		bucket.resize(n);
		
		vc<tuple<int,int,int>> tmp(si(qs));
		rep(i,si(qs))
			tmp[i]=mt(qs[i].a,qs[i].b,i);
		con(0,tmp,f);
	}
};

//example
auto slv=[&](int dst,N x,N y,int lca){
	x.off^=y.off;
	rep(i,y.s)x.add(y.vs[i]);
	if(cv[lca]!=-1)x.add(cv[lca]);
	ans[dst]=x.getans(ks[dst]);
};
