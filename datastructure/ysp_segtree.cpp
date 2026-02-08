//CF616E
//strict-increasing な階段凸包の頂点数を数える
struct ysp_segtree{
	struct N{
		int cut,mx;
		N(int v=0):cut(0),mx(v){}
	};
	int s;
	vc<N> x;
	ysp_segtree(vi a){
		int n=si(a);
		s=1;
		while(s<n)s*=2;
		x.resize(s*2);
		rep(i,n)x[s+i]=N(a[i]);
		gnr(i,1,s)upd(i);
	}
	pi sub(int i,int v){
		if(x[i].mx<=v)return pi(0,v);
		if(i>=s)return pi(1,x[i].mx);
		if(v<x[i*2].mx){
			return pi(sub(i*2,v).a+x[i].cut,x[i].mx);
		}else{
			return sub(i*2+1,v);
		}
	}
	void upd(int i){
		assert(i<s);
		tie(x[i].cut,x[i].mx)=sub(i*2+1,x[i*2].mx);
	}
	void point_update(int i,int v){
		i+=s;
		x[i]=N(v);
		while(i>>=1)upd(i);
	}
	pi get(int b,int e,int v,int l,int r,int i){
		if(e<=l||r<=b)return pi(0,v);
		if(b<=l&&r<=e)return sub(i,v);
		int m=(l+r)/2;
		pi a=get(b,e,v,l,m,i*2);
		pi c=get(b,e,a.b,m,r,i*2+1);
		return pi(a.a+c.a,c.b);
	}
	//cnt,mx
	pi get(int b,int e,int v){
		assert(b<=e);
		return get(b,e,v,0,s,1);
	}
};


const int V=ten(9);

//increasing な階段凸包の面積を求める
//x 座標を V で切る
//UCup 1st Stage11(Shanghai) G
struct ysp_segtree{
	//葉ノードの cut には対応する点の x 座標を入れておく
	struct N{
		int cut,mx;
		N(pi v=pi(V,0)):cut(v.a),mx(v.b){}
	};
	int s;
	vc<N> x;
	ysp_segtree(vc<pi> a){
		int n=si(a);
		s=1;
		while(s<n)s*=2;
		x.resize(s*2);
		rep(i,n)x[s+i]=N(a[i]);
		gnr(i,1,s)upd(i);
	}
	//cut,mx
	//cut というのは，高さ v より上にある部分の面積
	pi sub(int i,int v){
		if(x[i].mx<=v)return pi(0,v);
		if(i>=s)return pi((x[i].mx-v)*(V-x[i].cut),x[i].mx);
		if(v<x[i*2].mx){
			return pi(sub(i*2,v).a+x[i].cut,x[i].mx);
		}else{
			return sub(i*2+1,v);
		}
	}
	void upd(int i){
		assert(i<s);
		tie(x[i].cut,x[i].mx)=sub(i*2+1,x[i*2].mx);
	}
	void point_set(int i,pi v){
		i+=s;
		x[i]=N(v);
		while(i>>=1)upd(i);
	}
	pi get(int b,int e,int v,int l,int r,int i){
		if(e<=l||r<=b)return pi(0,v);
		if(b<=l&&r<=e)return sub(i,v);
		int m=(l+r)/2;
		pi a=get(b,e,v,l,m,i*2);
		pi c=get(b,e,a.b,m,r,i*2+1);
		return pi(a.a+c.a,c.b);
	}
	//cnt,mx
	pi get(int b,int e,int v){
		assert(b<=e);
		return get(b,e,v,0,s,1);
	}
};

//UCUP 2-25-K

