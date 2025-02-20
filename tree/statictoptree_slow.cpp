//N にデフォルトコンストラクタがなくても動く親切設計
template<class N>
struct optimal_segtree{
	optimal_segtree(){}
	vc<N> x;
	int n;
	vi hei,par;
	vc<array<int,2>> ch;
	optimal_segtree(vc<N> a,vi hs):x(move(a)),n(si(x)),hei(hs),par(2*n-1,-1),ch(2*n-1,{-1,-1}){
		assert(si(hs)==n);
		//https://atcoder.jp/contests/abc351/editorial/9910
		vc<pi> buf;
		auto mg=[&](){
			// bufの最後の2要素をマージし、マージしたものをbufにpushする
			auto [v,vd]=gpp(buf);
			auto [u,ud]=gpp(buf);
			int w=si(x),wd=max(ud,vd)+1;
			x.pb(N::merge(x[u],x[v]));
			hei.pb(wd);
			par[u]=w;
			par[v]=w;
			ch[w]={u,v};
			buf.eb(w,wd);
		};
		rep(i,n){
			buf.eb(i,hs[i]);
			while (true) {
				int len = buf.size();
				if (len >= 3 && (buf[len - 3].b == buf[len - 2].b || buf[len - 3].b <= buf[len - 1].b)) {
					auto z = gpp(buf);
					mg();
					buf.pb(z);
				} else if (len >= 2 && buf[len - 2].b <= buf[len - 1].b) {
					mg();
				} else {
					break;
				}
			}
		}
		while (buf.size() >= 2) mg();
		assert(si(x)==2*n-1);
	}
	void point_set(int i,N a){
		x[i]=move(a);
		while((i=par[i])!=-1){
			x[i]=N::merge(x[ch[i][0]],x[ch[i][1]]);
		}
	}
	const N& getall()const{
		return x.back();
	}
	int getheight()const{
		return hei.back();
	}
	bool empty()const{return x.empty();}
};

//ABC351G
//Multiuni 2024-7-E
template<class E,class N,class M,class F>
struct Online_TreeDP{
	HLD<E> hld;
	int n;
	vi lid;
	vc<optimal_segtree<N>> seg;
	vc<optimal_segtree<M>> lch;
	F genN_;
	N genN(int v){
		if(lch[v].empty())return genN_(v,M());
		else return genN_(v,lch[v].getall());
	}
	Online_TreeDP(const vvc<E>&t,F f):hld(t,0),n(si(t)),lid(n,-1),
		seg(n),lch(n),genN_(f){
		for(auto v:reout(hld.ni)){
			{
				int s=max<int>(si(hld.g[v])-1,0);
				if(s){
					vc<M> ls;ls.reserve(s);
					vi hei(s);
					rep(i,s){
						int to=hld.g[v][i+1];
						lid[to]=i;
						ls.eb(seg[to].getall());
						hei[i]=seg[to].getheight();
					}
					lch[v]=optimal_segtree<M>(move(ls),hei);
				}
			}
			if(hld.head[v]==v){
				int s=hld.hei[v];
				vc<N> ls;ls.reserve(s);
				vi hei(s);
				rep(i,s){
					int to=hld.adv(v,i);
					ls.pb(genN(to));
					hei[i]=lch[to].empty()?0:lch[to].getheight()+1;
				}
				seg[v]=optimal_segtree<N>(move(ls),hei);
			}
		}
	}
	pi getij(int v){
		int h=hld.head[v];
		return pi(h,hld.dep[v]-hld.dep[h]);
	}
	void upd_seg(int v){
		auto [i,j]=getij(v);
		seg[i].point_set(j,genN(v));
	}
	//v についてなにか操作したらこれを呼ぶ!
	void upd(int v){
		upd_seg(v);
		
		while(v>=0){
			v=hld.head[v];
			int p=hld.par[v];
			if(p!=-1){
				lch[p].point_set(lid[v],M(seg[v].getall()));
				upd_seg(p);
			}
			v=p;
		}
	}
	N getall(){
		return seg[0].getall();
	}
};
template<class N,class M,class E,class F>
auto getTreeDP(const vvc<E>&t,F f){
	return Online_TreeDP<E,N,M,F>(t,f);
}
//log が 1 つ
//genN(int v,M m) (m は light children の情報)
//N: compress が起こるやつ
//M: rake が起こるやつ，根の頂点の情報を持たないよ
//M(N) というコンストラクタで N->M の変換が必要
//N はデフォルトコンストラクタいりません
//M は必要です

//↓例: 木の最大マッチングを求める
struct N{
	int v[2][2];
	N(){rep(i,2)rep(j,2)v[i][j]=-inf;}
	static N merge(const N&a,const N&b){
		N res;
		rep(i,2)rep(j,2)rep(k,2)rep(l,2){
			chmax(res.v[i][l],a.v[i][j]+b.v[k][l]+(j&&k));
		}
		return res;
	}
	int getans()const{
		return v[0][0];
	}
};

struct M{
	int v[2];
	M(){v[0]=0;v[1]=-inf;}
	M(N n){
		rep(i,2){
			v[i]=-inf;
			rep(j,2)chmax(v[i],n.v[i][j]);
		}
	}
	static M merge(const M&a,const M&b){
		M res;
		rep(i,2)rep(j,2){
			chmax(res.v[i||j],a.v[i]+b.v[j]);
		}
		return res;
	}
	int getans()const{
		return v[0];
	}
};

vi matching_sizes(const vvc<int>&t,vc<pi> es){
	int n=si(t);
	vi avail(n,true);
	auto genN=[&](int v,M m){
		N res;
		int z=m.getans();
		chmax(res.v[0][0],z);
		if(avail[v]){
			chmax(res.v[0][0],m.v[1]+1);
			chmax(res.v[1][0],z);
			chmax(res.v[0][1],z);
		}
		return res;
	};
	
	auto dp=getTreeDP<N,M>(t,genN);
	vi res;
	res.pb(dp.getall().getans());
	for(auto [a,b]:es){
		avail[a]=false;
		dp.upd(a);
		avail[b]=false;
		dp.upd(b);
		res.pb(dp.getall().getans());
	}
	return res;
}

//UCUP 3-10-H
//TLE なのですが…
