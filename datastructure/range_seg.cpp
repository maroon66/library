//UOJ Goodbye Renyin C
//upd(x,v) X座標 x に v を置く
//get(y) X座標が y 以下に置かれた値の max
struct N{
	int h=0,s=0;
	vi xs,vs;
	void add(int){
		s++;
	}
	void init(){
		xs.resize(s);
		vs.resize(s,-inf);
		s=0;
	}
	void add2(int x){
		xs[s++]=x;
	}
	void init2(){
	}
	void upd(int x,int v){
		for(x=lwb(xs,x);x<si(xs);x+=(x+1)&(-x-1))
			chmax(vs[x],v);
	}
	int get(int y){
		int res=-inf;
		for(y=lwb(xs,y+1)-1;y>=0;y-=(y+1)&(-y-1))
			chmax(res,vs[y]);
		return res;
	}
};

//UOJ Goodbye Renyin C
//upd(l,r,x,v) 区間 [l,r), 高さ x に v という値を置く
//get(l,r,y) 区間 [l,r) 高さ y 以下にある値の max を返す
struct seg{
	const int n,s;
	vc<N> a[2];
	bool initdone=false;
	seg(int nn):n(nn),s(minp2(n)){
		rep(k,2)a[k].resize(s*2);
	}
	vc<tuple<int,int,int>> abuf;
	void init(){
		rep(k,2)rep(i,2*s)a[k][i].init();
		sort(all(abuf));
		for(auto [x,l,r]:abuf){
			add2(l,r,x);
		}
		rep(k,2)rep(i,2*s)a[k][i].init2();
		initdone=true;
	}
	template<class F>
	void fpoint(F f,int k,int i){
		for(i+=s;i;i>>=1)f(k,i);
	}
	template<class F>
	void frange(F f,int k,int l,int r){
		for(l+=s,r+=s;l<r;l>>=1,r>>=1){
			if(l&1)f(k,l);
			if(r&1)f(k,r-1);
			l++; //ceil l
		}
	}
	void add(int l,int r,int x){
		assert(!initdone);
		abuf.eb(x,l,r);
		const auto f=[&](int k,int i){
			a[k][i].add(x);
		};
		fpoint(f,0,l);
		frange(f,1,l,r);
	}
	void add2(int l,int r,int x){
		assert(!initdone);
		const auto f=[&](int k,int i){
			a[k][i].add2(x);
		};
		fpoint(f,0,l);
		frange(f,1,l,r);
	}
	void upd(int l,int r,int x,int v){
		assert(initdone);
		const auto f=[&](int k,int i){
			a[k][i].upd(x,v);
		};
		fpoint(f,0,l);
		frange(f,1,l,r);
	}
	int get(int l,int r,int y){
		assert(initdone);
		int res=y;
		const auto f=[&](int k,int i){
			chmax(res,a[k][i].get(y));
		};
		frange(f,0,l,r);
		fpoint(f,1,l);
		return res;
	}
};