//UCUP 4-12-K
//prefix min/max そしてその積の和
const int none=LLONG_MIN;
struct ysp_segtree{
	struct Res{
		ull minsum,maxsum,mulsum;
		void add(ull len,int v){
			mulsum+=minsum*v+maxsum*v+len*v*v;
			minsum+=len*v;
			maxsum+=len*v;
		}
		void set(ull len,int v){
			minsum=len*v;
			maxsum=len*v;
			mulsum=len*v*v;
		}
		Res operator+(const Res&rhs)const{
			return Res{minsum+rhs.minsum,maxsum+rhs.maxsum,mulsum+rhs.mulsum};
		}
	};
	struct N{
		int lzadd,lzset;
		int minval,maxval;
		Res cut;
		N():lzadd(0),lzset(none),minval(inf),maxval(-inf){}
		N(int v):lzadd(0),lzset(none),minval(v),maxval(v){}
		void add(ull len,int v){
			if(lzset==none){
				lzadd+=v;
				minval+=v;
				maxval+=v;
				cut.add(len/2,v);
			}else{
				lzset+=v;
				minval=maxval=lzset;
				cut.set(len/2,lzset);
			}
		}
		void set(ull len,int v){
			lzset=v;
			minval=maxval=lzset;
			cut.set(len/2,lzset);
		}
		void push(ull len,N&x,N&y){
			if(lzset==none){
				x.add(len/2,lzadd);
				y.add(len/2,lzadd);
			}else{
				x.set(len/2,lzset);
				y.set(len/2,lzset);
			}
			lzadd=0;
			lzset=none;
		}
	};
	int s;
	vc<N> x;
	ysp_segtree(vi a){
		int n=si(a);
		s=1;
		while(s<n)s*=2;
		x.resize(s*2);
		rep(i,n)x[s+i]=N(a[i]);
		gnr(i,1,s)upd(i);
	}
	ull ask_min(ull len,int i,int&minval){
		if(len==1){
			chmin(minval,x[i].minval);
			return minval;
		}else{
			push(i);
			if(x[i].minval<minval){
				if(x[i*2].minval<minval){
					ull res=ask_min(len/2,i*2,minval);
					res+=x[i].cut.minsum;
					return res;
				}else{
					ull res=len/2*minval;
					res+=ask_min(len/2,i*2+1,minval);
					return res;
				}
			}else{
				return len*minval;
			}
		}
	}
	ull ask_max(ull len,int i,int&maxval){
		if(len==1){
			chmax(maxval,x[i].maxval);
			return maxval;
		}else{
			push(i);
			if(x[i].maxval>maxval){
				if(x[i*2].maxval>maxval){
					ull res=ask_max(len/2,i*2,maxval);
					res+=x[i].cut.maxsum;
					return res;
				}else{
					ull res=len/2*maxval;
					res+=ask_max(len/2,i*2+1,maxval);
					return res;
				}
			}else{
				return len*maxval;
			}
		}
	}
	Res ask_minmax(ull len,int i,int&minval,int&maxval){
		if(len==1){
			chmin(minval,x[i].minval);
			chmax(maxval,x[i].maxval);
			return Res{(ull)minval,(ull)maxval,(ull)minval*maxval};
		}else{
			push(i);
			if(minval<=x[i].minval&&x[i].maxval<=maxval){
				return Res{len*minval,len*maxval,len*minval*maxval};
			}else{
				if(x[i*2].minval<minval&&x[i*2].maxval>maxval){
					Res res=ask_minmax(len/2,i*2,minval,maxval);
					chmin(minval,x[i].minval);
					chmax(maxval,x[i].maxval);
					return res+x[i].cut;
				}else{
					Res res;
					if(x[i*2].minval<minval){
						assert(x[i*2].maxval<=maxval);
						res.minsum=ask_min(len/2,i*2,minval);
						res.maxsum=len/2*maxval;
						res.mulsum=res.minsum*maxval;
					}else if(x[i*2].maxval>maxval){
						assert(x[i*2].minval>=minval);
						res.maxsum=ask_max(len/2,i*2,maxval);
						res.minsum=len/2*minval;
						res.mulsum=res.maxsum*minval;
					}else{
						res.minsum=len/2*minval;
						res.maxsum=len/2*maxval;
						res.mulsum=len/2*minval*maxval;
					}
					chmin(minval,x[i*2].minval);
					chmax(maxval,x[i*2].maxval);
					Res z=ask_minmax(len/2,i*2+1,minval,maxval);
					chmin(minval,x[i].minval);
					chmax(maxval,x[i].maxval);
					return res+z;
				}
			}
		}
	}
	void push(int i){
		assert(i<s);
		int t=topbit(i);
		ull len=s>>t;
		
		x[i].push(len,x[i*2],x[i*2+1]);
	}
	void upd(int i){
		assert(i<s);
		int t=topbit(i);
		ull len=s>>t;
		
		x[i].minval=x[i*2].minval;
		x[i].maxval=x[i*2].maxval;
		x[i].cut=ask_minmax(len/2,i*2+1,x[i].minval,x[i].maxval);
	}
	void point_update(int i,int v){
		i+=s;
		x[i]=N(v);
		while(i>>=1)upd(i);
	}
	Res get(int l,int r,int i,int b,int e,int&minval,int&maxval){
		if(e<=l||r<=b)return Res{0,0,0};
		if(b<=l&&r<=e)return ask_minmax(r-l,i,minval,maxval);
		push(i);
		Res p=get(l,(l+r)/2,i*2+0,b,e,minval,maxval);
		Res q=get((l+r)/2,r,i*2+1,b,e,minval,maxval);
		return p+q;
	}
	Res get(int b,int e){
		assert(b<=e);
		int minval=inf,maxval=-inf;
		return get(0,s,1,b,e,minval,maxval);
	}
	void add(int l,int r,int i,int b,int e,int v){
		if(e<=l||r<=b)return;
		if(b<=l&&r<=e)return x[i].add(r-l,v);
		push(i);
		add(l,(l+r)/2,i*2+0,b,e,v);
		add((l+r)/2,r,i*2+1,b,e,v);
		return upd(i);
	}
	void add(int b,int e,int v){
		assert(b<=e);
		return add(0,s,1,b,e,v);
	}
	void set(int l,int r,int i,int b,int e,int v){
		if(e<=l||r<=b)return;
		if(b<=l&&r<=e)return x[i].set(r-l,v);
		push(i);
		set(l,(l+r)/2,i*2+0,b,e,v);
		set((l+r)/2,r,i*2+1,b,e,v);
		return upd(i);
	}
	void set(int b,int e,int v){
		assert(b<=e);
		return set(0,s,1,b,e,v);
	}
};
